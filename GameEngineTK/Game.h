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
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "DebugCamera.h"
#include "FollowCamera.h"
#include "Obj3d.h"
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "ModelExpansionEffect.h"
#include "LandShape.h"
#include "Device.h"

#include <d3dCompiler.h>
#include "PARTICLE.h"

// old: D3DX11CompileFromFile

//#include <d3dcompiler.inl>

#pragma comment(lib, "d3dcompiler.lib")


#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ

//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}


//頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
	//D3DXVECTOR2 Tex; //テクスチャー座標
};
//シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
};


//頂点の構造体
struct OBJSimpleVertex
{
	D3DXVECTOR3 Pos; //位置
					 //D3DXVECTOR2 Tex; //テクスチャー座標
};

//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;//ワールドから射影までの変換行列
};


//オリジナルメッシュ
struct MY_MESH
{
	DWORD dwNumVert;
	DWORD dwNumFace;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
};


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

	HRESULT InitD3D();

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

	void D3DXMATRIXToMatrix(D3DXMATRIX & d3dxmatrix);

	void MatrixToD3DXMATRIX(DirectX::SimpleMath::Matrix& matrix);

//	void D3DXMATRIXToMatrix(D3DXMATRIX& d3dxmatrix);

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();
	void Present();

	//void CreateDevice();
	//void CreateResources();

	//void OnDeviceLost();

	// Device resources.
	HWND                                            m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	D3D_FEATURE_LEVEL                               m_featureLevel;
	//Microsoft::WRL::ComPtr<ID3D11Device>            devices.Device.Get();
	//Microsoft::WRL::ComPtr<ID3D11Device1>           devices.Device.Get()1;
	//Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	//Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

	//Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
	//Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
	//Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

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
	LandShape m_LandShape;
	//	球モデル
	std::unique_ptr<DirectX::Model> m_modelBall;
	//	球のワールド行列
	DirectX::SimpleMath::Matrix m_worldBall[20];

	////	頭モデル
	//std::unique_ptr<DirectX::Model> m_modelHead;

	//	回転を加えるための行列
	DirectX::SimpleMath::Matrix m_rot;
	//	逆回転を加えるための行列
	DirectX::SimpleMath::Matrix m_Rrot;

	//	ティーポットのスケールを変化させるための行列
	DirectX::SimpleMath::Matrix m_Scale;

	//	キーボード
	std::unique_ptr<DirectX::Keyboard> keyboard;
	//	自機の座標
	DirectX::SimpleMath::Vector3 head_pos;

	//	自機の回転角
	float head_rot;

	//	自機のワールド行列1
	DirectX::SimpleMath::Matrix head_world;
	//	自機のワールド行列2
	DirectX::SimpleMath::Matrix head_world2;

	//	自機パーツ
	std::vector<Obj3d> m_Obj;
	//	サイン用の角度
	float m_cycle;

	//	カメラ
	std::unique_ptr<FollowCamera> m_Camera;

	// 現在のカメラ
	Camera* m_CurrentCamera;

	////キーボードトラッカー
	//DirectX::Keyboard::KeyboardStateTracker KeybordTracker;
	std::unique_ptr<Player> player;

	//敵
	std::vector<std::unique_ptr<Enemy>> m_Enemies;

	ModelExpansionEffect* Effect;


	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;


	//文字列
	std::wstring m_str;


	//IDXGISwapChain* m_pSwapChain;
	//ID3D11RenderTargetView* m_pBackBuffer_TexRTV;

	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* m_pVertexLayout;

	ID3D11GeometryShader* m_pGeometryShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11SamplerState* m_pSampler;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11BlendState* m_pBlendState;
	ID3D11DepthStencilState* m_DepthStencilState;


	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;



	ID3D11Buffer* m_OBJpConstantBuffer;
	MY_MESH m_Mesh;

	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);

	HRESULT InitModel();

	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);

	void RenderSprite(D3DXMATRIX & WVP);


	
	std::vector<PARTICLE*> m_pParticle;
	int MAX_PARTICLE = 300;

};       
