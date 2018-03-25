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

	p0 = objectData->GetPositionX();
	p1 = objectData->GetPositionY();
	p2 = objectData->GetPositionZ();
	r = objectData->GetScaleX() * objectData->GetLocalSize();
}


void PlaneCollider::Collision(SegmentCollider * segmentCollider)
{
	IntersectPlane2Segment(segmentCollider, this);
}
