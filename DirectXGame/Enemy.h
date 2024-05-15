#pragma once

#include <WorldTransform.h>
#include <Model.h>

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
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

private:

	WorldTransform	worldTransform_;
	Model*			model_			= nullptr;
	uint32_t		textureHandle_	= 0u;
	Vector3			velocity_;
};