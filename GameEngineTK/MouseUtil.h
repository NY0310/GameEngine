#pragma once

#include <windows.h>
#include <Mouse.h>
#include <DirectXMath.h>

// マウスユーティリティクラス
class MouseUtil
{
public:
	// ボタン
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
	// コンストラクタ
	MouseUtil(HWND window, Mode mode = Mode::MODE_ABSOLUTE);
	// 毎フレーム更新
	void Update();
	//// マウス接続チェック
	//bool IsConnected() const;
	// 指定ボタンが押されているか
	bool IsPressed(Button button);
	// 指定ボタンのトリガーチェック
	bool IsTriggered(Button button);
	// 指定ボタンのリリースチェック
	bool IsReleased(Button button);
	// モードセット
	void SetMode(Mode mode);
	// モード取得
	Mode GetMode();
	// マウス座標の取得
	DirectX::XMINT2 GetPos();
	// Ｘ座標の取得
	int GetX();
	// Ｙ座標の取得
	int GetY();
	// ホイール値のリセット
	void ResetWheelValue();
	// ホイール値の取得
	int GetWheelValue();
protected:
	// マウス
	std::unique_ptr<DirectX::Mouse> m_Mouse;
	// ステート
	DirectX::Mouse::State m_State;
	// ボタンステートトラッカー
	DirectX::Mouse::ButtonStateTracker m_ButtonStateTracker;
	// ボタン押下状態
	bool* m_ButtonStates[Button::Num];
	// ボタントリガー状態
	DirectX::Mouse::ButtonStateTracker::ButtonState* m_ButtonTrackerStates[Button::Num];
};