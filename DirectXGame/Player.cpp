#include "Player.h"
#include <cassert>
#include <ImGuiManager.h>

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

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	worldTransform_.translation_ = _position;
}

void Player::Update() 
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
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP))
	{
		move.y += kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
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

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	Rotate();

	// TODO: 次回！時間経過で消失！

	ImGui::Begin("pos");

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

}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// 弾の速度
		const float kbulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kbulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		newBullet->SetParent(worldTransform_.parent_);

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
	// 回転速さ{ラジアン / frame}
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A))
	{
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

Player::~Player()
{
	// bullet_の解放
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
}
