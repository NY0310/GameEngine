#pragma once
#include <d3d11_1.h>
#include <d3dx10.h>
#include <SimpleMath.h>

namespace NYLibrary
{
	class Math
	{
	public:
		//2�_�Ԃ̋���
		static int Distance(int x1, int y1, int x2, int y2);
		//�s�^�S���X�̒藝
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

		//Uv���W�n���琳�K�f�o�C�X���W�n�ɕϊ�����
		static const D3DXVECTOR3& ChangeRegularDevice(const D3DXVECTOR3& position);
		static const D3DXVECTOR2& ChangeRegularDevice(const D3DXVECTOR2& position);
		//Uv���W�n���琳�K�f�o�C�X���W�n�ɕϊ�����
		static const DirectX::SimpleMath::Vector3& ChangeRegularDevice(const DirectX::SimpleMath::Vector3& position);
		static const DirectX::SimpleMath::Vector2& ChangeRegularDevice(const DirectX::SimpleMath::Vector2& position);
		//�s���float4�̐�
		static const D3DXVECTOR4 & MatrixTimes(const D3DXMATRIX & matrix, const D3DXVECTOR4 & vector);
		//���`���
		static float Lerp(const float start, const float end, const float now)
		{
			return  start * (1.0f - now) + end * now; 
		}
		//���`���
		static D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const float time)
		{
			return  start * (1.0f - time) + end * time;
		}
		//���`���(�i�s�x�Z�o)
		static float ReverseLerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR3& now)
		{
			D3DXVECTOR3 seVec,snVec;//�n�_����I�_�̃x�N�g���@�A�n�_���猻�ݒn�ւ̃x�N�g��
			float projectionLength;//�ˉe�̒���
			float seLength;//�n�_����I�_�̒���
			D3DXVec3Normalize(&seVec,&(end - start));//�n�_����I�_�̐��K���x�N�g��
			snVec = now - start;
			projectionLength = D3DXVec3Dot(&seVec, &snVec);
			seLength = D3DXVec3Length(&seVec);
			return projectionLength / seLength;//�S�̂̒����Ǝˉe�̒��������݂̐i�s�x
		}
		//���`���(�i�s�x�Z�o)
		static float ReverseLerp(float start, float end, float now)
		{
			float seLength, snLength;//�n�_����I�_�̒����A�n�_���猻�݂̒���
			seLength = end - start;
			snLength = now - start;
			return snLength / seLength;
		}

		//��̒l���r���đ傫���ق���Ԃ�
		static float ComparisonBig(float A, float B) {return  (A > B) ? A : B; }
		//��̒l���r���đ傫���ق���Ԃ�
		static float ComparisonBig(const D3DXVECTOR3& vec3) {
			float max = ComparisonBig(vec3.x, vec3.y);
			return ComparisonBig(max, vec3.z);
		}

		//�񐔂��`�F�b�N���񐔂Ȃ�0�ɂ���
		static bool ValidateNan(float& A);
		//�񐔂��`�F�b�N���񐔂Ȃ�0�ɂ���
		static bool ValidateNan(D3DXVECTOR2& A);
		//�񐔂��`�F�b�N���񐔂Ȃ�0�ɂ���
		static bool ValidateNan(D3DXVECTOR4& A);

		//���[���h�s�񂩂�ړ������𒊏o����
		D3DXVECTOR3 ExtractionPosition(const D3DXMATRIX& worldMatrix) { return D3DXVECTOR3(worldMatrix._41, worldMatrix._42, worldMatrix._43); }
		//���[���h�s�񂩂��]�����𒊏o����
		//

		//����1������2�̒l�̃v���X����}�C�i�X�܂ł̊ԂŃN�����v����
		static void ClampAbsolute(D3DXVECTOR2& data, float maxAbsolute) {
			ClampAbsolute(data.x, maxAbsolute);
			ClampAbsolute(data.y, maxAbsolute);

		}


		//����1������2�̒l�̃v���X����}�C�i�X�܂ł̊ԂŃN�����v����
		static void ClampAbsolute(D3DXVECTOR2& data,const D3DXVECTOR2& maxAbsolute) {
			ClampAbsolute(data.x, maxAbsolute.x);
			ClampAbsolute(data.y, maxAbsolute.y);
		}


	

		//����1������2�̒l�̃v���X����}�C�i�X�܂ł̊ԂŃN�����v����
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

		//�ő�ƍŏ��͈̔͂ŃN�����v����
		static int Clamp(int maxNum, int minNum, int data)
		{
			return max(min(maxNum, data), minNum);
		}

		//�ő�ƍŏ��͈̔͂ŃN�����v����
		static D3DXVECTOR2 Clamp(int maxNum, int minNum, D3DXVECTOR2& data)
		{
			data.x = max(min(maxNum, data.x), minNum);
			data.y = max(min(maxNum, data.y), minNum);
			return data;
		}


		//�ő�ƍŏ��͈̔͂ŃN�����v����
		static D3DXVECTOR4 Clamp(int maxNum, int minNum, D3DXVECTOR4 data)
		{
			data.x =  max(min(maxNum, data.x), minNum);
			data.y = max(min(maxNum, data.y), minNum);
			data.z = max(min(maxNum, data.z), minNum);
			data.w = max(min(maxNum, data.w), minNum);
			return data;
		}

		//���R����Ԃ�
		static int NaturalNumber(int number)
		{
			(number < 0) ? number * -1 : number;
		}

		//���R����Ԃ�
		static float NaturalNumber(float number)
		{
			(number < 0) ? number * -1.0f : number;
		}

		//���R����Ԃ�
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


		//2�̒l�Ƃ�0
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