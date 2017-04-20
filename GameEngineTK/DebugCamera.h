#pragma once
// デバッグ用カメラクラス
#include <d3d11.h>
#include <SimpleMath.h>
#include <Mouse.h>

// デバッグ用カメラクラス
class DebugCamera
{
	// カメラの距離
	static const float DEFAULT_CAMERA_DISTANCE;

	// マウス関係
	DirectX::Mouse::State mouseState;
	std::unique_ptr<DirectX::Mouse> mouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mouseTracker;

	// 横回転
	float m_yAngle, m_yTmp;

	// 縦回転
	float m_xAngle, m_xTmp;

	// ドラッグされた座標
	int m_x, m_y;

	float m_sx, m_sy;

	// 生成されたビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// スクロールフォイール値
	int m_scrollWheelValue;

private:

	void Motion(int x, int y);

public:
	// コンストラクタ
	DebugCamera(int w, int h);

	// 更新
	void Update();

	DirectX::SimpleMath::Matrix GetCameraMatrix()
	{
		return m_view;
	}
};

