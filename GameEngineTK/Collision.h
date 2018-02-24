/***************************************************************************
*|
*|	�T�v�@�@�Փ˔��胉�C�u����
*|�@�쐬�ҁ@GS2 16 ���c�N��
*|�@�쐬���@2017//
*|___________________________________________________________________________
****************************************************************************/
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>


//��
class Sphere
{
public:
	//���S���W
	DirectX::SimpleMath::Vector3 Center;
	//���a
	float Radius;
	//�R���X�g���N�^
	Sphere() {
		//�f�t�H���g�̔w�i��1���[�g���ɂ���
		Radius = 1.0f;
	}
};

class InkSphere : public Sphere
{
public:
	DirectX::SimpleMath::Vector4 Color;
};


//����
class Segment
{
public:
	//�n�_���W
	DirectX::SimpleMath::Vector3 Start;
	//�I���n�_
	DirectX::SimpleMath::Vector3 End;
};

class InkSegment : public Segment
{
public:
	DirectX::SimpleMath::Vector4 Color;
	int index;
};

//�O�p�`
class Triangle
{
public:
	//���_�̍��W
	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;
	//�@���x�N�g��
	DirectX::SimpleMath::Vector3 Normal;
	//UV
	DirectX::SimpleMath::Vector2 Uv0;
	DirectX::SimpleMath::Vector2 Uv1;
	DirectX::SimpleMath::Vector2 Uv2;

private:
};


bool CheckSpere2Sphere(const Sphere & sphereA, const Sphere & sphereB);
void ComputeTriangle(const DirectX::SimpleMath::Vector3 & a, const DirectX::SimpleMath::Vector3 & b, const DirectX::SimpleMath::Vector3 & _p2, Triangle* c);

bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);
bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);
void ClosestPtPoint2Triangle(const DirectX::SimpleMath::Vector3& _point, const Triangle& _triangle, DirectX::SimpleMath::Vector3* _closest);
bool CheckPoint2Triangle(const  DirectX::SimpleMath::Vector3& _point, const Triangle& _triangle);
