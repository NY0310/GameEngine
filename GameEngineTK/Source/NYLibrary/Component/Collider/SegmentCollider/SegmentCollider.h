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
		SegmentCollider(ObjectData* objectData);
		SegmentCollider() = delete;
		virtual ~SegmentCollider() = default;
		//������
		void Initialize();
		//�X�V
		void Update();
		//
		void CalcSegmentPosition();
		//���ƎO�p�`�|���S���̓����蔻��
		void Collision(SegmentCollider* segment) {}
		// ��]��̐����̍��W�����߂�
		D3DXVECTOR3 CalcSegmentPosition(const D3DXVECTOR3 & localposition, D3DXMATRIX & matrix);
		//�O�p�`�|���S���Ƃ̓����蔻��
		void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider);
		//�O�p�`�|���S���Ƃ̓����蔻��
		void Collision(PlaneCollider* planeCollider);


		//�J�n�_�̃��[���h�s����擾����
		const D3DXMATRIX& GetStartWorldMatrix() {
			return startWorldMatrix;
		}
		//�I�_�̃��[���h�s����擾����
		const D3DXMATRIX& GetEndWorldMatrix() {
			return endWorldMatrix;
		}
	private:
		D3DXMATRIX startWorldMatrix;
		D3DXMATRIX endWorldMatrix;
	};
}
