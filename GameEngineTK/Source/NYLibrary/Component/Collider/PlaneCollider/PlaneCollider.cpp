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

	//ポリゴンのどれかひとつの頂点と法線ベクトルから平面を作成する
	CreatePlaneFromPointNormal(objectData->GetPolygons().begin()->p2, objectData->GetPolygons().begin()->normal);

}




//ひとつの頂点と法線ベクトルから平面を作成する
void PlaneCollider::CreatePlaneFromPointNormal(D3DXVECTOR3 pos, D3DXVECTOR3 normal)//※normalは単位ベクトルであること
{
	//pとnormalを内積
	//r = pos.x * normal.x + pos.y * normal.y + pos.z * normal.z;
	r = D3DXVec3Dot(&pos, &normal);
	nX = normal.x;
	nY = normal.y;
	nZ = normal.z;
}



void PlaneCollider::Collision(SegmentCollider * segmentCollider)
{
	IntersectPlane2Segment(segmentCollider, this);
}
