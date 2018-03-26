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
#include <d3dx9math.h>
#include "../Math/Math.h"

namespace NYLibrary
{
	class SphereCollider;
	class InkSphereCollider;
	class SegmentCollider;
	class InkSegmentCollider;
	class TriangleCollider;
	class TrianglePolygonListCollider;
	class PlaneCollider;


	//��
	class Sphere
	{
	public:
		//���S���W
		D3DXVECTOR3 center;
		//���a
		float radius;
	};

	class InkSphere : public Sphere
	{
	public:
		D3DXVECTOR4 color;
	};


	//����
	class Segment
	{
	public:
		//�n�_���W
		D3DXVECTOR3 start;
		//�I���n�_
		D3DXVECTOR3 end;
	};

	class InkSegment : public Segment
	{
	public:
		D3DXVECTOR4 color;
		int index;
	};

	//�O�p�`
	class Triangle
	{
	public:
		//���_�̍��W
		D3DXVECTOR3 p0;
		D3DXVECTOR3 p1;
		D3DXVECTOR3 p2;
		//�@���x�N�g��
		D3DXVECTOR3 normal;
		//UV
		D3DXVECTOR2 uv0;
		D3DXVECTOR2 uv1;
		D3DXVECTOR2 uv2;

	private:
	};


	//���ʂ̒�`
	class Plane {	
	public:
		float nX, nY, nZ, r;
	};


	bool CheckSpere2Sphere(const Sphere & sphereA, const Sphere & sphereB);
	void ComputeTriangle(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b, const D3DXVECTOR3 & _p2, Triangle* c);

	bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _triangle, D3DXVECTOR3 * _inter);
	void ClosestPtPoint2Triangle(const D3DXVECTOR3& _point, const Triangle& _triangle, D3DXVECTOR3* _closest);
	bool CheckPoint2Triangle(const  D3DXVECTOR3& _point, const Triangle& _triangle);
	//bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, D3DXVECTOR3 *_inter);

	//���ƑS�Ă̎O�p�`�|���S���̓����蔻��	
	void CheckSegment2AllTriangle(SegmentCollider* segment, TrianglePolygonListCollider* trianglePolygonListCollider);
	//���ƎO�p�`�|���S���̓����蔻��	
	bool CheckSegment2Triangle(const Segment* segment, Triangle* triangle,D3DXVECTOR3& inter);
	void IntersectPlane2Segment(SegmentCollider * inkSegmentColslider, PlaneCollider * planeCollider);
	bool IntersectPlane2Segment(D3DXVECTOR3 * out, D3DXVECTOR3 A, D3DXVECTOR3 B, Plane PL);
};