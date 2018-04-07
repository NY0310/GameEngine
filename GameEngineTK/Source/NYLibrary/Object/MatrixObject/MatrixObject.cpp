/// <summary>
/// �I�u�W�F�N�g�̍s��N���X
/// </summary>
#include "MatrixObject.h"
using namespace NYLibrary;

MatrixObject::MatrixObject()
{
	MatrixInitialize();
}


void NYLibrary::MatrixObject::MatrixInitialize()
{
	isUseQuternion = true;
	transfer = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	localSize = 0;
	eulerAangle = D3DXVECTOR3(0, 0, 0);
	quaternion = D3DXQUATERNION(0, 0, 0, 0);
	D3DXMatrixIdentity(&worldMatrix);//���[���h�s��
	D3DXMatrixIdentity(&transferMatrix);
	D3DXMatrixIdentity(&scaleMatrix);//�X�P�[���s��
	D3DXMatrixIdentity(&rotationMatrix);//��]�s��
	D3DXMatrixIdentity(&wvp);//���[���h�r���[�v���W�F�N�V�����s��
	worldMatrixOrderFactory = new WorldMatrixOrderFactory();
	//���[���h�s��̊|���Z�����t�@�N�g���[�̏�����
	order = worldMatrixOrderFactory->Set(WorldMatrixOrder::ORDER::SCALEMAT_ROTOMAT_TRANSMAT);

}


/// <summary>
/// ���[���h�s��̊|���Z�����ւ���
/// </summary>
/// <param name="order"></param>
void MatrixObject::ChangeOrder(WorldMatrixOrder::ORDER order)
{
	//this->order.release();
	this->order = worldMatrixOrderFactory->Set(order);
}



/// <summary>
/// ���[���h�s��쐬
/// </summary>
void MatrixObject::Calc()
{
	FollowCamera* camera = FollowCamera::GetInstance();
	//�S�Ă̍s����쐬
	CreateAllMatrix();
	//�X�P�[���E��]�E�ړ��s�񂩂烏�[���h�s��쐬
	order._Get()->Calculation(worldMatrix, scaleMatrix, rotationMatrix, transferMatrix);
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
	if(isCalcTransferMatrix)
	D3DXMatrixTranslation(&transferMatrix, this->transfer.x, this->transfer.y, this->transfer.z);
}

/// <summary>
/// ��]�s��쐬
/// </summary>
void MatrixObject::CreateRotationMatrix()
{
	if(isCalcRotationMatrix)
	//�N�H�[�^�j�I�����g�p���邩
	if (isUseQuternion)
	{
		D3DXMatrixRotationQuaternion(&rotationMatrix, &quaternion);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, eulerAangle.x, eulerAangle.y, eulerAangle.z);
	}
}


/// <summary>
/// �X�P�[���s��쐬
/// </summary>
void MatrixObject::CreateScaleMatrix()
{
	if(isCalcScaleMatrix)
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
}


