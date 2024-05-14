#pragma once

#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"

/// <summary>
/// 
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <param name="_position">初期座標</param>
	/// <param name="_velocity">速度</param>
	void Initialize(Model* _model, const Vector3& _position, const Vector3& _velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& _viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
};