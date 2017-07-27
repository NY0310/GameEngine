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
	//	���@�ƃJ�����̋���
	static const float CAMERA_DISTANCE;
	//	�R���X�g���N�^
	FollowCamera(int width, int height);
	//	�X�V
	void Update() override;

	//	�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTarGetTrans(const DirectX::SimpleMath::Vector3& tarGetTrans);
	//	�Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(float targetAngle);

	//	�L�[�{�[�h���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);
	//�v���C�����Z�b�g
	void SetPlayer(Player* player) { m_player = player; }
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