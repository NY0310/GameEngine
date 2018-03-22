#pragma once
#include "../Collider.h"


namespace NYLibrary
{
	class ObjectData;

	//����
	class SegmentCollider : public Collider, public Segment
	{
	public:
		//�R���X�g���N�^
		SegmentCollider(const std::string& tag, ObjectData* matrixObject);
		//������
		void Initialize();
		//�X�V
		void Update();
		//���ƎO�p�`�|���S���̓����蔻��
		void Collision(SegmentCollider* segment) {}
		//�O�p�`�|���S���Ƃ̓����蔻��
		void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider);
	};
}
