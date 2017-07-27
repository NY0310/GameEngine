#include "KeyboardUtil.h"

using namespace DirectX;

KeyboardUtil::KeyboardUtil()
{
	m_Keyboard = std::make_unique<Keyboard>();
}

void KeyboardUtil::Update()
{
	// �L�[�{�[�h�X�e�[�g���擾
	m_State = m_Keyboard->GetState();
	// �g���K�[��Ԃ̍X�V
	m_KeyboardTracker.Update(m_State);
}

bool KeyboardUtil::IsPressed(DirectX::Keyboard::Keys key)
{
	// �X�e�[�g�̃L�[���������`�F�b�N
	return m_State.IsKeyDown(key);
}

bool KeyboardUtil::IsTriggered(DirectX::Keyboard::Keys key)
{
	// �g���b�J�[����g���K�[�����`�F�b�N
	return m_KeyboardTracker.IsKeyPressed(key);
}

bool KeyboardUtil::IsReleased(DirectX::Keyboard::Keys key)
{
	// �g���b�J�[���烊���[�X�����`�F�b�N
	return m_KeyboardTracker.IsKeyReleased(key);
}