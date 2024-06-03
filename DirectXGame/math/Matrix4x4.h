#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 {
	float m[4][4];
	Matrix4x4 operator*=(const Matrix4x4& _lm)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m[i][j] *= _lm.m[i][j];
			}
		}
		return *this;
	}

};

//Matrix4x4 operator+=(const Matrix4x4& _lm, const Matrix4x4& _rm)
//{
//	Matrix4x4 result{};
//	for (int i = 0; i < 4; i++) 
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			result.m[i][j] = _lm.m[i][j] + _rm.m[i][j];
//		}
//	}
//
//	return result;
//}



