#pragma once
#include <vector>
#include <D3DX10math.h>
#include "../Component.h"



namespace NYLibrary
{
	class Collider : public Component
	{
	public:
		//�R���X�g���N�^
		Collider();
		//�f�X�g���N�^
		~Collider() = default;
		////�X�V����
		//void Update(Element* element) = 0;
	private:
		std::vector<Collider*> collisitonCollider;
	};



	/// <summary>
	/// ����
	/// </summary>
	class SphereCollider : public Collider
	{
	public:
		//�R���X�g���N�^
		SphereCollider(const D3DXVECTOR3& center, const float& radius) :center(center), radius(radius){}
		//���S���W
		D3DXVECTOR3 center;
		//���a
		float radius;
	};

	class InkSphereCollider : public SphereCollider
	{
	public:
		InkSphereCollider(const D3DXVECTOR3& center, const float& radius, const D3DXVECTOR4 color) : SphereCollider(center, radius), color(color) {}
		D3DXVECTOR4 color;
	};


	//����
	class SegmentCollider : public Collider
	{
	public:
		SegmentCollider(const D3DXVECTOR3& start, const D3DXVECTOR3& end) :start(start), end(end) {}
		//�n�_���W
		D3DXVECTOR3 start;
		//�I���n�_
		D3DXVECTOR3 end;
	};

	class InkSegmentCollider : public SegmentCollider
	{
	public:
		InkSegmentCollider(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR4& color) : SegmentCollider(start, end), color(color) {}
		D3DXVECTOR4 color;
		int index;
	};


	//�O�p�`
	class TriangleCollider : public Collider
	{
	public:
		TriangleCollider(const D3DXVECTOR3& P0, const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, const D3DXVECTOR3& Normal, const D3DXVECTOR2& Uv0, const D3DXVECTOR2& Uv1, const D3DXVECTOR2& Uv2) :
			P0(P0), P1(P1), P2(P2), Normal(Normal), Uv0(Uv0), Uv1(Uv1), Uv2(Uv2) {}
		//���_�̍��W
		D3DXVECTOR3 P0;
		D3DXVECTOR3 P1;
		D3DXVECTOR3 P2;
		//�@���x�N�g��
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