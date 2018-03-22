#include "SegmentCollider.h"
#include "../../Element.h"

using namespace NYLibrary;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="matrixObject">オブジェクトのデータ</param>
SegmentCollider::SegmentCollider(const std::string & tag, ObjectData * matrixObject):
	Collider(tag, matrixObject)
{
	Initialize();
}

/// <summary>
/// 初期化
/// </summary>
void SegmentCollider::Initialize()
{
	Update();
	//自身の情報を登録する
	RegisterCollider();
}

/// <summary>
/// オブジェクトのデータを更新する
/// </summary>
void SegmentCollider::Update()
{
	//現フレームにてコライダーに当たったコライダーリストをクリア
	collisitonColliderListNow.clear();


	D3DXVECTOR3 positon = objectData->GetPosition();
	float worldHalfSize =  objectData->GetScale().z * objectData->GetLocalSize() / 2;
	positon.z += worldHalfSize;
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, positon.x, positon.y, positon.z);
	D3DXMatrixRotationYawPitchRoll(&mat, objectData->GetRotation().x, objectData->GetRotation().y, objectData->GetRotation().z);
	start.x =  mat._41;
	start.y = mat._42;
	start.z = mat._43;


	positon = objectData->GetPosition();
	positon.z -= worldHalfSize;
	D3DXMatrixTranslation(&mat, positon.x, positon.y, positon.z);
	D3DXMatrixRotationYawPitchRoll(&mat, objectData->GetRotation().x, objectData->GetRotation().y, objectData->GetRotation().z);
	end.x = mat._41;
	end.y = mat._42;
	end.z = mat._43;
}




void SegmentCollider::Collision(TrianglePolygonListCollider * trianglePolygonListCollider)
{
	CheckSegment2AllTriangle(this, trianglePolygonListCollider);
}
