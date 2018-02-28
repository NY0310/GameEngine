/// <summary>
/// �I�u�W�F�N�g�̍s��N���X
/// </summary>
#include "MatrixObject.h"

MatrixObject::MatrixObject()
{
	scale = D3DXVECTOR3(1,1,1);
	rotation = D3DXVECTOR3(0, 0, 0);
}

/// <summary>
/// ���[���h�s��쐬
/// </summary>
void MatrixObject::Update()
{
	FollowCamera* camera = FollowCamera::GetInstance();
	//�S�Ă̍s����쐬
	CreateAllMatrix();
	//�X�P�[���E��]�E�ړ��s�񂩂烏�[���h�s��쐬
	worldMatrix = scaleMatrix * rotationMatrix * transferMatrix;
	//���[���h�s��E�r���[�s��E�v���W�F�N�V�����s����쐬
	wvp = worldMatrix * camera->GetView() * camera->GetProjection();
}

/// <summary>
/// �S�s��쐬
/// </summary>
void MatrixObject::CreateAllMatrix()
{
	//�ړ��s��쐬
	CreateTransferMatrix();
	//��]�s��쐬
	CreateScaleMatrix();
	//�X�P�[���s��쐬
	CreateRotationMatrix();
}

/// <summary>
/// �ړ��s��쐬
/// </summary>
void MatrixObject::CreateTransferMatrix()
{
	D3DXMatrixTranslation(&transferMatrix, this->transfer.x, this->transfer.y, this->transfer.y);
}

/// <summary>
/// ��]�s��쐬
/// </summary>
void MatrixObject::CreateRotationMatrix()
{
	//�N�H�[�^�j�I�����g�p���邩
	if (IsUseQuternion)
	{
		D3DXMatrixRotationQuaternion(&rotationMatrix, &quaternion);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.x, rotation.y, rotation.z);
	}
}


/// <summary>
/// �X�P�[���s��쐬
/// </summary>
void MatrixObject::CreateScaleMatrix()
{
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
}


