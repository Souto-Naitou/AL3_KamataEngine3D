#include "Player.h"
#include <cassert>
#include <ImGuiManager.h>
#include "TextureManager.h"
#include <WinApp.h>

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

	uint32_t textureReticle = TextureManager::Load("x.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f, 0.5f });
}

void Player::Update(const ViewProjection& _viewProjection)
{
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

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 16;

	// 範囲を超えない処理
	worldTransform_.translation_.x = clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
	

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	UpdateWorldTransform3DReticle();
	Update3DReticleWithCursor(_viewProjection);

#pragma region 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算

	Vector3 positionReticle = GetWorldPosition(worldTransform3DReticle_.matWorld_);

	// ビューポート行列
	matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1.0f);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = _viewProjection.matView * _viewProjection.matProjection * matViewport;

	// ワールドｰ>スクリーン座標
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

#pragma endregion

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

	reticleModel->Draw(worldTransform3DReticle_, _viewProjection);
}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		// 弾の速度
		const float kbulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kbulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = GetWorldPosition(worldTransform3DReticle_.matWorld_) - GetWorldPosition(worldTransform_.matWorld_);
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

void Player::UpdateWorldTransform3DReticle()
{

#pragma region 自機のワールド座標から3Dレティクルのワールド座標を計算

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0,0,1.0f };
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = GetWorldPosition(worldTransform_.matWorld_) + offset;
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
	Vector2 spritePosition;
	spritePosition = sprite2DReticle_->GetPosition();
	ImGui::Text("2DReticle:(%f,%f)", spritePosition.x, spritePosition.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
		"3DReticle:(%+.2f,%+.2f,%+.2f)",
		worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y,
		worldTransform3DReticle_.translation_.z
	);
	ImGui::End();
}

Player::~Player()
{
	// bullet_の解放
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
	delete sprite2DReticle_;
}
