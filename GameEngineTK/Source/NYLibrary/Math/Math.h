#pragma once
#include <d3d11_1.h>
#include <d3dx10.h>
#include <SimpleMath.h>

namespace NYLibrary
{
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
		//行列とfloat4の積
		static const D3DXVECTOR4 & MatrixTimes(const D3DXMATRIX & matrix, const D3DXVECTOR4 & vector);
		//線形補間
		static float Lerp(const float start, const float end, const float now)
		{
			return  start * (1.0f - now) + end * now; 
		}
		//線形補間
		static D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const float time)
		{
			return  start * (1.0f - time) + end * time;
		}
		//線形補間(進行度算出)
		static float ReverseLerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR3& now)
		{
			D3DXVECTOR3 seVec,snVec;//始点から終点のベクトル　、始点から現在地へのベクトル
			float projectionLength;//射影の長さ
			float seLength;//始点から終点の長さ
			D3DXVec3Normalize(&seVec,&(end - start));//始点から終点の正規化ベクトル
			snVec = now - start;
			projectionLength = D3DXVec3Dot(&seVec, &snVec);
			seLength = D3DXVec3Length(&seVec);
			return projectionLength / seLength;//全体の長さと射影の長さが現在の進行度
		}
		//線形補間(進行度算出)
		static float ReverseLerp(float start, float end, float now)
		{
			float seLength, snLength;//始点から終点の長さ、始点から現在の長さ
			seLength = end - start;
			snLength = now - start;
			return snLength / seLength;
		}

		//二つの値を比較して大きいほうを返す
		static float ComparisonBig(float A, float B) {return  (A > B) ? A : B; }
		//二つの値を比較して大きいほうを返す
		static float ComparisonBig(const D3DXVECTOR3& vec3) {
			float max = ComparisonBig(vec3.x, vec3.y);
			return ComparisonBig(max, vec3.z);
		}

		//非数かチェックし非数なら0にする
		static bool ValidateNan(float& A);
		//非数かチェックし非数なら0にする
		static bool ValidateNan(D3DXVECTOR2& A);
		//非数かチェックし非数なら0にする
		static bool ValidateNan(D3DXVECTOR4& A);

		//ワールド行列から移動成分を抽出する
		D3DXVECTOR3 ExtractionPosition(const D3DXMATRIX& worldMatrix) { return D3DXVECTOR3(worldMatrix._41, worldMatrix._42, worldMatrix._43); }
		//ワールド行列から回転成分を抽出する
		//

		//引数1を引数2の値のプラスからマイナスまでの間でクランプする
		static void ClampAbsolute(D3DXVECTOR2& data, float maxAbsolute) {
			ClampAbsolute(data.x, maxAbsolute);
			ClampAbsolute(data.y, maxAbsolute);

		}


		//引数1を引数2の値のプラスからマイナスまでの間でクランプする
		static void ClampAbsolute(D3DXVECTOR2& data,const D3DXVECTOR2& maxAbsolute) {
			ClampAbsolute(data.x, maxAbsolute.x);
			ClampAbsolute(data.y, maxAbsolute.y);
		}


	

		//引数1を引数2の値のプラスからマイナスまでの間でクランプする
		static void ClampAbsolute(float& data, float max) {
			if (max <= data)
			{
				data = max;
			}
			else if (max * -1 >= data)
			{
				data = max * -1;
			}
		}

		//最大と最小の範囲でクランプする
		static int Clamp(int maxNum, int minNum, int data)
		{
			return max(min(maxNum, data), minNum);
		}

		//最大と最小の範囲でクランプする
		static D3DXVECTOR2 Clamp(int maxNum, int minNum, D3DXVECTOR2& data)
		{
			data.x = max(min(maxNum, data.x), minNum);
			data.y = max(min(maxNum, data.y), minNum);
			return data;
		}


		//最大と最小の範囲でクランプする
		static D3DXVECTOR4 Clamp(int maxNum, int minNum, D3DXVECTOR4 data)
		{
			data.x =  max(min(maxNum, data.x), minNum);
			data.y = max(min(maxNum, data.y), minNum);
			data.z = max(min(maxNum, data.z), minNum);
			data.w = max(min(maxNum, data.w), minNum);
			return data;
		}

		//自然数を返す
		static int NaturalNumber(int number)
		{
			(number < 0) ? number * -1 : number;
		}

		//自然数を返す
		static float NaturalNumber(float number)
		{
			(number < 0) ? number * -1.0f : number;
		}

		//自然数を返す
		static D3DXVECTOR4 NaturalNumber(const D3DXVECTOR4& number)
		{
			D3DXVECTOR4 output = number;
			if (number.x < 0)
				output.x = number.x * -1;
			if (number.y < 0)
				output.y = number.y* -1;
			if (number.z < 0)
				output.z = number.z * -1;
			if (number.w < 0)
				output.w = number.w * -1;
			return output;

		}	


		//2つの値とも0
		static bool Both0(int num1, int num2);

		//D3DXVECTOR3 * D3DXVECTOR3
		static D3DXVECTOR3 D3DXVec3Multiplication(const D3DXVECTOR3& A, const D3DXVECTOR3& B)
		{
			D3DXVECTOR3 output;
			output.x = A.x * B.x;
			output.x = A.y * B.y;
			output.x = A.z * B.z;
			return output;
		}
	};
};