#include "Interpolation.h"
#include <PrimitiveDrawer.h>
#include <cassert>
#include <cmath>
#include <MathExtension/mathExtension.h>


Vector3 CALC__CatmullRom(
	const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, const Vector3& _p3, float _t)
{
	const float s = 0.5f;

	float t2 = _t * _t;
	float t3 = t2 * _t;

	Vector3 e3 = -_p0 + 3 * _p1 - 3 * _p2 + _p3;
	Vector3 e2 = 2 * _p0 - 5 * _p1 + 4 * _p2 - _p3;
	Vector3 e1 = -_p0 + _p2;
	Vector3 e0 = 2 * _p1;

	return s * (e3 * t3 + e2 * t2 + e1 * _t + e0);
}

Vector3 Interpolation::CatmullRom(const std::vector<Vector3>& _controlPoints, float _t)
{
	assert(_controlPoints.size() > 4 && "制御点は4点以上必要です");

	// 区間数は制御点の数-1
	size_t division = _controlPoints.size() - 1;
	// 1区間の長さ (全体を1.0fとした場合)
	float areaWidth = 1.0f / division;

	// 区間内の始点を0.0f, 終点を1.0fとしたときの現在位置
	float t_2 = std::fmod(_t, areaWidth) * division;
	// 下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = clamp(t_2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(_t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = min(index, division);

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	if (index == 0)
		index0 = index1;
	if (index3 >= _controlPoints.size())
		index3 = index2;

	// 4点の座標
	const Vector3& p0 = _controlPoints[index0];
	const Vector3& p1 = _controlPoints[index1];
	const Vector3& p2 = _controlPoints[index2];
	const Vector3& p3 = _controlPoints[index3];

	// 4点を指定してCatmull-Rom補間
	return CALC__CatmullRom(p0, p1, p2, p3, t_2);
}

void Interpolation::DrawCatmullRom(const std::vector<Vector3>& _controlPoints, const ViewProjection& _viewprojection, const Vector4& _color)
{
	// 描画用の頂点リスト
	std::vector<Vector3> pointsDrawing;

	for (size_t i = 0; i < kSegmentCount_CatmullRom; i++)
	{
		float t = static_cast<float>(i) / kSegmentCount_CatmullRom;
		Vector3 pos = CatmullRom(_controlPoints, t);
		pointsDrawing.push_back(pos);
	}
	for (size_t i = 0; i < pointsDrawing.size() - 1; i++)
	{
		PrimitiveDrawer::GetInstance()->SetViewProjection(&_viewprojection);
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing[i], pointsDrawing[i + 1], _color);
	}
}