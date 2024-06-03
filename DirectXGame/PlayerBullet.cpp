#include "PlayerBullet.h"
#include <cassert>
#include "TextureManager.h"

void PlayerBullet::Initialize(Model* _model, const Vector3& _position, const Vector3& _velocity)
{
	assert(_model);

	model_ = _model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = _position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = _velocity;
}

void PlayerBullet::Update()
{
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& _viewProjection)
{
	model_->Draw(worldTransform_, _viewProjection, textureHandle_);
}

void PlayerBullet::SetParent(const WorldTransform* _parent)
{
	worldTransform_.parent_ = _parent;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	Vector3 result;
	result.x = worldTransform_.matWorld_.m[3][0];
	result.y = worldTransform_.matWorld_.m[3][1];
	result.z = worldTransform_.matWorld_.m[3][2];
	return result;
}
