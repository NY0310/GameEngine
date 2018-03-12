#include "KeyboardUtil.h"

using namespace DirectX;
using namespace std;
using namespace NYLibrary;


// static member variable
unique_ptr<KeyboardUtil> KeyboardUtil::m_Instance;

KeyboardUtil * KeyboardUtil::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = make_unique<KeyboardUtil>();
	}

	return m_Instance.get();
}

KeyboardUtil::KeyboardUtil()
{
	m_Keyboard = make_unique<Keyboard>();
}

void KeyboardUtil::Update()
{
	// キーボードステートを取得
	m_State = m_Keyboard->GetState();
	// トリガー状態の更新
	m_KeyboardTracker.Update(m_State);
}

bool KeyboardUtil::IsPressed(DirectX::Keyboard::Keys key)
{
	// ステートのキー押下情報をチェック
	return m_State.IsKeyDown(key);
}

bool KeyboardUtil::IsTriggered(DirectX::Keyboard::Keys key)
{
	// トラッカーからトリガー情報をチェック
	return m_KeyboardTracker.IsKeyPressed(key);
}

bool KeyboardUtil::IsReleased(DirectX::Keyboard::Keys key)
{
	// トラッカーからリリース情報をチェック
	return m_KeyboardTracker.IsKeyReleased(key);
}
