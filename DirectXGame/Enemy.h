#pragma once

#include <WorldTransform.h>
#include <Model.h>
#include "EnemyBullet.h"

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

	void	Fire();

private:

	WorldTransform	worldTransform_;
	Model*			model_			= nullptr;
	uint32_t		textureHandle_	= 0u;
	Vector3			velocity_;

	Phase			phase_			= Phase::Approach;

	// 弾
	std::list<EnemyBullet*> bullets_;


	void			Phase_Approach();
	void			Phase_Leave();
};