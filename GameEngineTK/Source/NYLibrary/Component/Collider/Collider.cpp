#include "Collider.h"
#include "../Element.h"

using namespace NYLibrary;

Collider::Collider(ObjectData* objectData)
	: tag(objectData->GetTag())
{
	this->objectData.reset(objectData);
}

void Collider::Update()
{	//���t���[���ɂăR���C�_�[�ɓ��������R���C�_�[���X�g���N���A
	collisitonColliderListNow.clear();
	tag = objectData->GetTag();
}
