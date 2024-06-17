#pragma once

#include <WorldTransform.h>
#include <Model.h>
#include "EnemyBullet.h"

class Player;
class GameScene;

enum class Phase
{
	Approach,	// 接近する
	Leave,		// 離脱する
};

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

	void	ApproachPhaseInitialize();
	void	ApproachPhaseUpdate();

	void	SetPlayer(Player* _player) { player_ = _player; }
	Vector3	GetWorldPosition();

	void	Fire();

	void	OnCollision();
	void	SetGameScene(GameScene* _gameScene) { gameScene_ = _gameScene; }
	bool	IsDead() const { return isDead_; };

private:

	WorldTransform	worldTransform_;
	Model*			model_			= nullptr;
	uint32_t		textureHandle_	= 0u;
	Vector3			velocity_;

	Phase			phase_			= Phase::Approach;

	Player*			player_			= nullptr;
	GameScene*		gameScene_		= nullptr;
	bool			isDead_			= false;

	void			Phase_Approach();
	void			Phase_Leave();
	static void		(Enemy::* pPhase[])();
};