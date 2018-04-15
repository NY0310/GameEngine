#include "TrianglePolygonListCollider.h"

using namespace NYLibrary;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="objectData">オブジェクトのデータ</param>
TrianglePolygonListCollider::TrianglePolygonListCollider(ObjectData * objectData)
	:Collider(objectData)
{
	Initialize();
}

/// <summary>
/// 初期化
/// </summary>
void TrianglePolygonListCollider::Initialize()
{
	Update();
	//自信を当たり判定管理クラスに登録する
	RegisterCollider();
}

/// <summary>
/// データを更新
/// </summary>
void TrianglePolygonListCollider::Update()
{
	Collider::Update();

	//ポリゴンデータを更新
	triangleList = objectData->GetPolygons();

	//当たった三角形ポリゴンの情報を初期化
	collisionTriangle.normal = D3DXVECTOR3(0,0,0);
	collisionTriangle.p0 = D3DXVECTOR3(0, 0, 0);
	collisionTriangle.p1 = D3DXVECTOR3(0, 0, 0);
	collisionTriangle.p2 = D3DXVECTOR3(0, 0, 0);
	collisionTriangle.uv0 = D3DXVECTOR2(0, 0);
	collisionTriangle.uv1 = D3DXVECTOR2(0, 0);
	collisionTriangle.uv2 = D3DXVECTOR2(0, 0);

	//当たった交点を初期化
	inter = D3DXVECTOR3(0, 0, 0);
}

/// <summary>
/// 全てのポリゴンと線で当たり判定を行う
/// </summary>
/// <param name="segment"></param>
void TrianglePolygonListCollider::Collision(SegmentCollider* segment)
{
	CheckSegment2AllTriangle(segment, this);
}


void TrianglePolygonListCollider::Finalize()
{
	Collider::Finalize();
	triangleList.clear();
}

