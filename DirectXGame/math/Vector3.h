#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};
//
//Vector3 operator+=(const Vector3& _lv, const Vector3& _rv)
//{ 
//	Vector3 result;
//
//	result.x = _lv.x + _rv.x;
//	result.y = _lv.y + _rv.y;
//	result.z = _lv.z + _rv.z;
//
//	return result;
//}