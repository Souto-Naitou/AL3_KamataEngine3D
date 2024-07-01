#pragma once

#include <WorldTransform.h>
#include <Model.h>
#include "EnemyBullet.h"
#include <TimedCall.h>

class Player;
class GameScene;

enum class Phase
{
	Approach,	// 接近する
	Leave,		// 離脱する
};

class BaseEnemyState;
class EnemyStateApproach;
class EnemyStateLeave;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	static const int kFireInterval = 60;
	int32_t shotRecastTime = 0;

	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void	Initialize(Model* _model, const Vector3& _position, const Vector3& _velocity);

	/// <summary>
	/// 更新処理
	/// </summary>
	void	Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void	Draw(const ViewProjection& _viewProjection);

	void	ChangeState(BaseEnemyState* _state);

	void	SetPlayer(Player* _player) { player_ = _player; }
	Vector3 GetWorldPosition();
	void	SetTranslate(const Vector3& _translate);

	void	Fire();

	void	OnCollision();
	void	SetGameScene(GameScene* _gameScene) { gameScene_ = _gameScene; }
	bool	IsDead() const { return isDead_; };

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void	ShotAndReset();

private:

	WorldTransform	worldTransform_;
	Model* model_ = nullptr;
	uint32_t		textureHandle_ = 0u;
	Vector3			velocity_;

	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;
	bool			isDead_ = false;

	// 時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	BaseEnemyState* state_;
};

class BaseEnemyState
{
protected:
	Enemy* enemy_;
public:
	BaseEnemyState(Enemy*);
	virtual void Update() {};
};

class EnemyStateApproach : public BaseEnemyState
{
public:
	EnemyStateApproach(Enemy* _enemy) : BaseEnemyState(_enemy) {};
	void Update();
};

class EnemyStateLeave : public BaseEnemyState
{
public:
	EnemyStateLeave(Enemy* _enemy) : BaseEnemyState(_enemy) {};
	void Update();
};