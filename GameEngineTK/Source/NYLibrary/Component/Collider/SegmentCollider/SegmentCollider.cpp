#include "SegmentCollider.h"
#include "../../Element.h"

using namespace NYLibrary;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="matrixObject">オブジェクトのデータ</param>
SegmentCollider::SegmentCollider(ObjectData * objectData):
	Collider(objectData)
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
	Collider::Update();
	CalcSegmentPosition();
}


void SegmentCollider::CalcSegmentPosition()
{
	D3DXVECTOR3 positon = objectData->GetPosition();
	float worldHalfSize = objectData->GetScale().z * objectData->GetLocalSize() / 2;
	start = CalcSegmentPosition(D3DXVECTOR3(0, 0, + worldHalfSize), startWorldMatrix);
	end = CalcSegmentPosition(D3DXVECTOR3(0, 0, - worldHalfSize), endWorldMatrix);
}

/// <summary>
/// 回転後の線分の座標を求める
/// </summary>
/// <param name="localposition">回転を無視した座標</param>
/// <returns>回転後の座標</returns>
D3DXVECTOR3 SegmentCollider::CalcSegmentPosition(const D3DXVECTOR3& localposition,D3DXMATRIX& matrix)
{
	D3DXMATRIX trans;
	D3DXMATRIX rotMat;
	D3DXMatrixIdentity(&matrix);
	//D3DXMatrixScaling(&mat, objectData->GetScaleX(), objectData->GetScaleY(), objectData->GetScaleZ());
	D3DXMatrixTranslation(&trans, localposition.x, localposition.y, localposition.z);
	matrix =  trans *  objectData->GetRotationMatrix() * objectData->GetTransferMatrix();

	return D3DXVECTOR3(matrix._41, matrix._42, matrix._43);
}


void SegmentCollider::Collision(TrianglePolygonListCollider * trianglePolygonListCollider)
{
	CheckSegment2AllTriangle(this, trianglePolygonListCollider);
}

void NYLibrary::SegmentCollider::Collision(PlaneCollider * planeCollider)
{
	IntersectPlane2Segment(this, planeCollider);
}
