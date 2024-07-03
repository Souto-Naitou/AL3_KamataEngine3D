#include "EnemyBullet.h"
#include <cassert>
#include "TextureManager.h"
#include <Vector3/calc/vector3calc.h>
#include "math/MathExtension/mathExtension.h"

void EnemyBullet::Initialize(Model* _model, const Vector3& _position, const Vector3& _velocity)
{
	assert(_model);

	model_ = _model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 3.0f);
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = _position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = _velocity;
	float velocityXZDist = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
	worldTransform_.rotation_.x = std::atan2f(-velocity_.y, velocityXZDist);
}

void EnemyBullet::Update()
{
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);
	// ベクトルを正規化
	Normalize(toPlayer);
	Normalize(velocity_);
	// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度へ
	velocity_ = Slerp(velocity_, toPlayer, 0.04f) * 0.5f;

	// 進行方向に見た目の回転を合わせる
	float velocityXZDist = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
	worldTransform_.rotation_.x = std::atan2f(-velocity_.y, velocityXZDist);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& _viewProjection)
{
	model_->Draw(worldTransform_, _viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	Vector3 result;
	result.x = worldTransform_.matWorld_.m[3][0];
	result.y = worldTransform_.matWorld_.m[3][1];
	result.z = worldTransform_.matWorld_.m[3][2];
	return result;
}
