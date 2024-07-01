#include "Enemy.h"
#include <TextureManager.h>
#include <MathExtension/mathExtension.h>
#include "Player.h"
#include "Vector3/calc/vector3calc.h"
#include <cassert>
#include "GameScene.h"
#include <functional>

Enemy::~Enemy()
{
}

void Enemy::Initialize(Model* _model, const Vector3& _position, const Vector3& _velocity)
{
	textureHandle_ = TextureManager::Load("img/enemy.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = _position;
	worldTransform_.UpdateMatrix();
	model_ = _model;
	velocity_ = _velocity;
	// 発射タイマー初期化
	shotRecastTime = kFireInterval;
	ChangeState(new EnemyStateApproach(this));
}

void Enemy::Update()
{
	if (timedCalls_.empty()) ShotAndReset();
	state_->Update();

	//// 発射タイマーカウントダウン
	//enemy_->shotRecastTime--;

	//// 指定時間に達した
	//if (enemy_->shotRecastTime == 0)
	//{
	//	enemy_->Fire();
	//	// 発射タイマーを初期化
	//	enemy_->shotRecastTime = enemy_->kFireInterval;
	//}

	// 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* _timedCall) {
		if (_timedCall->IsFinished())
		{
			delete _timedCall;
			return true;
		}
		return false;
	});

	for (TimedCall* timedCall : timedCalls_)
	{
		timedCall->Update();
	}

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& _viewProjection)
{
	model_->Draw(worldTransform_, _viewProjection, textureHandle_);
}

void Enemy::ChangeState(BaseEnemyState* _state)
{
	delete state_;
	state_ = _state;
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::SetTranslate(const Vector3& _translate)
{
	worldTransform_.translation_ = _translate;
	worldTransform_.UpdateMatrix();
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
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::OnCollision()
{
	isDead_ = true;
}

void Enemy::ShotAndReset()
{
	Fire();
	// 発射タイマー初期化
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::ShotAndReset, this), this->shotRecastTime));
}

void EnemyStateApproach::Update()
{
	Vector3 enemyTranslate = enemy_->GetWorldPosition();
	// 移動 (ベクトルを加算)
	enemyTranslate += Vector3(0, 0, -0.2f);
	enemy_->SetTranslate(enemyTranslate);
	// 規定の位置に到着したら離脱
	if (enemyTranslate.z < 0.0f)
	{
		enemy_->ChangeState(new EnemyStateLeave(enemy_));
	}
	return;
}

void EnemyStateLeave::Update()
{
	// 移動 （ベクトル加算）
	Vector3 enemyTranslate = enemy_->GetWorldPosition();
	enemyTranslate += Vector3(-0.5f, 0.5f, 0.1f);
	enemy_->SetTranslate(enemyTranslate);
	return;
}

BaseEnemyState::BaseEnemyState(Enemy* _enemy)
{
	enemy_ = _enemy;
	return;
}
