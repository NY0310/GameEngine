#include "Collider.h"
#include "../Element.h"

using namespace NYLibrary;

Collider::Collider(ObjectData* objectData)
	: tag(objectData->GetTag())
{
	this->objectData = objectData;
	isActive = true;
}

void Collider::Update()
{	//���t���[���ɂăR���C�_�[�ɓ��������R���C�_�[���X�g���N���A
	collisitonColliderListNow.clear();
	tag = objectData->GetTag();


	//���̏��
	center = objectData->GetPosition();
	float MaxLocalSize = 0.0f;
	float MaxScale = 0.0f;

	MaxLocalSize = Math::ComparisonBig(objectData->GetLocalSize());

	MaxScale = Math::ComparisonBig(objectData->GetScale());

	radius = MaxLocalSize * MaxScale / 2;

}

void Collider::Finalize()
{
	collisitonColliderListNow.clear();
	CollisionManager* collisionManager = CollisionManager::GetInstance();
	collisionManager->EraseCollider(this);
	listener = [&](Collider* collider) {};
}
