#include "TrianglePolygonListCollider.h"

using namespace NYLibrary;

/// <summary>
/// �S�Ẵ|���S���Ɛ��œ����蔻����s��
/// </summary>
/// <param name="segment"></param>
void TrianglePolygonListCollider::Collision(SegmentCollider* segment)
{
	CheckSegment2AllTriangle(segment, this);
}


