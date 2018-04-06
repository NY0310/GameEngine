//
// Game.h
//

#pragma once

#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <CommonStates.h>
#include <memory>

#include "../NYLibrary/Device/Device.h"
#include "StepTimer.h"
#include "../Scene/SceneManager.h"
#include "../NYLibrary/Camera/FollowCamera.h"
#include "../NYLibrary/Input/MouseUtil.h"
#include "../NYLibrary/Input/KeyboardUtil.h"
#include "../NYLibrary/Collision/CollisionManager.h"
#include "../NYLibrary/Light/Light.h"
#include "../NYLibrary/ShadowMap/ShadowMap.h"

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")


namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DirectX API errors
			throw std::exception();
		}
	}
}

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	Game();
	~Game() = default;
	// Initialization and management
	void Initialize(HWND window, int width, int height);


	// Basic game loop
	void Tick();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize( int& width, int& height ) const;
private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();
	void Present();
	HWND m_window;
	int  m_outputWidth;
	int  m_outputHeight;
					  
	D3D_FEATURE_LEVEL  m_featureLevel;


	// Rendering loop timer.
	DX::StepTimer                          m_timer;	//	汎用ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;


	//シーンマネージャ
	std::shared_ptr<Scene::SceneManager> sceneManager;
	NYLibrary::FollowCamera* camera;
	NYLibrary::MouseUtil* mouse;
	NYLibrary::KeyboardUtil* keyBoard;
	NYLibrary::CollisionManager* collisionManager;
	NYLibrary::Light* light;
};       
