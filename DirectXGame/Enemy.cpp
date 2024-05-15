#include "Enemy.h"
#include <TextureManager.h>

void Enemy::Initialize(Model* _model, const Vector3& _position, const Vector3& _velocity)
{
	textureHandle_ = TextureManager::Load("img/enemy.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = _position;
	model_ = _model;
	velocity_ = _velocity;
}

void Enemy::Update()
{
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	switch (phase_)
	{
	case Phase::Approach:
	default:
		// 接近
		Phase_Approach();
		break;

	case Phase::Leave:
		// 離脱
		Phase_Leave();
		break;
	}
}

void Enemy::Draw(const ViewProjection& _viewProjection)
{
	model_->Draw(worldTransform_, _viewProjection, textureHandle_);
}
