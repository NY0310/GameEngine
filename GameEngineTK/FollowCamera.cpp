//	�w�b�_�t�@�C���̃C���N���[�h
#include "FollowCamera.h"

using namespace std;
//	�ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;
unique_ptr<FollowCamera> FollowCamera::camera ;

FollowCamera* FollowCamera::GetInstance()
{
	if (!camera)
		camera =  make_unique<FollowCamera>();
	return camera.get();
}

//	�R���X�g���N�^
FollowCamera::FollowCamera()
{
	//	����������
	tarGetTrans = D3DXVECTOR3(0,0,0);
	targetAngle = 0.0f;
	isFPS = false;
	//m_player = nullptr;
}

//	�X�V
void FollowCamera::Update()
{
	//	�L�[�{�[�h�̏�ԊG��擾
	//Keyboard::State keystate = m_keyboard->GetState();
	//m_keyboardTracker.Update(keystate);

	//if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	//{
	//	m_isFPS = !m_isFPS;
	//}

	//�Ǐ]�Ώۂ̍��W�����Z�b�g
	/*SetTarGetTrans(m_player->GetTrans());
	SetTargetAngle(m_player->GetRot().y);*/

	//	�J�������_���W�A�Q�Ɠ_���W
	D3DXVECTOR3 eyepos, refpos;
	if(isFPS == true)
	{//	FPS�J�����̏���
		//	�Q�Ɠ_���W���v�Z
		 D3DXVECTOR3 position = tarGetTrans + D3DXVECTOR3(0, 0.2f, 0);
		//	���@����J�������W�ւ̍���
		D3DXVECTOR3 cameraV(0, 0, -CAMERA_DISTANCE);
		D3DXMATRIX rotmat;
		//	���@�̌��ɉ�荞�ނ��߂̉�]�s��
		 D3DXMatrixRotationY(&rotmat,targetAngle);
		//	�J�����ւ̃x�N�g������]
		D3DXVec3TransformNormal(&cameraV,&cameraV, &rotmat);
		//	������Ɛi�񂾈ʒu�����_���W
		eyepos = position + cameraV * 0.1f;
		//	������i�񂾈ʒu���Q�Ɠ_���W
		refpos = position + cameraV;
	}
	else
	{//	TPS�J�����̏���
	 //	�Q�Ɠ_���W���v�Z
		refpos = tarGetTrans + D3DXVECTOR3(0, 2, 0);
		//	���@����J�������W�ւ̍���
		D3DXVECTOR3 cameraV(0, 0, CAMERA_DISTANCE);
		//	���@�̌��ɉ�荞�ނ��߂̉�]�s��
		D3DXMATRIX rotmat;
		D3DXMatrixRotationY(&rotmat,targetAngle);
		//	�J�����ւ̃x�N�g������]
		D3DXVec3TransformNormal(&cameraV,&cameraV,&rotmat);
		//	�J�������W���v�Z
		eyepos = refpos + cameraV;
	}

	//	���_�����݈ʒu�����Ԃ���
	eyepos = eyepos + (eyepos - eyepos) * 0.05f;
	//	�Q�Ɠ_�����݈ʒu�����Ԃ���
	refpos = refpos + (refpos - refpos) * 0.2f;

	//���[�J����]
	//Yaw
	if (eyepos.y != eyepos.y) 
	{
		//	���_�����݈ʒu�����Ԃ���
		eyepos.y = eyepos.y + (eyepos.y - eyepos.y) * 0.05f;
		//	�Q�Ɠ_�����݈ʒu�����Ԃ���
		refpos.y = refpos.y + (refpos.y - refpos.y) * 0.2f;
	}
	//Pich
	else if (eyepos.x != eyepos.x)
	{
		eyepos.x = eyepos.x + (eyepos.x - eyepos.x) * 0.05f;
		refpos.x = refpos.x + (refpos.x - refpos.x) * 0.2f;
	}
	//Roll
	else if (eyepos.z != eyepos.z)
	{
		eyepos.z = eyepos.z + (eyepos.z - eyepos.z) * 0.05f;
		refpos.z = refpos.z + (refpos.z - refpos.z) * 0.2f;
	}

	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	////�g��{���̕⊮
	//m_zoom += (zoom - m_zoom) * 0.05f;

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
void FollowCamera::SetTarGetTrans(const D3DXVECTOR3& targetPos)
{
	this->tarGetTrans = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngle)
{
	this->targetAngle = targetAngle;
}

////	�L�[�{�[�h���Z�b�g
//void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
//{
//	m_keyboard = keyboard;
//}

