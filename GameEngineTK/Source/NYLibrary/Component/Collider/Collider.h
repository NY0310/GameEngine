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
		Collider(std::string tag, ObjectData* objectData) : tag(tag) {
			this->objectData.reset(objectData); }
		Collider() = delete;
		//�f�X�g���N�^
		~Collider() { objectData.release(); };
		//������
		virtual void Initialize() {}
		//�X�V
		virtual void Update() {}
		//�����Ƃ̓����蔻��
		virtual void Collision(SegmentCollider* segmentCollider) {}
		//�O�p�`�|���S���Ƃ̓����蔻��
		virtual void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider) {}
		//�����蔻��Ǘ��N���X�Ɏ��M�̏��𑗂�
		void RegisterCollider() {		
			CollisionManager* collisionManager = CollisionManager::GetInstance();
			collisionManager->AddCollider(this);
		}
		//�������󂯎��
		void addListener(std::function<void(Collider* collider)> listener) {
			this->listener = listener;
		}
		//�^�O���擾����
		const std::string& GetTag() { return tag; }
		// �A�N�e�B�u��Ԃ�ݒ肷��
		void SetActive(bool active) { isActive = active; }
		// �A�N�e�B�u���擾����
		bool GetActive()const { return isActive; }
		//�I�u�W�F�N�g�f�[�^���擾
		ObjectData* GetObjectData() { return objectData.get(); }
		//������������Element�֐����Ăяo��
		void OnCollision() { listener(this); }
		void AddCollisionCollider(Collider* collider) { collisitonColliderListNow.emplace_back(collider); }
	protected:
		//�I�u�W�F�N�g�̃^�O(���̃^�O���������͓̂����蔻����s��Ȃ�)
		std::string tag;
		//���W�A�s��Ǘ��̃A�h���X
		std::unique_ptr<ObjectData> objectData;
		//�A�N�e�B�u��Ԃ�
		bool isActive;
		//���t���[���ɂăR���C�_�[�ɓ��������R���C�_�[���X�g
		std::vector<Collider*> collisitonColliderListNow;
	private:
		//�����_�����󂯎��
		std::function<void(Collider* collider)> listener = [&](Collider* collider) {};
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