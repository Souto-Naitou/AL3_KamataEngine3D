#pragma once

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
private:
	// 衝突半径
	float radius_ = 1.0f;
public:
	// 半径を取得
	float GetRadius() { return radius_; }
	// 半径を設定
	void SetRadius(float _radius) { radius_ = _radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision() {};
	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
};