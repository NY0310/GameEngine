//	�w�b�_�t�@�C���̃C���N���[�h
#include "Camera.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

//	�R���X�g���N�^
Camera::Camera()
{
	//	�����o�ϐ��̏�����
	m_eyepos = Vector3(0, 0, 5);
	m_refpos = Vector3(0, 0, 0);
	m_upvec = Vector3(0, 1, 0);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = Devices::Get().Width() / Devices::Get().Height();
	m_NearClip = 0.1f;
	m_FarClip = 1000.0f;
	m_zoom = 1.0f;
	m_proj = Matrix::Identity;

	//	�r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//	�ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY / m_zoom, m_aspect, m_NearClip, m_FarClip);
}

//	�f�X�g���N�^
Camera::~Camera()
{

}

//	�X�V
void Camera::Update()
{
	//	�r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//	�ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY / m_zoom, m_aspect, m_NearClip, m_FarClip);
}


/// <summary>
/// �R�c���Q�c���W�ϊ�
/// ���[���h���W����X�N���[�����W���v�Z����
/// </summary>
/// <param name="pos2d"></param>
///// <returns>����</returns>
bool Camera::Project(const Vector3& worldPos, Vector2* screenPos)
{
	auto& devices = Devices::Get();

	Vector4 clipPos;
	Vector4 worldPosV4(worldPos.x, worldPos.y, worldPos.z, 1.0f);

	// �r���[�ϊ�
	clipPos = Vector4::Transform(worldPosV4, m_view);

	// �ˉe�ϊ�
	clipPos = Vector4::Transform(clipPos, m_proj);

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
void Camera::UnProject(const Vector2& screenPos, Segment* worldSegment)
{
	auto& devices = Devices::Get();


	Vector2 clipPos;
	Vector4 clipPosNear;
	Vector4 clipPosFar;

	// �r���[�|�[�g�̎擾
	//D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();

	// �X�N���[�����W���ˉe���W
	/*clipPos.x = (screenPos.x - viewport.TopLeftX) / (viewport.Width / 2.0f) - 1.0f;
	clipPos.y = (screenPos.y - viewport.TopLeftY) / (viewport.Height / 2.0f) - 1.0f;*/
	clipPos.y = -clipPos.y;

	clipPosNear.x = m_NearClip * clipPos.x;
	clipPosNear.y = m_NearClip * clipPos.y;
	clipPosNear.z = 0;
	clipPosNear.w = m_NearClip;

	clipPosFar.x = m_FarClip * clipPos.x;
	clipPosFar.y = m_FarClip * clipPos.y;
	clipPosFar.z = m_FarClip;
	clipPosFar.w = m_FarClip;

	// �v���W�F�N�V�����A�r���[�t�ϊ�
	Matrix invMat = m_view * m_proj;
	invMat.Invert(invMat);

	Matrix invView;
	m_view.Invert(invView);

	Matrix invProj;
	m_proj.Invert(invProj);

	// �ˉe���W���r���[���W
	Vector4 viewStart = Vector4::Transform(clipPosNear, invProj);
	Vector4 viewEnd = Vector4::Transform(clipPosFar, invProj);
	// �r���[���W�����[���h���W
	Vector4 start = Vector4::Transform(viewStart, invView);
	Vector4 end = Vector4::Transform(viewEnd, invView);

	worldSegment->Start.x = start.x;
	worldSegment->Start.y = start.y;
	worldSegment->Start.z = start.z;

	worldSegment->End.x = end.x;
	worldSegment->End.y = end.y;
	worldSegment->End.z = end.z;
}



//	�r���[�s����擾����֐�
const Matrix& Camera::GetView()
{
	return m_view;
}

//	�ˉe�s����擾����֐�
const Matrix& Camera::GetProjection()
{
	return m_proj;
}

//	���_���W�̃Z�b�g����֐�
void Camera::SetEyePos(const Vector3& eyepos)
{
	m_eyepos = eyepos;
}

//	�Q�Ɠ_�̃Z�b�g
void Camera::SetRefPos(const Vector3& refpos)
{
	m_refpos = refpos;
	//m_refpos = Vector3(0, 0, 0);
}

//	������x�N�g���̃Z�b�g
void Camera::SetUpVec(const DirectX::SimpleMath::Vector3 & upvec)
{
	m_upvec = upvec;
}

//	������������p�̃Z�b�g
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

//	�A�X�y�N�g��̃Z�b�g
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

//	�j�A�N���b�v�̃Z�b�g
void Camera::SetNearClip(float nearclip)
{
	m_NearClip = nearclip;
}

//	�t�@�[�N���b�v�̃Z�b�g
void Camera::SetFarClip(float farclip)
{
	m_FarClip = farclip;
}
