#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include <list>
#include <Sprite.h>
#include "PlayerBullet.h"
#include "ViewProjection.h"
#include "Collider.h"

class Enemy;

class Player : public Collider
{
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビューポート
	Matrix4x4 matViewport;
	// モデル
	Model* model_ = nullptr;
	Model* reticleModel = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t textureReticle_ = 0u;
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
	std::vector<Sprite*>sprite2DReticleLockon_;
	// マウス座標(クライアントエリア座標)
	POINT mousePosition;

	// 
	Vector2 spritePosition;

	std::list<Enemy*>* enemiesList_;

	XINPUT_STATE joyState_;

	//std::vector<Enemy*> lockonEnemyList;
	std::vector<std::pair<Enemy*, Sprite*>> lockonEnemyPairList_;

	// 弾の速度
	const float kbulletSpeed = 5.0f;

	// 衝突属性
	const unsigned int collisionAttribute_ = 0b0001;
	const unsigned int collisionMask_ = 0b1100;


	void UpdateWorldTransform3DReticle(Vector3 _translation);
	void GetCursorPosition();
	void Update3DReticleWithCursor(const ViewProjection&);

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
	void	DrawUI();

	/// <summary>
	/// 攻撃
	/// </summary>
	void	Attack();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	Vector3 GetWorldPosition(const Matrix4x4& _m);
	Vector3 GetWorldPosition();
	std::vector<std::pair<Enemy*, Sprite*>>* GetLockonEnemyListPair() { return &lockonEnemyPairList_; }

	void SetEnemies(std::list<Enemy*>* _enemiesList) { enemiesList_ = _enemiesList; }

	// 衝突時に呼ばれる関数
	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* _parent);
};