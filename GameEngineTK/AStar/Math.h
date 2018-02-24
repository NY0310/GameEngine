#pragma once
#include <d3d11_1.h>
#include <d3dx10.h>
#include <SimpleMath.h>


class Math
{
public:
	//2点間の距離
	static int Distance(int x1, int y1, int x2, int y2);
	//ピタゴラスの定理
	static int PythagoreanTheorem(int a, int b);
	static D3DXVECTOR4 VectorToD3DXVECTOR4(DirectX::SimpleMath::Vector4 vector4);

	static D3DXMATRIX MatrixToD3DXMATRIX(DirectX::SimpleMath::Matrix matrix);
	static D3DXVECTOR3 VectorToD3DXVECTOR3(DirectX::SimpleMath::Vector3 vector3);
	static D3DXVECTOR2 VectorToD3DXVECTOR2(DirectX::SimpleMath::Vector2 vector2);
	static DirectX::SimpleMath::Vector4 D3DXVECTOR4ToVector(D3DXVECTOR4 d3dxvector4);
	static DirectX::SimpleMath::Vector3 D3DXVECTOR3ToVector(D3DXVECTOR3 d3dxvector3);

	static DirectX::SimpleMath::Vector2 D3DXVECTOR2ToVector(D3DXVECTOR2 d3dxvector2);

	static DirectX::SimpleMath::Matrix D3DXMATRIXToMatrix(D3DXMATRIX d3dxmatrix);

	static D3DXQUATERNION QuaterniontoD3DXQUATERNION(DirectX::SimpleMath::Quaternion quaternion);
	static DirectX::SimpleMath::Quaternion D3DXQUATERNIONtoQuaternion(D3DXQUATERNION d3dxquaternion);
	static float Clamp(float x, float low, float hight) {
		if (x < low)
			return low;
		if (x > hight)
			return hight;
		return x;
	}
	static float RegularDeviceClamp(float x) {
		float low = -1.0f;
		float hight = 1.0f;
		if (x < low)
			return low;
		if (x > hight)
			return hight;
		return x;
	}

	//Uv座標系から正規デバイス座標系に変換する
	static const D3DXVECTOR3& ChangeRegularDevice(const D3DXVECTOR3& position);
	static const D3DXVECTOR2& ChangeRegularDevice(const D3DXVECTOR2& position);
	//Uv座標系から正規デバイス座標系に変換する
	static const DirectX::SimpleMath::Vector3& ChangeRegularDevice(const DirectX::SimpleMath::Vector3& position);
	static const DirectX::SimpleMath::Vector2& ChangeRegularDevice(const DirectX::SimpleMath::Vector2& position);

private:
	//2つの値とも0
	static bool Both0(int num1, int num2);
};