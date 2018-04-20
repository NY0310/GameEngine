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
	//�r���{�[�h�ɂ��Ȃ�
	isBillBoard = false;
	isParantInfluence = false;

}


/// <summary>
/// ���[���h�s��̊|���Z�����ւ���
/// </summary>
/// <param name="order"></param>
void MatrixObject::ChangeOrder(WorldMatrixOrder::ORDER order)
{
	this->order = worldMatrixOrderFactory->Set(order);
}



/// <summary>
/// ���[���h�s��쐬
/// </summary>
void MatrixObject::Calc()
{

	//�S�Ă̍s����쐬
	CalcAllMatrix();
	//���[���h�s����쐬
	CalcWorldMatrix();
	//���[���h�E�r���[�E�v���W�F�N�V�����s����쐬
	CalcWVP();
}

/// <summary>
/// �e�̃��[���h�s��ɉe����^����
/// </summary>
/// <param name="parentWorldMatrix">�e�̃��[���h�s��</param>
void MatrixObject::Calc(const D3DXMATRIX& parentWorldMatrix)
{
	//�S�Ă̍s����쐬
	CalcAllMatrix();
	//���[���h�s����쐬
	CalcWorldMatrix();
	this->worldMatrix *= parentWorldMatrix;
	//���[���h�E�r���[�E�v���W�F�N�V�����s����쐬
	CalcWVP();

}


/// <summary>
/// �S�s��쐬
/// </summary>
void MatrixObject::CalcAllMatrix()
{
	//�ړ��s��쐬
	CalcTransferMatrix();
	//��]�s��쐬
	CalcScaleMatrix();
	//�X�P�[���s��쐬
	CalcRotationMatrix();
}


/// <summary>
/// ���[���h�s����쐬
/// </summary>
void MatrixObject::CalcWorldMatrix()
{
	//�X�P�[���E��]�E�ړ��s�񂩂烏�[���h�s��쐬
	order._Get()->Calculation(worldMatrix, scaleMatrix, rotationMatrix, transferMatrix);
}


/// <summary>
/// ���[���h�E�r���[�E�v���W�F�N�V�����s����쐬
/// </summary>
void MatrixObject::CalcWVP()
{
	FollowCamera* camera = FollowCamera::GetInstance();

	if (isBillBoard)
	{

		//�r���{�[�h�s��A���[���h�s��E�r���[�s��E�v���W�F�N�V�����s����쐬
		wvp = camera->GetBillboard()  * worldMatrix  * camera->GetView() * camera->GetProjection();
	}
	else
	{
		//���[���h�s��E�r���[�s��E�v���W�F�N�V�����s����쐬
		wvp = worldMatrix * camera->GetView() * camera->GetProjection();
	}
}

/// <summary>
/// �ړ��s��쐬
/// </summary>
void MatrixObject::CalcTransferMatrix()
{
	if(isCalcTransferMatrix)
	D3DXMatrixTranslation(&transferMatrix, this->transfer.x, this->transfer.y, this->transfer.z);
}

/// <summary>
/// ��]�s��쐬
/// </summary>
void MatrixObject::CalcRotationMatrix()
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
void MatrixObject::CalcScaleMatrix()
{
	if(isCalcScaleMatrix)
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
}


