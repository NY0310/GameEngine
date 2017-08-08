//
// Game.h
//

#pragma once

#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include "StepTimer.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Keyboard.h>
#include <SpriteFont.h>
#include "DebugCamera.h"
#include "FollowCamera.h"
#include "Obj3d.h"
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "ModelExpansionEffect.h"
#include "Device.h"

#include <d3dCompiler.h>
#include "ToManageParticle.h"
#include "Stage.h";
#include "HomingBullet.h"
#include "OBJ.h"
// old: D3DX11CompileFromFile

//#include <d3dcompiler.inl>

#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ


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

	DirectX::SimpleMath::Matrix D3DXMATRIXToMatrix(D3DXMATRIX  d3dxmatrix);

	void FireHomingBullets(const DirectX::SimpleMath::Vector3 pos);
private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();
	void Present();
	HWND                                            m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	D3D_FEATURE_LEVEL                               m_featureLevel;

	//デバイス
	Devices* m_devices;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	//	エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	汎用ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;

	//	ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//	ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//	プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;

	//	デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugCamera;

	//	エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//	天球モデル
	Obj3d m_objSkydome;

	//	球モデル
	std::unique_ptr<DirectX::Model> m_modelBall;


	//	キーボード
	std::unique_ptr<DirectX::Keyboard> keyboard;


	//	カメラ
	std::unique_ptr<FollowCamera> m_Camera;

	// 現在のカメラ
	Camera* m_CurrentCamera;

	////キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker KeybordTracker;
	std::unique_ptr<Player> player;

	//敵
	std::vector<std::unique_ptr<Enemy>> m_Enemies;



	//スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;


	//文字列
	std::wstring m_str;




	Stage stage;


	// ホーミング弾
	std::vector<std::unique_ptr<HomingBullet>> m_HomingBullets;
	LandShape m_LandShape;




	int clearcnt;
	int CLEARNUM = 15;

	ToManageParticle* tomanageparticle;
	OBJ* obj;
	ShaderManager shadermanager;//シェーダー関連

};       
