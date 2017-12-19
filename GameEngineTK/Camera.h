/// <summary>
/// �J�����𐧌䂷��N���X
/// </summary>

//	���d�C���N���[�h�̖h�~
#pragma once

//	�w�b�_�t�@�C���̃C���N���[�h
#include <d3d11_1.h>
#include <SimpleMath.h>

#include "Collision.h"
#include "Device.h"
class Camera
{
public:
	//	�R���X�g���N�^
	Camera(int width, int height);
	//	�f�X�g���N�^
	virtual ~Camera();

	//	�X�V
	virtual void Update();

	//bool Project(const Vector3 & worldPos, Vector2 * screenPos);

	//void UnProject(const Vector2 & screenPos, Segment * worldSegment);

	bool Project(const DirectX::SimpleMath::Vector3 & worldPos, DirectX::SimpleMath::Vector2 * screenPos);

	void UnProject(const DirectX::SimpleMath::Vector2 & screenPos, Segment * worldSegment);

	const DirectX::SimpleMath::Matrix& GetBillboard() const { return m_Billboard; }
	const DirectX::SimpleMath::Matrix& GetBillboardConstrainY() const { return m_BillboardConstrainY; }

	//	�J�����̈ʒu(���_���W�j�̎擾
	const DirectX::SimpleMath::Vector3& GetEyePos() { return m_eyepos; }

	//	�J�����̌��Ă����(�����_/�Q�Ɠ_)�̎擾
	const DirectX::SimpleMath::Vector3& GetRefPos() { return m_refpos; }

	//�J�����̏�����x�N�g���̎擾
	const DirectX::SimpleMath::Vector3& GetUpVec() { return m_upvec; }

	//	�r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetView();

	//	�ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& GetProjection();

	//�g��{���̎擾
	const float& GetZoom() { return m_zoom; }

	//	���_���W�̃Z�b�g
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);

	//	�Q�Ɠ_�̃Z�b�g
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);

	//	������x�N�g���̃Z�b�g
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);

	//	������������p�̃Z�b�g
	void SetFovY(float fovY);

	//	�A�X�y�N�g��̃Z�b�g
	void SetAspect(float aspect);

	//	�j�A�N���b�v�̃Z�b�g
	void SetNearClip(float nearclip);

	//	�t�@�[�N���b�v�̃Z�b�g
	void SetFarClip(float farclip);

	//�g��{���̃Z�b�g
	void SetZoom(float zoom) { m_zoom = zoom; }

protected:
	//	�J�����̈ʒu(���_���W�j
	DirectX::SimpleMath::Vector3 m_eyepos;
	//	�J�����̌��Ă����(�����_/�Q�Ɠ_)
	DirectX::SimpleMath::Vector3 m_refpos;
	//	�J�����̏�����x�N�g��
	DirectX::SimpleMath::Vector3 m_upvec;
	//	�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//	������������p
	float m_fovY;
	//	�A�X�y�N�g��(���c�̔䗦�j
	float m_aspect;
	// �j�A�N���b�v(��O�̕\�����E����)
	float m_NearClip;
	//	�t�@�[�N���b�v(���̕\�����E����)
	float m_FarClip;
	//	�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;
	// �r���{�[�h�s��
	DirectX::SimpleMath::Matrix m_Billboard;
	// �r���{�[�h�s��(Y���������j
	DirectX::SimpleMath::Matrix m_BillboardConstrainY;
	//�g��{��
	float m_zoom;
};