#pragma once

#include <WinSDKVer.h>
#include <Keyboard.h>

// �L�[�{�[�h���[�e�B���e�B�N���X
class KeyboardUtil
{
public:
	// �R���X�g���N�^
	KeyboardUtil();
	// ���t���[���X�V
	void Update();
	// �w��L�[��������Ă��邩
	bool IsPressed(DirectX::Keyboard::Keys key);
	// �w��L�[�̃g���K�[�`�F�b�N
	bool IsTriggered(DirectX::Keyboard::Keys key);
	// �w��L�[�̃����[�X�`�F�b�N
	bool IsReleased(DirectX::Keyboard::Keys key);
protected:
	// �L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_Keyboard;
	// �X�e�[�g
	DirectX::Keyboard::State m_State;
	// �L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;
};