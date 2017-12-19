#include <assert.h>

#include "MouseUtil.h"

using namespace DirectX;
using namespace MyLibrary;

// static member variable
std::unique_ptr<MouseUtil> MouseUtil::m_Instance;

MouseUtil * MouseUtil::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = std::make_unique<MouseUtil>();
	}

	return m_Instance.get();
}

using ButtonState = Mouse::ButtonStateTracker::ButtonState;

MouseUtil::MouseUtil(DirectX::Mouse::Mode mode)
{
	m_Mouse = std::make_unique<Mouse>();
	m_Mouse->SetMode(mode);

	// 押下状態の変数アドレスを記録
	m_ButtonStates[Left] =		&m_State.leftButton;
	m_ButtonStates[Middle] =	&m_State.middleButton;
	m_ButtonStates[Right] =		&m_State.rightButton;
	m_ButtonStates[X1] =		&m_State.xButton1;
	m_ButtonStates[X2] =		&m_State.xButton2;
	// トリガー状態の変数アドレスを記録
	m_ButtonTrackerStates[Left] =	&m_ButtonStateTracker.leftButton;
	m_ButtonTrackerStates[Middle] = &m_ButtonStateTracker.middleButton;
	m_ButtonTrackerStates[Right] =	&m_ButtonStateTracker.rightButton;
	m_ButtonTrackerStates[X1] =		&m_ButtonStateTracker.xButton1;
	m_ButtonTrackerStates[X2] =		&m_ButtonStateTracker.xButton2;
}

void MouseUtil::SetWindow(HWND window)
{
	m_Mouse->SetWindow(window);
}

void MouseUtil::Update()
{
	// キーボードステートを取得
	m_State = m_Mouse->GetState();
	// トリガー状態の更新
	m_ButtonStateTracker.Update(m_State);
}

bool MouseUtil::IsConnected() const
{
	//return m_Mouse->IsConnected();
	return true;
}

bool MouseUtil::IsPressed(Button button)
{
	// ボタン番号が範囲内に収まっていることをチェック
	assert( 0 <= button && button < Num );

	return *m_ButtonStates[button];
}

bool MouseUtil::IsTriggered(Button button)
{
	// ボタン番号が範囲内に収まっていることをチェック
	assert(0 <= button && button < Num);

	// トリガーか
	if (*m_ButtonTrackerStates[button] == ButtonState::PRESSED)
	{
		return true;
	}

	return false;
}

bool MouseUtil::IsReleased(Button button)
{
	// ボタン番号が範囲内に収まっていることをチェック
	assert(0 <= button && button < Num);

	// トリガーか
	if (*m_ButtonTrackerStates[button] == ButtonState::RELEASED)
	{
		return true;
	}

	return false;
}

void MouseUtil::SetMode(DirectX::Mouse::Mode mode)
{
	m_Mouse->SetMode(mode);
}

DirectX::Mouse::Mode MouseUtil::GetMode()
{
	return m_State.positionMode;
}

DirectX::XMINT2 MouseUtil::GetPos()
{
	XMINT2 pos;
	pos.x = m_State.x;
	pos.y = m_State.y;

	return pos;
}

int MouseUtil::GetX()
{
	return m_State.x;
}

int MouseUtil::GetY()
{
	return m_State.y;
}

void MouseUtil::ResetWheelValue()
{
	m_Mouse->ResetScrollWheelValue();
}

int MouseUtil::GetWheelValue()
{
	return m_State.scrollWheelValue;
}
