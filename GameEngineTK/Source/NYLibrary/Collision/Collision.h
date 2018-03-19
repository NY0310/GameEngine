/***************************************************************************
*|
*|	概要　　衝突判定ライブラリ
*|　作成者　GS2 16 中田湧介
*|　作成日　2017//
*|___________________________________________________________________________
****************************************************************************/
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <d3dx9math.h>

namespace NYLibrary
{
	class SphereCollider;
	class InkSphereCollider;
	class SegmentCollider;
	class InkSegmentCollider;
	class TriangleCollider;
	class TrianglePolygonListCollider;



	//球
	class Sphere
	{
	public:
		//中心座標
		D3DXVECTOR3 center;
		//半径
		float radius;
	};

	class InkSphere : public Sphere
	{
	public:
		D3DXVECTOR4 color;
	};


	//線分
	class Segment
	{
	public:
		//始点座標
		D3DXVECTOR3 start;
		//終了地点
		D3DXVECTOR3 end;
	};

	class InkSegment : public Segment
	{
	public:
		D3DXVECTOR4 color;
		int index;
	};

	//三角形
	class Triangle
	{
	public:
		//頂点の座標
		D3DXVECTOR3 p0;
		D3DXVECTOR3 p1;
		D3DXVECTOR3 p2;
		//法線ベクトル
		D3DXVECTOR3 normal;
		//UV
		D3DXVECTOR2 uv0;
		D3DXVECTOR2 uv1;
		D3DXVECTOR2 uv2;

	private:
	};


	bool CheckSpere2Sphere(const Sphere & sphereA, const Sphere & sphereB);
	void ComputeTriangle(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b, const D3DXVECTOR3 & _p2, Triangle* c);

	bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _triangle, D3DXVECTOR3 * _inter);
	void ClosestPtPoint2Triangle(const D3DXVECTOR3& _point, const Triangle& _triangle, D3DXVECTOR3* _closest);
	bool CheckPoint2Triangle(const  D3DXVECTOR3& _point, const Triangle& _triangle);
	//bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, D3DXVECTOR3 *_inter);

	//線と全ての三角形ポリゴンの当たり判定	
	void CheckSegment2AllTriangle(SegmentCollider* segment, TrianglePolygonListCollider* trianglePolygonListCollider);
	//線と三角形ポリゴンの当たり判定	
	bool CheckSegment2Triangle(const SegmentCollider* segment, Triangle* triangle,D3DXVECTOR3* inter);
};