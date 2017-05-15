//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "DebugCamera.h"
#include <Model.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include  "Camera.h"
#include "FollowCamera.h"
const static int MAX_BALL = 20;
const static int MAX_GROUND = 10;

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

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	//汎用ステイト設定
	std::unique_ptr<DirectX::CommonStates> m_states;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	//エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//プロジェクション行列、射影行列
	DirectX::SimpleMath::Matrix m_proj;

	//デバックカメラ
	//std::unique_ptr<DebugCamera> m_debugCamera;

	//エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory>m_factory;
	//天球
	std::unique_ptr<DirectX::Model>m_Skydome;
	//地面
	std::unique_ptr<DirectX::Model>m_Ground;
	//球
	std::unique_ptr<DirectX::Model>m_ball;
	//ティーポット
	std::unique_ptr<DirectX::Model>m_Teapot;
	//頭
	std::unique_ptr<DirectX::Model>m_Head;



	////球ワールド
	DirectX::SimpleMath::Matrix m_worldBall[MAX_BALL + 1];
	//球ワールド
	DirectX::SimpleMath::Matrix m_worldGround;
	//ティーポットワールド
	DirectX::SimpleMath::Matrix m_worldTeapot[20];
	//頭ワールド
	DirectX::SimpleMath::Matrix m_worldHead;
	////床ワールド
	//DirectX::SimpleMath::Matrix m_worldGround[MAX_GROUND][MAX_GROUND];

	//ティーポット初期座標
	DirectX::SimpleMath::Vector3 m_teapotPos[20];
	float m_rotation;
	float m_scale;
	DirectX::SimpleMath::Matrix rotmaty;

	float timeStep;
	float m_time;
	DX::StepTimer s_timer;

	float distance;


	std::unique_ptr<DirectX::Keyboard> keyboard;
	//自機のざひょう
	DirectX::SimpleMath::Vector3 head_pos;

	float head_lotate;
//	float m_pos[20];

	//カメラ距離
	const float CAMERA_DISTANCE = 5.0f;

	//カメラ
	std::unique_ptr<FollowCamera>m_Camera;
};