#pragma once
#include <vector>
#include <Vector3.h>
#include <Vector4.h>
#include <ViewProjection.h>

class Interpolation
{
public:
	static Vector3 CatmullRom(const std::vector<Vector3>& _controlPoints, float _t);

	void DrawCatmullRom(const std::vector<Vector3>& _controlPoints, const ViewProjection& _viewprojection, const Vector4& _color);

private:
	static const size_t kSegmentCount_CatmullRom = 100u;
};