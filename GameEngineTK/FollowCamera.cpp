//	�w�b�_�t�@�C���̃C���N���[�h
#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//	�ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

//	�R���X�g���N�^
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
{
	//	����������
	m_tarGetTrans = Vector3::Zero;
	m_targetAngle = 0.0f;
	m_keyboard = nullptr;
	m_isFPS = false;
	m_player = nullptr;
}

//	�X�V
void FollowCamera::Update()
{
	//	�L�[�{�[�h�̏�ԊG��擾
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);

	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		m_isFPS = !m_isFPS;
	}

	//�Ǐ]�Ώۂ̍��W�����Z�b�g
	SetTarGetTrans(m_player->GetTrans());
	SetTargetAngle(m_player->GetRot().y);

	//	�J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;
	if(m_isFPS == true)
	{//	FPS�J�����̏���
		Vector3 position;
		//	�Q�Ɠ_���W���v�Z
		position = m_tarGetTrans + Vector3(0, 0.2f, 0);
		//	���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		//	���@�̌��ɉ�荞�ނ��߂̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//	�J�����ւ̃x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//	������Ɛi�񂾈ʒu�����_���W
		eyepos = position + cameraV * 0.1f;
		//	������i�񂾈ʒu���Q�Ɠ_���W
		refpos = position + cameraV;
	}
	else
	{//	TPS�J�����̏���
	 //	�Q�Ɠ_���W���v�Z
		refpos = m_tarGetTrans + Vector3(0, 2, 0);
		//	���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
		//	���@�̌��ɉ�荞�ނ��߂̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//	�J�����ւ̃x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//	�J�������W���v�Z
		eyepos = refpos + cameraV;
	}

	//	���_�����݈ʒu�����Ԃ���
	eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
	//	�Q�Ɠ_�����݈ʒu�����Ԃ���
	refpos = m_refpos + (refpos - m_refpos) * 0.2f;

	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	//this->SetEyePos(Vector3(0.00749884499f, 1.54812920f, 4.99988747f)); //���_�ʒu
	//this->SetRefPos(Vector3(-3.61911745e-08f, 1.98155320f, -3.12580148e-08f));//�����ʒu


	//this->SetEyePos(Vector3(0.0f, 0.0f, -2.0f)); //���_�ʒu
	//this->SetRefPos(Vector3(-0.0f, 0.5f, 0.0f));//�����ʒu
	//this->SetUpVec(Vector3(0.0f, 1.0f, 0.0f));//����ʒu
	//m_fovY = XM_PI / 4;
	//m_aspect = (FLOAT)640 / (FLOAT)480;
	//m_NearClip = 0.1f;
	//m_FarClip = 100.0f;


	//	���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::SetTarGetTrans(const DirectX::SimpleMath::Vector3 & targetPos)
{
	m_tarGetTrans = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngle)
{
	m_targetAngle = targetAngle;
}

//	�L�[�{�[�h���Z�b�g
void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}

