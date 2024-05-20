#pragma once

#include <Model.h>
#include <WorldTransform.h>

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet
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

	/// <summary>
	/// デスフラグを取得
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

private:
	static const int32_t kLifeTime = 60 * 5;		// 寿命<frm>

	WorldTransform	worldTransform_;				// ワールド変換
	Model* model_ = nullptr;						// モデル
	uint32_t		textureHandle_ = 0u;			// テクスチャハンドル
	int32_t			deathTimer_ = kLifeTime;		// デスタイマー
	bool			isDead_ = false;				// デスフラグ
	Vector3			velocity_;

};