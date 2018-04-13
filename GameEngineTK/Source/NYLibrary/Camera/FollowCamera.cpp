//	�w�b�_�t�@�C���̃C���N���[�h
#include "FollowCamera.h"
#include "./../../Scene/GameScene/Charactor/Player/MainPlayer.h"

using namespace std;
using namespace NYLibrary;


//	�ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 1;
unique_ptr<FollowCamera> FollowCamera::camera ;

FollowCamera* FollowCamera::GetInstance()
{
	if (!camera)
		camera.reset(new FollowCamera());
	return camera.get();
}

//	�R���X�g���N�^
FollowCamera::FollowCamera()
{
	//	����������
	isFPS = false;
	player = nullptr;

}

//	�X�V
void FollowCamera::Update()
{

	if (!player)
		return;
	//	�J�������_���W�A�Q�Ɠ_���W
	D3DXVECTOR3 eyepos, refpos;
	if(isFPS == true)
	{//	FPS�J�����̏���
		//	�Q�Ɠ_���W���v�Z
		 D3DXVECTOR3 position = player->GetPosition() + D3DXVECTOR3(0, 0.2f, 0);
		//	���@����J�������W�ւ̍���
		D3DXVECTOR3 cameraV(0, 0, -CAMERA_DISTANCE);
		D3DXMATRIX rotmat;
		//	���@�̌��ɉ�荞�ނ��߂̉�]�s��
//		 D3DXMatrixRotationY(&rotmat,targetAngle);
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
		refpos = player->GetPosition() + D3DXVECTOR3(0, 0.3, 0);
		//	���@����J�������W�ւ̍���
		D3DXVECTOR3 cameraV(0, 0.2,CAMERA_DISTANCE);
		//	���@�̌��ɉ�荞�ނ��߂̉�]�s��
		D3DXMATRIX rotmat;
		//D3DXMatrixRotationY(&rotmat,targetAngle);
		//	�J�����ւ̃x�N�g������]
		D3DXVec3TransformNormal(&cameraV,&cameraV,&player->aimMatrix->GetRotationMatrix());
		//	�J�������W���v�Z
		eyepos = refpos + cameraV;
	}


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


	//	���N���X�̍X�V
	Camera::Update();
}
