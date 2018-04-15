#pragma once
#include <memory>
#include <vector>
#include <algorithm>

namespace NYLibrary
{
	class Collider;
	class SphereCollider;
	class InkSphereCollider;
	class SegmentCollider;
	class InkSegmentCollider;
	class TriangleCollider;
	class TrianglePolygonListCollider;
	class PlaneCollider;

	class CollisionManager
	{
	public:
		//�C���X�^���X���擾����
		static CollisionManager* GetInstance();
		//�X�V����
		void Update();
		void AllTypeCheckAndCollisition(Collider * collider, Collider * _collider);
		//�R���C�_�[��ǉ�����
		void AddCollider(Collider* collider) {
			colliderList.emplace_back(collider);
		}
		//�����蔻����s���C���^�[�o����ݒ�
		void SetCollisitonIntervalFrame(int frame) { collisitonIntervalFrame = frame; frameCnt = 0; }

		//�R���C�_�[�̍폜
		void EraseCollider(Collider* collider);
	private:
		//�R���X�g���N�^
		CollisionManager() { collisitonIntervalFrame = 0; }
		//�����蔻�����邩���肷��
		bool CheckCallCollision(Collider* collider, Collider* collider_)const;
		//���N���X�̃C���X�^���X
		static std::unique_ptr<CollisionManager> collisionManager;
		//�R���C�_�[���X�g(�^�O�A�R���C�_�[)
		std::vector<Collider*> colliderList;
		//���̃R���C�_�[��
		bool TypeCheckSegmentCollider(Collider* collider, Collider * collider_);
		//�O�p�`�|���S���̃R���C�_�[��
		bool TypeCheckTrianglePolygonListCollider(Collider* collider, Collider * collider_);

		bool TypeCheckPlaneCollider(Collider * collider, Collider * collider_);

		//�����蔻����s��
		int collisitonIntervalFrame;
		//�t���[���J�E���g
		int frameCnt;
	};
};
