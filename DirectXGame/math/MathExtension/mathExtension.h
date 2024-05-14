#pragma once
#include <Vector3.h>
#include <Matrix4x4.h>

float cotanf(float _x);
float clamp(
	float _x, 
	float _min, 
	float _max
);
int clamp(
	int _x,
	int _min,
	int _max
);
Vector3 TransformNormal(const Vector3& _v, const Matrix4x4& _m);