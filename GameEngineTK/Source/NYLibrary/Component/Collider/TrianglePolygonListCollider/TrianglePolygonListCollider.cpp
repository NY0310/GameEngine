#include "TrianglePolygonListCollider.h"

using namespace NYLibrary;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="objectData">�I�u�W�F�N�g�̃f�[�^</param>
TrianglePolygonListCollider::TrianglePolygonListCollider(ObjectData * objectData)
	:Collider(objectData)
{
	Initialize();
}

/// <summary>
/// ������
/// </summary>
void TrianglePolygonListCollider::Initialize()
{
	Update();
	//���M�𓖂��蔻��Ǘ��N���X�ɓo�^����
	RegisterCollider();
}

/// <summary>
/// �f�[�^���X�V
/// </summary>
void TrianglePolygonListCollider::Update()
{
	Collider::Update();

	//�|���S���f�[�^���X�V
	triangleList = objectData->GetPolygons();

	//���������O�p�`�|���S���̏���������
	collisionTriangle.normal = D3DXVECTOR3(0,0,0);
	collisionTriangle.p0 = D3DXVECTOR3(0, 0, 0);
	collisionTriangle.p1 = D3DXVECTOR3(0, 0, 0);
	collisionTriangle.p2 = D3DXVECTOR3(0, 0, 0);
	collisionTriangle.uv0 = D3DXVECTOR2(0, 0);
	collisionTriangle.uv1 = D3DXVECTOR2(0, 0);
	collisionTriangle.uv2 = D3DXVECTOR2(0, 0);

	//����������_��������
	inter = D3DXVECTOR3(0, 0, 0);
}

/// <summary>
/// �S�Ẵ|���S���Ɛ��œ����蔻����s��
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

