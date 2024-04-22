#pragma once
#include <WorldTransform.h>
#include <Model.h>

class Player {

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	void	Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void	Update();

	/// <summary>
	/// 描画
	/// </summary>
	void	Draw();

};