#include "SegmentCollider.h"
#include "../../Element.h"

using namespace NYLibrary;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="matrixObject">�I�u�W�F�N�g�̃f�[�^</param>
SegmentCollider::SegmentCollider(ObjectData * objectData):
	Collider(objectData)
{

	Initialize();
}

/// <summary>
/// ������
/// </summary>
void SegmentCollider::Initialize()
{
	Update();
	//���g�̏���o�^����
	RegisterCollider();
}

/// <summary>
/// �I�u�W�F�N�g�̃f�[�^���X�V����
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
/// ��]��̐����̍��W�����߂�
/// </summary>
/// <param name="localposition">��]�𖳎��������W</param>
/// <returns>��]��̍��W</returns>
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
