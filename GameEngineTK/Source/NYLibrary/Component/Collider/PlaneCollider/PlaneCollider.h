#pragma once
#include "../Collider.h"

namespace NYLibrary
{
	class ObjectData;

	//����
	class PlaneCollider : public Collider, public Plane
	{
	public:
		//�R���X�g���N�^
		PlaneCollider(ObjectData* objectData);
		virtual ~PlaneCollider() = default;
		//������
		void Initialize();
		//�X�V
		void Update();
		//���ƎO�p�`�|���S���̓����蔻��
		void Collision(PlaneCollider* segment) {}
		//�O�p�`�|���S���Ƃ̓����蔻��
		void Collision(SegmentCollider* segmentCollider);
		//��_��ݒ肷��
		void SetInter(const D3DXVECTOR3& inter) { this->inter = inter; }
		//����������_���擾
		const D3DXVECTOR3& GetInter() { return inter; }

	private:
		//����������_
		D3DXVECTOR3 inter;

	};
}
