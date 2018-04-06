#include "Collider.h"
#include "../Element.h"

using namespace NYLibrary;

Collider::Collider(ObjectData* objectData)
	: tag(objectData->GetTag())
{
	this->objectData.reset(objectData);

}

void Collider::Update()
{	//現フレームにてコライダーに当たったコライダーリストをクリア
	collisitonColliderListNow.clear();
	tag = objectData->GetTag();


	//球の情報
	center = objectData->GetPosition();
	float MaxLocalSize = 0.0f;
	float MaxScale = 0.0f;

	MaxLocalSize = Math::ComparisonBig(MaxLocalSize, objectData->GetLocalSize());

	MaxScale = Math::ComparisonBig(MaxScale, objectData->GetScale().x);
	MaxScale = Math::ComparisonBig(MaxScale, objectData->GetScale().y);
	MaxScale = Math::ComparisonBig(MaxScale, objectData->GetScale().z);

	radius = MaxLocalSize * MaxScale / 2;

}
