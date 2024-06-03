#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include "PlayerBullet.h"
#include <list>

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

	// 弾
	std::list<PlayerBullet*> bullets_;

	// 回転
	void Rotate();

public:
	~Player();
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <param name="_textureHandle">テクスチャハンドル</param>
	void	Initialize(Model* _model, uint32_t _textureHandle, Vector3 _position);

	/// <summary>
	/// 更新
	/// </summary>
	void	Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_viewProjection">ビュープロジェクション（参照渡し）</param>
	void	Draw(ViewProjection& _viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void	Attack();

	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* _parent);
};