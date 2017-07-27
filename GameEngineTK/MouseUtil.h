#pragma once

#include <windows.h>
#include <Mouse.h>
#include <DirectXMath.h>

// �}�E�X���[�e�B���e�B�N���X
class MouseUtil
{
public:
	// �{�^��
	enum Button {
		Left,
		Middle,
		Right,
		X1,
		X2,

		Num
	};
	using Mode = DirectX::Mouse::Mode;
public:
	// �R���X�g���N�^
	MouseUtil(HWND window, Mode mode = Mode::MODE_ABSOLUTE);
	// ���t���[���X�V
	void Update();
	//// �}�E�X�ڑ��`�F�b�N
	//bool IsConnected() const;
	// �w��{�^����������Ă��邩
	bool IsPressed(Button button);
	// �w��{�^���̃g���K�[�`�F�b�N
	bool IsTriggered(Button button);
	// �w��{�^���̃����[�X�`�F�b�N
	bool IsReleased(Button button);
	// ���[�h�Z�b�g
	void SetMode(Mode mode);
	// ���[�h�擾
	Mode GetMode();
	// �}�E�X���W�̎擾
	DirectX::XMINT2 GetPos();
	// �w���W�̎擾
	int GetX();
	// �x���W�̎擾
	int GetY();
	// �z�C�[���l�̃��Z�b�g
	void ResetWheelValue();
	// �z�C�[���l�̎擾
	int GetWheelValue();
protected:
	// �}�E�X
	std::unique_ptr<DirectX::Mouse> m_Mouse;
	// �X�e�[�g
	DirectX::Mouse::State m_State;
	// �{�^���X�e�[�g�g���b�J�[
	DirectX::Mouse::ButtonStateTracker m_ButtonStateTracker;
	// �{�^���������
	bool* m_ButtonStates[Button::Num];
	// �{�^���g���K�[���
	DirectX::Mouse::ButtonStateTracker::ButtonState* m_ButtonTrackerStates[Button::Num];
};