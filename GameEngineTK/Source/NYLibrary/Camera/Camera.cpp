//	�w�b�_�t�@�C���̃C���N���[�h
#include "Camera.h"
using namespace NYLibrary;

//	�R���X�g���N�^
Camera::Camera()
{
	//	�����o�ϐ��̏�����
	eyepos = D3DXVECTOR3(0, 0, 5);
	refpos = D3DXVECTOR3(0, 0, 0);
	upvec = D3DXVECTOR3(0, 1, 0);

	fovY = XMConvertToRadians(60.0f);
	aspect = static_cast<float> (Devices::Get().Width()) / static_cast <float>(Devices::Get().Height());
	nearClip = 0.1f;
	farClip = 1000.0f;
	zoom = 1.0f;
	D3DXMatrixIdentity(&proj);

	//	�r���[�s��̐���
	D3DXMatrixLookAtRH(&view,&eyepos, &refpos,&upvec);

	//	�ˉe�s��̐���
	D3DXMatrixPerspectiveFovRH(&proj,fovY / zoom, aspect, nearClip, farClip);
}

//	�f�X�g���N�^
Camera::~Camera()
{

}

//	�X�V
void Camera::Update()
{
	//	�r���[�s��̐���
	D3DXMatrixLookAtRH(&view, &eyepos, &refpos, &upvec);

	//	�ˉe�s��̐���
	D3DXMatrixPerspectiveFovRH(&proj, fovY / zoom, aspect, nearClip, farClip);

}


/// <summary>
/// �R�c���Q�c���W�ϊ�
/// ���[���h���W����X�N���[�����W���v�Z����
/// </summary>
/// <param name="pos2d"></param>
///// <returns>����</returns>
bool Camera::Project(const D3DXVECTOR3& worldPos, D3DXVECTOR2* screenPos)
{
	auto& devices = Devices::Get();

	D3DXVECTOR4 clipPos;
	D3DXVECTOR4 worldPosV4(worldPos.x, worldPos.y, worldPos.z, 1.0f);

	// �r���[�ϊ�
	D3DXVec4Transform(&clipPos,&worldPosV4,&view);

	// �ˉe�ϊ�
	D3DXVec4Transform(&clipPos,&clipPos,&proj);

	// �r���[�|�[�g�̎擾
	D3D11_VIEWPORT viewport = devices.Viewport();

	// �j�A�N���b�v���O���ɂ��Ȃ���Όv�Z�ł��Ȃ�
	if (clipPos.w <= 1.0e-5) return false;

	// �r���[�|�[�g�ϊ�
	float ndcX = clipPos.x / clipPos.w;
	float ndcY = -clipPos.y / clipPos.w;

	screenPos->x = (ndcX + 1.0f) * 0.5f * viewport.Width + viewport.TopLeftX;
	screenPos->y = (ndcY + 1.0f) * 0.5f * viewport.Height + viewport.TopLeftY;

	return true;
}

/// <summary>
/// �R�c���Q�c���W�ϊ�
/// �X�N���[�����W���A�j�A�N���b�v�A�t�@�[�N���b�v�Ԃ̐����ɕϊ�����
/// </summary>
/// <param name="screenPos"></param>
/// <param name="worldSegment"></param>
void Camera::UnProject(const D3DXVECTOR2& screenPos, Segment* worldSegment)
{
	auto& devices = Devices::Get();


	D3DXVECTOR2 clipPos;
	D3DXVECTOR4 clipPosNear;
	D3DXVECTOR4 clipPosFar;

	// �r���[�|�[�g�̎擾
	//D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();

	// �X�N���[�����W���ˉe���W
	/*clipPos.x = (screenPos.x - viewport.TopLeftX) / (viewport.Width / 2.0f) - 1.0f;
	clipPos.y = (screenPos.y - viewport.TopLeftY) / (viewport.Height / 2.0f) - 1.0f;*/
	clipPos.y = -clipPos.y;

	clipPosNear.x = nearClip * clipPos.x;
	clipPosNear.y = nearClip * clipPos.y;
	clipPosNear.z = 0;
	clipPosNear.w = nearClip;

	clipPosFar.x = farClip * clipPos.x;
	clipPosFar.y = farClip * clipPos.y;
	clipPosFar.z = farClip;
	clipPosFar.w = farClip;

	// �v���W�F�N�V�����A�r���[�t�ϊ�
	D3DXMATRIX invMat = view * proj;
	D3DXMatrixIdentity(&invMat);

	D3DXMATRIX invView;
	D3DXMatrixIdentity(&invView);

	D3DXMATRIX invProj;
	D3DXMatrixIdentity(&invProj);

	// �ˉe���W���r���[���W
	D3DXVECTOR4 viewStart, viewEnd, start, end;
	D3DXVec4Transform(&viewStart,&clipPosNear, &invProj);
	D3DXVec4Transform(&viewEnd,&clipPosFar, &invProj);
	// �r���[���W�����[���h���W
	D3DXVec4Transform(&start,&viewStart, &invView);
	D3DXVec4Transform(&end,&viewEnd, &invView);

	worldSegment->start.x = start.x;
	worldSegment->start.y = start.y;
	worldSegment->start.z = start.z;

	worldSegment->end.x = end.x;
	worldSegment->end.y = end.y;
	worldSegment->end.z = end.z;
}



//	�r���[�s����擾����֐�
const D3DXMATRIX& Camera::GetView()
{
	return view;
}

//	�ˉe�s����擾����֐�
const D3DXMATRIX& Camera::GetProjection()
{
	return proj;
}

//	���_���W�̃Z�b�g����֐�
void Camera::SetEyePos(const D3DXVECTOR3& eyepos)
{
	this->eyepos = eyepos;
}

//	�Q�Ɠ_�̃Z�b�g
void Camera::SetRefPos(const D3DXVECTOR3& refpos)
{
	this->refpos = refpos;
}

//	������x�N�g���̃Z�b�g
void Camera::SetUpVec(const D3DXVECTOR3 & upvec)
{
	this->upvec = upvec;
}

//	������������p�̃Z�b�g
void Camera::SetFovY(float fovY)
{
	this->fovY = fovY;
}

//	�A�X�y�N�g��̃Z�b�g
void Camera::SetAspect(float aspect)
{
	aspect = aspect;
}

//	�j�A�N���b�v�̃Z�b�g
void Camera::SetNearClip(float nearclip)
{
	nearClip = nearclip;
}

//	�t�@�[�N���b�v�̃Z�b�g
void Camera::SetFarClip(float farclip)
{
	farClip = farclip;
}
