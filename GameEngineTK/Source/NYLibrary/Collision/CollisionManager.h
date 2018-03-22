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
	private:
		//�����蔻�����邩���肷��
		bool CheckCallCollision(Collider* collider, Collider* collider_)const;
		//���N���X�̃C���X�^���X
		static std::unique_ptr<CollisionManager> collisionManager;
		//�R���C�_�[���X�g(�^�O�A�R���C�_�[)
		std::vector<Collider*> colliderList;
		//bool Collision(const Sphere & sphereA, const Sphere & sphereB);
		//void Collision(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b, const D3DXVECTOR3 & _p2, Triangle* c);
		//bool Collision(const Sphere & _sphere, const Triangle & _triangle, D3DXVECTOR3 * _inter);
		//bool Collision(const SegmentCollider& segment, const TrianglePolygonListCollider& _triangle, D3DXVECTOR3 *_inter);
		//�_�ƎO�p�`�̍ŋߐړ_���v�Z
		//void Collision(const D3DXVECTOR3& _point, const TriangleCollider& _triangle, D3DXVECTOR3*);
		////���ƎO�p�`�|���S���̓����蔻��
		//bool Collision(SegmentCollider* segment, TrianglePolygonListCollider* _triangle);
		//���ƎO�p�`�̓����蔻��
	//	bool Collision(const SegmentCollider* segment, const Triangle* triangle, D3DXVECTOR3 _inter);

		//bool Collision(const  D3DXVECTOR3& _point, const TriangleCollider& _triangle);
		//void Collision(const D3DXVECTOR3& _point, const TriangleCollider& _triangle, D3DXVECTOR3* _closest);


		//���̃R���C�_�[��
		bool TypeCheckSegmentCollider(Collider* collider, Collider * collider_);
		//�O�p�`�|���S���̃R���C�_�[��
		bool TypeCheckTrianglePolygonListCollider(Collider* collider, Collider * collider_);


	};
};
