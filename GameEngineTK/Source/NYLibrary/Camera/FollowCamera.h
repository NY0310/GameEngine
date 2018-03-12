/// <summary>
/// ���@�ɒǏ]����J�����N���X
/// </summary>

//	���d�C���N���[�h�̖h�~
#pragma once

#include <Windows.h>
#include "Camera.h"
class Player;

namespace NYLibrary
{
	class FollowCamera : public Camera
	{
	public:
		static FollowCamera* GetInstance();

		//	���@�ƃJ�����̋���
		static const float CAMERA_DISTANCE;
		//	�R���X�g���N�^
		FollowCamera();
		FollowCamera& operator= (const FollowCamera&) = delete;
		FollowCamera(const FollowCamera&) = delete;
		//	�X�V
		void Update() override;



		//const DirectX::SimpleMath::Vector3& GetEyepos()const { return ; }
		//�v���C�����Z�b�g
		void SetPlayer(Player* player) { this->player = player; }
	private:
		//	�Ǐ]�Ώۂ̍��W���Z�b�g
		void SetTarGetTrans(const D3DXVECTOR3& tarGetTrans);
		//	�Ǐ]�Ώۂ̉�]�p���Z�b�g
		void SetTargetAngle(float targetAngle);
		//���N���X�C���X�^���X
		static  std::unique_ptr<FollowCamera> camera;
	protected:
		//	���@�̍��W
		D3DXVECTOR3 targetTrans;
		//	�Ǐ]�Ώۂ̉�]�p
		float targetAngle;
		//	FPS�t���O
		bool isFPS;
		Player* player;
	};
};
