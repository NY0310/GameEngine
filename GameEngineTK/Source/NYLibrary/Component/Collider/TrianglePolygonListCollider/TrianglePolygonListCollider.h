#pragma once
#include "../Collider.h"

namespace NYLibrary
{
	/// <summary>
	/// �O�p�`�|���S��
	/// </summary>
	class TrianglePolygonListCollider : public Collider
	{
	public:
		TrianglePolygonListCollider() = default;
		//TrianglePolygonListCollider(const std::string& tag,const std::vector<TriangleCollider*>& triangleCollider):Collider(tag,nullptr){}
		//�O�p�`�|���S�����擾����
		const std::vector<Triangle*>& GetTriangleList() { return triangleList; }
		//�X�V����
		void Update() {}
		//��_��ݒ肷��
		void SetInter(const D3DXVECTOR3& inter) { this->inter = inter; }
		//���ƎO�p�`�|���S���̓����蔻��
		void Collision(SegmentCollider* segment);
		//���������O�p�`�̐ݒ�
		void SetCollisionTriangle(Triangle* triangle) { collisionTriangle = triangle; }
	private:
		//�O�p�`�|���S��
		std::vector<Triangle*> triangleList;
		//���������O�p�`
		Triangle* collisionTriangle;
		//����������_
		D3DXVECTOR3 inter;
	};
}