#include "PlaneCollider.h"
#include "../../Element.h"

using namespace NYLibrary;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="matrixObject">オブジェクトのデータ</param>
PlaneCollider::PlaneCollider(ObjectData * objectData) :
	Collider(objectData)
{

	Initialize();
}

/// <summary>
/// 初期化
/// </summary>
void PlaneCollider::Initialize()
{
	Update();
	//自身の情報を登録する
	RegisterCollider();
}

/// <summary>
/// オブジェクトのデータを更新する
/// </summary>
void PlaneCollider::Update()
{
	Collider::Update();

	p0 = objectData->GetPositionX();
	p1 = objectData->GetPositionY();
	p2 = objectData->GetPositionZ();
	r = objectData->GetScaleX() * objectData->GetLocalSize();
}


void PlaneCollider::Collision(SegmentCollider * segmentCollider)
{
	IntersectPlane2Segment(segmentCollider, this);
}
