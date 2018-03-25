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
}
