#pragma once
#include "../Collider.h"
#include "../../Element.h"


namespace NYLibrary
{
	/// <summary>
	/// �O�p�`�|���S��
	/// </summary>
	class TrianglePolygonListCollider : public Collider
	{
	public:
		//�R���X�g���N�^
		TrianglePolygonListCollider(std::string tag, Element* matrixObject);
		//�O�p�`�|���S�����擾����
		std::vector<Triangle>& GetTriangleList() { return triangleList; }
		//����������
		void Initialize();
		//�X�V����
		void Update();
		//��_��ݒ肷��
		void SetInter(const D3DXVECTOR3& inter) { this->inter = inter; }
		//���������O�p�`��ݒ肷��
		void SetCollisionTriangle(Triangle triangle) { collisionTriangle = triangle; }
		//���ƎO�p�`�|���S���̓����蔻��
		void Collision(SegmentCollider* segment);
		//�O�p�`�|���S���Ƃ̓����蔻��
		void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider) {}
		//���������O�p�`���擾
		Triangle GetCollisionTriangle() { return collisionTriangle; }
		//����������_���擾
		D3DXVECTOR3 GetInter() { return inter; }
	private:
		//�O�p�`�|���S��
		std::vector<Triangle> triangleList;
		//���������O�p�`
		Triangle collisionTriangle;
		//����������_
		D3DXVECTOR3 inter;
	};
}