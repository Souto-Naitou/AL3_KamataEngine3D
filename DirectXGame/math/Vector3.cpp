#include "Vector3.h"

Vector3& Vector3::operator+=(const Vector3& _rv)
{
	x += _rv.x;
	y += _rv.y;
	z += _rv.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& _v)
{
	Vector3 result{};
	result.x = x - _v.x;
	result.y = y - _v.y;
	result.z = z - _v.z;
	return result;
}

Vector3 Vector3::operator+(const Vector3& _v)
{
	Vector3 result{};
	result.x = x + _v.x;
	result.y = y + _v.y;
	result.z = z + _v.z;
	return result;
}
