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
		FollowCamera& operator= (const FollowCamera&) = delete;
		FollowCamera(const FollowCamera&) = delete;
		//	�X�V
		void Update() override;
		//�v���C�����Z�b�g
		void SetPlayer(Player* player) { this->player = player; }
	private:
		//	�R���X�g���N�^
		FollowCamera();
		//���N���X�C���X�^���X
		static  std::unique_ptr<FollowCamera> camera;
	protected:
		//	FPS�t���O
		bool isFPS;
		Player* player;
	};
};
