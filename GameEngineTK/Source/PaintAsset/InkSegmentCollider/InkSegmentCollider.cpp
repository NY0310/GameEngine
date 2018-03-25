#include "InkSegmentCollider.h"
#include "../../NYLibrary/Component/Element.h"

using namespace NYLibrary;
using namespace std;

InkSegmentCollider::InkSegmentCollider(ObjectData * objectData)
	:SegmentCollider(objectData)
{
	int a = 0;
}

void InkSegmentCollider::Initialize()
{
	color = objectData->GetColor();
	SegmentCollider::Initialize();
}

void InkSegmentCollider::Update()
{
	SegmentCollider::Update(); 
	color = objectData->GetColor();
}
