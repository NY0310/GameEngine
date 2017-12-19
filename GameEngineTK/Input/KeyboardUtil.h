﻿#pragma once

#include <WinSDKVer.h>
#include <Keyboard.h>

namespace MyLibrary
{
	// キーボードユーティリティクラス
	class KeyboardUtil
	{
	public:
		static KeyboardUtil* GetInstance();
	private:
		static std::unique_ptr<KeyboardUtil> m_Instance;
	public:
		// コンストラクタ
		KeyboardUtil();
		// 毎フレーム更新
		void Update();
		// 指定キーが押されているか
		bool IsPressed(DirectX::Keyboard::Keys key);
		// 指定キーのトリガーチェック
		bool IsTriggered(DirectX::Keyboard::Keys key);
		// 指定キーのリリースチェック
		bool IsReleased(DirectX::Keyboard::Keys key);
	protected:
		// キーボード
		std::unique_ptr<DirectX::Keyboard> m_Keyboard;
		// ステート
		DirectX::Keyboard::State m_State;
		// キーボードトラッカー
		DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;
	};
}