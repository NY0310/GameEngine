#pragma once
#include <vector>
#include <D3DX10math.h>
#include "../Component.h"



namespace NYLibrary
{
	class Collider : public Component
	{
	public:
		//コンストラクタ
		Collider();
		//デストラクタ
		~Collider() = default;
		////更新処理
		//void Update(Element* element) = 0;
	private:
		std::vector<Collider*> collisitonCollider;
	};



	/// <summary>
	/// 球体
	/// </summary>
	class SphereCollider : public Collider
	{
	public:
		//コンストラクタ
		SphereCollider(const D3DXVECTOR3& center, const float& radius) :center(center), radius(radius){}
		//中心座標
		D3DXVECTOR3 center;
		//半径
		float radius;
	};

	class InkSphereCollider : public SphereCollider
	{
	public:
		InkSphereCollider(const D3DXVECTOR3& center, const float& radius, const D3DXVECTOR4 color) : SphereCollider(center, radius), color(color) {}
		D3DXVECTOR4 color;
	};


	//線分
	class SegmentCollider : public Collider
	{
	public:
		SegmentCollider(const D3DXVECTOR3& start, const D3DXVECTOR3& end) :start(start), end(end) {}
		//始点座標
		D3DXVECTOR3 start;
		//終了地点
		D3DXVECTOR3 end;
	};

	class InkSegmentCollider : public SegmentCollider
	{
	public:
		InkSegmentCollider(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR4& color) : SegmentCollider(start, end), color(color) {}
		D3DXVECTOR4 color;
		int index;
	};


	//三角形
	class TriangleCollider : public Collider
	{
	public:
		TriangleCollider(const D3DXVECTOR3& P0, const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, const D3DXVECTOR3& Normal, const D3DXVECTOR2& Uv0, const D3DXVECTOR2& Uv1, const D3DXVECTOR2& Uv2) :
			P0(P0), P1(P1), P2(P2), Normal(Normal), Uv0(Uv0), Uv1(Uv1), Uv2(Uv2) {}
		//頂点の座標
		D3DXVECTOR3 P0;
		D3DXVECTOR3 P1;
		D3DXVECTOR3 P2;
		//法線ベクトル
		D3DXVECTOR3 Normal;
		//UV
		D3DXVECTOR2 Uv0;
		D3DXVECTOR2 Uv1;
		D3DXVECTOR2 Uv2;
	};


	class TrianglePolygonListCollider : public Collider
	{
	private:
		TrianglePolygonListCollider();
		std::vector<TriangleCollider*> triangleList;
		TriangleCollider triangle;
	};

};