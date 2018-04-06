#include "InkSegmentCollider.h"
#include "../../NYLibrary/Component/Element.h"

using namespace NYLibrary;
using namespace std;

InkSegmentCollider::InkSegmentCollider(ObjectData * objectData)
	:SegmentCollider(objectData)
{


}

void InkSegmentCollider::Initialize()
{
	color = objectData->GetColor();
	SegmentCollider::Initialize();
}

void InkSegmentCollider::Update()
{
	SegmentCollider::Update(); 

	auto & pos = FollowCamera::GetInstance()->GetEyePos();

	float startDistance = D3DXVec3Length(&(pos - start));
	float endDistance = D3DXVec3Length(&(pos - end));


	if (startDistance < endDistance)
	{
		D3DXVECTOR3 save = start;
		start = end;
		end = save;
	}




	color = objectData->GetColor();
}
