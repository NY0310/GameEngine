/// <summary>
/// �J�����𐧌䂷��N���X
/// </summary>

//	���d�C���N���[�h�̖h�~
#pragma once

//	�w�b�_�t�@�C���̃C���N���[�h
#pragma once;

#include "../Collision/Collision.h"
#include "../Device/Device.h"

namespace NYLibrary
{
	class Camera
	{
	public:
		//	�R���X�g���N�^
		Camera();
		//	�f�X�g���N�^
		virtual ~Camera();

		//	�X�V
		virtual void Update();

		//bool Project(const Vector3 & worldPos, Vector2 * screenPos);

		//void UnProject(const Vector2 & screenPos, Segment * worldSegment);

		bool Project(const D3DXVECTOR3& worldPos, D3DXVECTOR2* screenPos);

		void UnProject(const D3DXVECTOR2 & screenPos, Segment * worldSegment);

		const D3DXMATRIX& GetBillboard() const { return billboard; }
		const D3DXMATRIX& GetBillboardConstrainY() const { return billboardConstrainY; }

		//	�J�����̈ʒu(���_���W�j�̎擾
		const D3DXVECTOR3& GetEyePos() { return eyepos; }

		//	�J�����̌��Ă����(�����_/�Q�Ɠ_)�̎擾
		const D3DXVECTOR3& GetRefPos() { return refpos; }

		//�J�����̏�����x�N�g���̎擾
		const D3DXVECTOR3& GetUpVec() { return upvec; }

		//	�r���[�s��̎擾
		const D3DXMATRIX& GetView();

		//	�ˉe�s��̎擾
		const D3DXMATRIX& GetProjection();

		//�g��{���̎擾
		const float& GetZoom() { return zoom; }

		//	���_���W�̃Z�b�g
		void SetEyePos(const D3DXVECTOR3& eyepos);

		//	�Q�Ɠ_�̃Z�b�g
		void SetRefPos(const D3DXVECTOR3& refpos);

		//	������x�N�g���̃Z�b�g
		void SetUpVec(const D3DXVECTOR3& upvec);

		//	������������p�̃Z�b�g
		void SetFovY(float fovY);

		//	�A�X�y�N�g��̃Z�b�g
		void SetAspect(float aspect);

		//	�j�A�N���b�v�̃Z�b�g
		void SetNearClip(float nearclip);

		//	�t�@�[�N���b�v�̃Z�b�g
		void SetFarClip(float farclip);

		//�g��{���̃Z�b�g
		void SetZoom(float zoom) { this->zoom = zoom; }

	protected:
		//	�J�����̈ʒu(���_���W�j
		D3DXVECTOR3 eyepos;
		//	�J�����̌��Ă����(�����_/�Q�Ɠ_)
		D3DXVECTOR3 refpos;
		//	�J�����̏�����x�N�g��
		D3DXVECTOR3 upvec;
		//	�r���[�s��
		D3DXMATRIX view;
		//	������������p
		float fovY;
		//	�A�X�y�N�g��(���c�̔䗦�j
		float aspect;
		// �j�A�N���b�v(��O�̕\�����E����)
		float nearClip;
		//	�t�@�[�N���b�v(���̕\�����E����)
		float farClip;
		//	�ˉe�s��
		D3DXMATRIX proj;
		// �r���{�[�h�s��
		D3DXMATRIX billboard;
		// �r���{�[�h�s��(Y���������j
		D3DXMATRIX billboardConstrainY;
		//�g��{��
		float zoom;
	};
};