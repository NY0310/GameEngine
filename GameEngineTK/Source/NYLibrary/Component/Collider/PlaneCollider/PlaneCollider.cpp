#include "PlaneCollider.h"
#include "../../Element.h"

using namespace NYLibrary;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="matrixObject">�I�u�W�F�N�g�̃f�[�^</param>
PlaneCollider::PlaneCollider(ObjectData * objectData) :
	Collider(objectData)
{

	Initialize();
}

/// <summary>
/// ������
/// </summary>
void PlaneCollider::Initialize()
{
	Update();
	//���g�̏���o�^����
	RegisterCollider();
}

/// <summary>
/// �I�u�W�F�N�g�̃f�[�^���X�V����
/// </summary>
void PlaneCollider::Update()
{
	Collider::Update();

	//�|���S���̂ǂꂩ�ЂƂ̒��_�Ɩ@���x�N�g�����畽�ʂ��쐬����
	CreatePlaneFromPointNormal(objectData->GetPolygons().begin()->p2, objectData->GetPolygons().begin()->normal);

}




//�ЂƂ̒��_�Ɩ@���x�N�g�����畽�ʂ��쐬����
void PlaneCollider::CreatePlaneFromPointNormal(D3DXVECTOR3 pos, D3DXVECTOR3 normal)//��normal�͒P�ʃx�N�g���ł��邱��
{
	//p��normal�����
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
