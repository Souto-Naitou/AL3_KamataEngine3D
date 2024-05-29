#pragma once
#include <WorldTransform.h>
#include "ViewProjection.h"
#include <Model.h>

/// <summary>
/// 天球
/// </summary>
class Skydome
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* _model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& _viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};