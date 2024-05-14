#include "Vector3.h"

Vector3& Vector3::operator+=(const Vector3& _rv)
{
	x += _rv.x;
	y += _rv.y;
	z += _rv.z;
	return *this;
}
