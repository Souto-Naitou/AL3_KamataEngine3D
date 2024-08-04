#include "Player.h"
#include <cassert>
#include <ImGuiManager.h>
#include "TextureManager.h"
#include <WinApp.h>
#include "Enemy.h"

#include "Matrix4x4/calc/matrix4calc.h"
#include "Vector3/calc/vector3calc.h"
#include "MathExtension/mathExtension.h"

void Player::Initialize(Model* _model, uint32_t _textureHandle, Vector3 _position)
{
	// NULLチェック
	assert(_model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = _model;
	textureHandle_ = _textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform3DReticle_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	worldTransform_.translation_ = _position;

	reticleModel = Model::CreateFromOBJ("cube");

	textureReticle_ = TextureManager::Load("x.png");
	sprite2DReticle_ = Sprite::Create(textureReticle_, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f, 0.5f });
	sprite2DReticleLockon_.clear();

	// 衝突属性を設定
	SetCollisionAttribute(collisionAttribute_);

	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(collisionMask_);

	matViewport = MakeViewportMatrix(0, 0, 1280, 720, 0.0f, 100.0f);

	spritePosition = Vector2(640, 360);
}

void Player::Update(const ViewProjection& _viewProjection)
{
	_viewProjection;
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		}
	);

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A))
	{
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_W))
	{
		move.y += kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		move.y -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_LEFTARROW)) spritePosition.x -= 10.0f;
	if (input_->PushKey(DIK_RIGHTARROW)) spritePosition.x += 10.0f;
	if (input_->PushKey(DIK_UPARROW)) spritePosition.y -= 10.0f;
	if (input_->PushKey(DIK_DOWNARROW)) spritePosition.y += 10.0f;
	sprite2DReticle_->SetPosition(spritePosition);

	// 移動限界座標
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 16;

	// 範囲を超えない処理
	worldTransform_.translation_.x = clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
	

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState_))
	{
		move.x += static_cast<float>(joyState_.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed);
		move.y += static_cast<float>(joyState_.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed);
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// スプライトの現在座標を取得
	spritePosition = sprite2DReticle_->GetPosition();

	if (spritePosition.x + spritePosition.y == 0) spritePosition.y = 0.1f;

	Update3DReticleWithCursor(_viewProjection);

	for (Enemy* enemy : *enemiesList_)
	{
		Vector3 epos = enemy->GetScreenPosition();

		float enemyLen = Length(epos);
		float reticleLen = Length(Vector3{ spritePosition.x, spritePosition.y, 0 });

		if (abs(enemyLen - reticleLen) < 10.0f)
		{
			bool isSame = false;
			for (auto lcEnemy : lockonEnemyPairList_)
			{
				if (lcEnemy.first == enemy)
				{
					isSame = true;
					break;
				}
			}
			if (!isSame)
			{
				lockonEnemyPairList_.push_back({});
				lockonEnemyPairList_.back().first = enemy;
				lockonEnemyPairList_.back().second = Sprite::Create(textureReticle_, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f, 0.5f });
			}
		}
	}

	//if (Input::GetInstance()->GetJoystickState(0, joyState_))
	//{
	//	spritePosition.x += static_cast<float>(joyState_.Gamepad.sThumbRX / SHRT_MAX * 10.0f);
	//	spritePosition.y -= static_cast<float>(joyState_.Gamepad.sThumbRY / SHRT_MAX * 10.0f);

	//	// スプライトの座標変更を反映
	//	sprite2DReticle_->SetPosition(spritePosition);
	//}

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	Rotate();

	// ImGui
	ImGui::Begin("Player");
	ImGui::Text("x:%3.1f", worldTransform_.translation_.x);
	ImGui::Text("y:%3.1f", worldTransform_.translation_.y);

	Vector3 vect = {};
	for (float t = 0; t < 1.0f; t += 0.01f)
	{
		vect = Slerp(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), t);
	}
	

	ImGui::End();
}

void Player::Draw(ViewProjection& _viewProjection)
{
	model_->Draw(worldTransform_, _viewProjection, textureHandle_);

	// 弾描画
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(_viewProjection);
	}

	//reticleModel->Draw(worldTransform3DReticle_, _viewProjection);
}

void Player::DrawUI()
{
	for (auto lockon : lockonEnemyPairList_)
	{
		Vector3 pos = lockon.first->GetScreenPosition();
		lockon.second->SetSize(Vector2(20, 20));
		lockon.second->SetPosition({ pos.x, pos.y });
		lockon.second->Draw();
	}
	sprite2DReticle_->Draw();
}

void Player::Attack()
{
	//if (!Input::GetInstance()->GetJoystickState(0, joyState_)) return;

	if (input_->TriggerKey(DIK_SPACE) || joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		for (auto lockon : lockonEnemyPairList_)
		{
			Vector3 velocity(0, 0, kbulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = lockon.first->GetWorldPosition() - GetWorldPosition(worldTransform_.matWorld_);
			velocity = Multiply(kbulletSpeed, Normalize(velocity));
			//velocity = TransformNormal(velocity, worldTransform_.matWorld_);


			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(worldTransform_.matWorld_), velocity);
			//newBullet->SetParent(worldTransform_.parent_);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldPosition(const Matrix4x4& _m)
{
	Vector3 worldPos;

	worldPos.x = _m.m[3][0];
	worldPos.y = _m.m[3][1];
	worldPos.z = _m.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{
	// Do nothing
}

void Player::SetParent(const WorldTransform* _parent)
{
	worldTransform_.parent_ = _parent;
}

void Player::Rotate()
{
	return;
}

void Player::UpdateWorldTransform3DReticle(Vector3 _translation)
{

#pragma region 自機のワールド座標から3Dレティクルのワールド座標を計算

	//// 自機から3Dレティクルへの距離
	//const float kDistancePlayerTo3DReticle = 100.0f;
	//// 自機から3Dレティクルへのオフセット(Z+向き)
	//Vector3 offset = { 0,0,1.0f };
	//// 自機のワールド行列の回転を反映
	//offset = TransformNormal(offset, worldTransform_.matWorld_);
	//// ベクトルの長さを整える
	//offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = _translation;
	worldTransform3DReticle_.UpdateMatrix();

#pragma endregion
	
}

void Player::GetCursorPosition()
{
	// マウス座標を取得(スクリーン座標)
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	return;
}

void Player::Update3DReticleWithCursor(const ViewProjection& _viewProjection)
{
	// マウス座標の取得・計算
	GetCursorPosition();
	// ビュープロジェクションビューポートの合成行列
	Matrix4x4 matVPV = _viewProjection.matView * _viewProjection.matProjection * matViewport;
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	mousePosition.x = static_cast<long>(spritePosition.x);
	mousePosition.y = static_cast<long>(spritePosition.y);
	if (mousePosition.x + mousePosition.y == 0)
	{
		mousePosition.x = 1;
		mousePosition.y = 1;
	}
	// スクリーン座標
	Vector3 posNear = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0);
	Vector3 posFar = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 1);

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイ
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 30.0f;
	worldTransform3DReticle_.translation_ = posNear + (mouseDirection * kDistanceTestObject);
	worldTransform3DReticle_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", spritePosition.x, spritePosition.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
		"3DReticle:(%+.2f,%+.2f,%+.2f)",
		worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y,
		worldTransform3DReticle_.translation_.z
	);
	ImGui::Text("BulletCount : %u", bullets_.size());
	ImGui::End();
}

Player::~Player()
{
	// bullet_の解放
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
	for (auto lockon : lockonEnemyPairList_)
	{
		delete lockon.second;
	}
	delete sprite2DReticle_;
}
