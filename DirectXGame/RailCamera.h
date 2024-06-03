#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"


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

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_{};
	ViewProjection viewProjection_{};
	Vector3 translateSpeed_{};
	Vector3 rotateSpeed_{};
};