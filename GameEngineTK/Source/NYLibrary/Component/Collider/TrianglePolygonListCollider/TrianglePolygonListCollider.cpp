#include "TrianglePolygonListCollider.h"

using namespace NYLibrary;

/// <summary>
/// ‘S‚Ä‚Ìƒ|ƒŠƒSƒ“‚Æü‚Å“–‚½‚è”»’è‚ğs‚¤
/// </summary>
/// <param name="segment"></param>
void TrianglePolygonListCollider::Collision(SegmentCollider* segment)
{
	CheckSegment2AllTriangle(segment, this);
}


