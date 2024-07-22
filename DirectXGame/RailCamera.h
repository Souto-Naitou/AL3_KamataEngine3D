#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Interpolation.h"


class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 _translate, Vector3 _rotate);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& _viewProjection);

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_{};
	ViewProjection viewProjection_{};
	Vector3 translateSpeed_{};
	Vector3 rotateSpeed_{};
	Interpolation interpolation_{};

	// スプライン曲線制御点
	std::vector<Vector3> controlPoints_;
	float t = 0.0f;
};