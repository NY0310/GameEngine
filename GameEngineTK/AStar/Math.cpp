#include "Math.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

/// <summary>
/// 2点間の距離
/// </summary>
/// <param name="x1">点1のx座標</param>
/// <param name="y1">点1のy座標</param>
/// <param name="x2">点2のx座標</param>
/// <param name="y2">点2のy座標</param>
int Math::Distance(int x1, int y1, int x2, int y2)
{
	//Xの座標の差
	int differenceX = x1 - x2;
	//Yの座標の差
	int differenceY = y1 - y2;
	//2つの値とも0
	if (Both0(differenceX, differenceY))
		return 0;
	//斜辺を返す
	return PythagoreanTheorem(differenceX, differenceY);
}

/// <summary>
/// ピタゴラスの定理
/// </summary>
/// <param name="a">高さ</param>
/// <param name="b">底辺</param>
/// <returns></returns>
int Math::PythagoreanTheorem(int a, int b)
{
	return static_cast<int>(sqrt((a * a) + (b * b)));
}

/// <summary>
/// 2つの値とも0
/// </summary>
/// <param name="num1">1つめの値</param>
/// <param name="num2">2つめの値</param>
/// <returns></returns>
bool Math::Both0(int num1, int num2)
{
	if(num1 == 0 && num2 == 0)
		return true;
	return false;
}






D3DXVECTOR4 Math::VectorToD3DXVECTOR4(DirectX::SimpleMath::Vector4 vector4)
{
	float x = vector4.x;
	float y = vector4.y;
	float z = vector4.z;
	float w = vector4.w;

	D3DXVECTOR4 d3dxvector4 = D3DXVECTOR4(x, y, z, w);
	return d3dxvector4;

}




D3DXVECTOR3 Math::VectorToD3DXVECTOR3(DirectX::SimpleMath::Vector3 vector3)
{
	float x = vector3.x;
	float y = vector3.y;
	float z = vector3.z;
	D3DXVECTOR3 d3dxvector3 = D3DXVECTOR3(x, y, z);
	return d3dxvector3;

}


D3DXVECTOR2 Math::VectorToD3DXVECTOR2(DirectX::SimpleMath::Vector2 vector2)
{
	float x = vector2.x;
	float y = vector2.y;

	D3DXVECTOR2 d3dxvector2 = D3DXVECTOR2(x, y);
	return d3dxvector2;

}

Vector4 Math::D3DXVECTOR4ToVector(D3DXVECTOR4 d3dxvector4)
{
	float x = d3dxvector4.x;
	float y = d3dxvector4.y;
	float z = d3dxvector4.z;
	float w = d3dxvector4.w;

	Vector4 vector4 = Vector4(x, y, z, w);
	return vector4;
}


Vector3 Math::D3DXVECTOR3ToVector(D3DXVECTOR3 d3dxvector3)
{
	float x = d3dxvector3.x;
	float y = d3dxvector3.y;
	float z = d3dxvector3.z;
	Vector3 vector3 = Vector3(x, y, z);
	return vector3;
}


Vector2 Math::D3DXVECTOR2ToVector(D3DXVECTOR2 d3dxvector2)
{
	float x = d3dxvector2.x;
	float y = d3dxvector2.y;
	Vector2 vector2 = Vector2(x, y);
	return vector2;
}


Matrix Math::D3DXMATRIXToMatrix(D3DXMATRIX d3dxmatrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	Matrix matrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			matrix.m[i][j] = d3dxmatrix.m[i][j];
		}
	}


	return matrix;
}

D3DXQUATERNION Math::QuaterniontoD3DXQUATERNION(Quaternion quaternion)
{
	D3DXQUATERNION q;
	q.x = quaternion.x;
	q.y = quaternion.y;
	q.z = quaternion.z;
	return q;
}

Quaternion Math::D3DXQUATERNIONtoQuaternion(D3DXQUATERNION d3dxquaternion)
{
	Quaternion q;
	q.x = d3dxquaternion.x;
	q.y = d3dxquaternion.y;
	q.z = d3dxquaternion.z;
	return q;
}

D3DXMATRIX Math::MatrixToD3DXMATRIX(DirectX::SimpleMath::Matrix matrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	D3DXMATRIX d3dxmatrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			d3dxmatrix.m[i][j] = matrix.m[i][j];
		}
	}

	return d3dxmatrix;
}
