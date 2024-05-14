#include "mathExtension.h"
#include <cmath>

float cotanf(float _x)
{
	return 1.0f / std::tanf(_x);
}

float clamp(float _x, float _min, float _max)
{
	float result = _x;
	if (_x < _min) result = _min;
	else if (_x > _max) result = _max;
	return result;
}

int clamp(int _x, int _min, int _max)
{
	int result = _x;
	if (_x < _min) result = _min;
	else if (_x > _max) result = _max;
	return result;
}

Vector3 TransformNormal(const Vector3& _v, const Matrix4x4& _m)
{
	Vector3 result
	{
		_v.x * _m.m[0][0] + _v.y * _m.m[1][0] + _v.z * _m.m[2][0] + 1.0f * _m.m[3][0],
		_v.x * _m.m[0][1] + _v.y * _m.m[1][1] + _v.z * _m.m[2][1] + 1.0f * _m.m[3][1],
		_v.x * _m.m[0][2] + _v.y * _m.m[1][2] + _v.z * _m.m[2][2] + 1.0f * _m.m[3][2]
	};

	return result;
}
