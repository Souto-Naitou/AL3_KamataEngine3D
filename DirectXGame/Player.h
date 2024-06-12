#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include "PlayerBullet.h"
#include <list>
#include <Sprite.h>
#include "ViewProjection.h"

class Player {

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 
	ViewProjection viewProjection_;

	// モデル
	Model* model_ = nullptr;
	Model* reticleModel = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	// 回転
	void Rotate();

	// 3Dレティクル用ワールド変換データ
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	void UpdateWorldTransform3DReticle();

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
	void	Update(const ViewProjection& _viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_viewProjection">ビュープロジェクション（参照渡し）</param>
	void	Draw(ViewProjection& _viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 攻撃
	/// </summary>
	void	Attack();

	Vector3 GetWorldPosition(const Matrix4x4& _m);
	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* _parent);
};