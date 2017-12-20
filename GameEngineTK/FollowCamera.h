/// <summary>
/// ���@�ɒǏ]����J�����N���X
/// </summary>

//	���d�C���N���[�h�̖h�~
#pragma once

#include <Windows.h>
#include <Keyboard.h>
#include "Camera.h"
#include "Player.h"

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

	//	�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTarGetTrans(const DirectX::SimpleMath::Vector3& tarGetTrans);
	//	�Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(float targetAngle);


	//const DirectX::SimpleMath::Vector3& GetEyepos()const { return ; }

	//	�L�[�{�[�h���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);
	//�v���C�����Z�b�g
	void SetPlayer(Player* player) { m_player = player; }
private:
	//���N���X�C���X�^���X
	static  std::unique_ptr<FollowCamera> camera;
protected:
	//	���@�̍��W
	DirectX::SimpleMath::Vector3 m_tarGetTrans;
	//	�Ǐ]�Ώۂ̉�]�p
	float m_targetAngle;
	//	�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//	�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//	FPS�t���O
	bool m_isFPS;
	//�v���C��
	Player* m_player;
};