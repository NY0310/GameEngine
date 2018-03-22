#include "SegmentCollider.h"
#include "../../Element.h"

using namespace NYLibrary;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="matrixObject">�I�u�W�F�N�g�̃f�[�^</param>
SegmentCollider::SegmentCollider(const std::string & tag, ObjectData * matrixObject):
	Collider(tag, matrixObject)
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
	//���t���[���ɂăR���C�_�[�ɓ��������R���C�_�[���X�g���N���A
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
