#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3& operator+=(const Vector3& _rv);
	Vector3 operator-() const;
	Vector3 operator-(const Vector3& _v);
	Vector3 operator+(const Vector3& _v);
	Vector3 operator*(float _f) const;
	Vector3 operator*(const float& _f);
};

Vector3 operator*(const float _f, const Vector3& _v);