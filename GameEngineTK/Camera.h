/// <summary>
///	�J�����𐧌䂷��N���X
/// </summary>

/* ---- ���d�C���N���[�h�̖h�~ ---------- */
#pragma once
/* ---- ���C�u�����̃C���N���[�h ---------- */
#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
public:
	//�R���X�g���N�^
	Camera();
	//
	Camera(int width, int height);
	//�f�X�g���N�^
	virtual ~Camera();
	//�X�V����
	virtual void Update();
	//�r���[�s��擾
	DirectX::SimpleMath::Matrix GetViewMatrix()
	{
		return m_viewmat;
	}
	//�ˉe�s��̎擾
	DirectX::SimpleMath::Matrix GetProjectionMatrix()
	{
		return m_projmat;
	}
	//���_���W�̎擾
	DirectX::SimpleMath::Vector3 GetEyepos()
	{
		return m_eyepos;
	}
	//�Q�Ɠ_�̎擾
	DirectX::SimpleMath::Vector3 GetRefPos()
	{
		return m_refpos;
	}
	//������x�N�g��
	DirectX::SimpleMath::Vector3 GetUpVec()
	{
		return m_upvec;
	}
	//������������p�̎擾
	float GetFovY()
	{
		return m_fovY;
	}
	//�A�X�y�N�g��̎擾
	float GetAspect()
	{
		return m_aspect;
	}
	//�j�A�N���b�v�̎擾
	float GetNearClip()
	{
		return m_near_clip;
	}
	//�t�@�[�N���b�v�̎擾
	float GetFarClip()
	{
		return m_far_clip;
	}


	//���_���W�̃Z�b�g
	void SetEyePos(DirectX::SimpleMath::Vector3 eyepos)
	{
		m_eyepos = eyepos;
	}
	////�Q�Ɠ_�̃Z�b�g
	void SetRefPos(DirectX::SimpleMath::Vector3 refpos)
	{
		m_refpos = refpos;
	}
	//������x�N�g���̃Z�b�g
	void SetUpvec(DirectX::SimpleMath::Vector3 upvec)
	{
		m_upvec = upvec;
	}
	//������������p�̐ݒ�
	void SetFovY(float fory)
	{
		m_fovY = fory;
	}
	//�A�X�y�N�g��̐ݒ�
	void SetAspect(float aspect)
	{
		m_aspect = aspect;
	}
	//�j�A�N���b�v�̐ݒ�
	void SetNearClip(float nearclip)
	{
		m_near_clip = nearclip;
	}
	//�t�@�[�N���b�v�̐ݒ�
	void SetFarClip(float farclip)
	{
		m_far_clip = farclip;
	}
protected:
	//�r���[�s��
	DirectX::SimpleMath::Matrix  m_viewmat;
	//���_���W�i�J�����̈ʒu�j
	DirectX::SimpleMath::Vector3 m_eyepos;
	//�Q�Ɠ_/�����_(�J�����̌��Ă����j
	DirectX::SimpleMath::Vector3 m_refpos;
	//������x�N�g��
	DirectX::SimpleMath::Vector3 m_upvec;
	//�ˉe�s��
	DirectX::SimpleMath::Matrix m_projmat;
	//������������p(�c�����ɉ��x�܂ł��邩)
	float m_fovY;
	//�A�X�y�N�g��(���c�̔䗦)
	float m_aspect;
	//�j�A�N���b�v(��O�̕\�����E����)
	float m_near_clip;
	//�t�@�[�N���b�v(���̕\�����E����)
	float m_far_clip;
};


