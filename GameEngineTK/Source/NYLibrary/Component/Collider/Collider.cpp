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


	//���̏��
	center = objectData->GetPosition();
	float MaxLocalSize = 0.0f;
	float MaxScale = 0.0f;

	MaxLocalSize = Math::ComparisonBig(MaxLocalSize, objectData->GetLocalSize());

	MaxScale = Math::ComparisonBig(MaxScale, objectData->GetScale().x);
	MaxScale = Math::ComparisonBig(MaxScale, objectData->GetScale().y);
	MaxScale = Math::ComparisonBig(MaxScale, objectData->GetScale().z);

	radius = MaxLocalSize * MaxScale / 2;

}
