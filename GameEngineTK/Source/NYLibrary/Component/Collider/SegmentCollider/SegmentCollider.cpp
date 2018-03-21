#include "SegmentCollider.h"
#include "../../Element.h"

using namespace NYLibrary;

SegmentCollider::SegmentCollider(const std::string & tag, ObjectData * matrixObject):
	Collider(tag, matrixObject)
{
}

void SegmentCollider::Initialize()
{
	D3DXVECTOR3 positon = matrixObject->GetPosition();
	D3DXVECTOR3 worldSize = matrixObject->GetScale() * matrixObject->GetLocalSize();

	start = positon - worldSize / 2;
	end = positon + worldSize / 2;

	//©g‚Ìî•ñ‚ğ“o˜^‚·‚é
	RegisterCollider();
}



void SegmentCollider::Collision(TrianglePolygonListCollider * trianglePolygonListCollider)
{
	CheckSegment2AllTriangle(this, trianglePolygonListCollider);
}
