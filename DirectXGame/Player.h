#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>

class Player {

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <param name="_textureHandle">テクスチャハンドル</param>
	void	Initialize(Model* _model, uint32_t _textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void	Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_viewProjection">ビュープロジェクション（参照渡し）</param>
	void	Draw(ViewProjection& _viewProjection);

};