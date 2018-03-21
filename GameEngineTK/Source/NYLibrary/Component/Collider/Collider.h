#pragma once
#include <vector>
#include <D3DX9math.h>
#include "../Component.h"
#include "../../Object/MatrixObject/MatrixObject.h"
#include "../../Collision/Collision.h"
#include "../../Collision/CollisionManager.h"



namespace NYLibrary
{
	class ObjectData;
	class Collider : public Component
	{
	public: 
		//�R���X�g���N�^
		Collider( std::string tag, ObjectData* matrixObject) : tag(tag) { this->matrixObject.reset(matrixObject); }
		Collider() = default;
		//�f�X�g���N�^
		~Collider() { matrixObject.release(); };
		//����������
		virtual void Initialize() = 0;
		//�X�V����
		virtual void Update() { ClearCollisitonColliderList(); }
		//�����Ƃ̓����蔻��
		virtual void Collision(SegmentCollider* segmentCollider) = 0;
		//�O�p�`�|���S���Ƃ̓����蔻��
		virtual void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider) = 0;
		//������������Element�֐����Ăяo��
		void OnCollision() { listener(this); }
		//�����蔻��Ǘ��N���X�Ɏ��M�̏��𑗂�
		void RegisterCollider() {		
			CollisionManager* collisionManager = CollisionManager::GetInstance();
			collisionManager->AddCollider(this);
		}
		//���������R���C�_�[��ǉ�����
		void AddCollisionCollider(Collider* collider) { collisitonColliderListNow.emplace_back(collider); }
		//�������󂯎��
		void addListener(std::function<void(Collider* collider)> listener) {
			this->listener = listener;
		}
		//�^�O��ݒ肷��
		void SetTag(const std::string& tagName) { tag = tagName; }
		//�^�O���擾����
		const std::string& GetTag() { return tag; }

	protected:
		//�I�u�W�F�N�g�̃^�O(���̃^�O���������͓̂����蔻����s��Ȃ�)
		std::string tag;
		//���W�A�s��Ǘ��̃A�h���X
		std::unique_ptr<ObjectData> matrixObject;
		//���t���[���ɂăR���C�_�[�ɓ��������R���C�_�[���X�g���N���A
		void ClearCollisitonColliderList() { collisitonColliderListNow.clear(); }
	private:
		//�����_�����󂯎��
		std::function<void(Collider* collider)> listener = [&](Collider* collider) {};
		//���t���[���ɂăR���C�_�[�ɓ��������R���C�_�[���X�g
		std::vector<Collider*> collisitonColliderListNow;

	};



	/// <summary>
	/// ����
	/// </summary>
	//class SphereCollider : public Sphere, public Collider
	//{
	//public:
	//	//�R���X�g���N�^
	//	SphereCollider(const std::string& tag, MatrixObject* matrixObject) : Collider(tag, matrixObject) {};
	//	//�X�V����
	//	void Update() { center = matrixObject->GetPosition(); }
	//};

	//class InkSphereCollider : public SphereCollider
	//{
	//public:
	//	InkSphereCollider(const std::string& tag, MatrixObject* matrixObject): SphereCollider(tag, matrixObject) {};
	//	D3DXVECTOR4 color;
	//};




	////class InkSegmentCollider : public SegmentCollider
	////{
	////public:
	////	//�R���X�g���N�^
	////	InkSegmentCollider(const std::string& tag, MatrixObject* matrixObject) : SegmentCollider(tag, matrixObject) {};
	////	//�X�V
	////	void Update() {
	////		SegmentCollider::Update();
	////	}
	////	D3DXVECTOR4 color;
	////	int index;
	////};


	////�O�p�`
	//class TriangleCollider : public Triangle, public Collider
	//{
	//public:
	//	//�R���X�g���N�^
	//	TriangleCollider(const std::string& tag, MatrixObject* matrixObject) : Collider(tag, matrixObject) {}
	//	//�X�V
	//	void Update(){}
	//};


};