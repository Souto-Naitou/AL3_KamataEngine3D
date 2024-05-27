#include "Enemy.h"
#include <TextureManager.h>
#include <MathExtension/mathExtension.h>
#include "Player.h"
#include "Vector3/calc/vector3calc.h"
#include <cassert>

Enemy::~Enemy()
{
	for (const EnemyBullet* bullet : bullets_)
	{
		delete bullet;
		bullet = nullptr;
	}
}

void Enemy::Initialize(Model* _model, const Vector3& _position, const Vector3& _velocity)
{
	textureHandle_ = TextureManager::Load("img/enemy.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = _position;
	model_ = _model;
	velocity_ = _velocity;
	ApproachPhaseInitialize();
}

void Enemy::Update()
{
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		}
	);

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	ApproachPhaseUpdate();

	// 弾更新
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Update();
	}
	
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
	// 弾描画
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Draw(_viewProjection);
	}
}

void Enemy::ApproachPhaseInitialize()
{
	// 発射タイマー初期化
	shotRecastTime = kFireInterval;
}

void Enemy::ApproachPhaseUpdate()
{
	// 発射タイマーカウントダウン
	shotRecastTime--;
	// 指定時間に達した
	if (shotRecastTime == 0)
	{
		Fire();
		// 発射タイマーを初期化
		shotRecastTime = kFireInterval;
	}
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Fire()
{
	assert(player_);
	// 弾の速度
	const float kbulletSpeed = 0.5f;
	Vector3 velocity(0, 0, kbulletSpeed);

	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = this->GetWorldPosition();

	Vector3 subVector = Subtract(playerPosition, enemyPosition);
	Vector3 normVector = Normalize(subVector);

	Vector3 result = Multiply(kbulletSpeed, normVector);

	//// 速度ベクトルを自機の向きに合わせて回転させる
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, result);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::OnCollision()
{
}
