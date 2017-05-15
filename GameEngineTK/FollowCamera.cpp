#include "FollowCamera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Update()
{	
	//���_�A�Q�Ɠ_
	Vector3 eyepos, refpos;
	//�^�[�Q�b�g���W�́A�����̍��W�ɒǏ]
	//�Q�Ɠ_���W���v�Z
	refpos = m_targetPos + Vector3(0, 2.0f, 0);
	//�^�[�Q�b�g���W����J�������W�ւ̍���
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
	//�����̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	//�J�����ւ̃x�N�g������]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//�J�������W�n����
	eyepos = refpos + cameraV;
	//�J�����̍X�V
	SetEyePos(eyepos);
	SetRefPos(refpos);

	//���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}
