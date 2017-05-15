/* ---- ���C�u�����̃C���N���[�h ---------- */
#include "Camera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
/***************************************************************************
*|	�T�v�@�@�R���X�g���N�^
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
Camera::Camera()
	:m_eyepos(0,0,0)
	,m_refpos(0,0,0)
	,m_upvec(0,0,0)
	,m_fovY(XMConvertToRadians(60.0f))
	,m_near_clip(0.1f)
	,m_far_clip(0.1f)
{
	//�r���[�s�񐶐�
	//m_viewmat = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//�ˉe�s��̐���
	//m_projmat = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_near_clip, m_far_clip);
}
/***************************************************************************
*|	�T�v�@�@�R���X�g���N�^
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
Camera::Camera(int width, int height)
	:m_eyepos(0, 0, 5)
	,m_refpos(0, 0, 0)
	,m_upvec(0, 1, 0)
	,m_fovY(XMConvertToRadians(60.0f))
	,m_aspect(width / height)
	,m_near_clip(0.1f)
	,m_far_clip(1000.0f)
{
	//�p�x��ۂ����܂ܒ�����1�ɂ���
	m_upvec.Normalize();
	//�r���[�s�񐶐�
	m_viewmat = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//�ˉe�s��̐���
	m_projmat = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_near_clip, m_far_clip);
}
/***************************************************************************
*|	�T�v�@�@�f�X�g���N�^
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
Camera::~Camera()
{
}

/***************************************************************************
*|	�T�v�@�@�X�V����
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
void Camera::Update()
{
	//�r���[�s�񐶐�
	m_viewmat = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//�ˉe�s��̐���
	m_projmat = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_near_clip, m_far_clip);
}
