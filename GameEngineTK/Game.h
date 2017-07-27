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


#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}


//���_�̍\����
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
	//D3DXVECTOR2 Tex; //�e�N�X�`���[���W
};
//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
};


//���_�̍\����
struct OBJSimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
					 //D3DXVECTOR2 Tex; //�e�N�X�`���[���W
};

//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
};


//�I���W�i�����b�V��
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

	//�f�o�C�X
	Devices* m_devices;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	//	�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�ėp�X�e�[�g�ݒ�
	std::unique_ptr<DirectX::CommonStates> m_states;

	//	���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//	�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//	�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;

	//	�f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debugCamera;

	//	�G�t�F�N�g�t�@�N�g��
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//	�V�����f��
	Obj3d m_objSkydome;
	LandShape m_LandShape;
	//	�����f��
	std::unique_ptr<DirectX::Model> m_modelBall;
	//	���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldBall[20];

	////	�����f��
	//std::unique_ptr<DirectX::Model> m_modelHead;

	//	��]�������邽�߂̍s��
	DirectX::SimpleMath::Matrix m_rot;
	//	�t��]�������邽�߂̍s��
	DirectX::SimpleMath::Matrix m_Rrot;

	//	�e�B�[�|�b�g�̃X�P�[����ω������邽�߂̍s��
	DirectX::SimpleMath::Matrix m_Scale;

	//	�L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> keyboard;
	//	���@�̍��W
	DirectX::SimpleMath::Vector3 head_pos;

	//	���@�̉�]�p
	float head_rot;

	//	���@�̃��[���h�s��1
	DirectX::SimpleMath::Matrix head_world;
	//	���@�̃��[���h�s��2
	DirectX::SimpleMath::Matrix head_world2;

	//	���@�p�[�c
	std::vector<Obj3d> m_Obj;
	//	�T�C���p�̊p�x
	float m_cycle;

	//	�J����
	std::unique_ptr<FollowCamera> m_Camera;

	// ���݂̃J����
	Camera* m_CurrentCamera;

	////�L�[�{�[�h�g���b�J�[
	//DirectX::Keyboard::KeyboardStateTracker KeybordTracker;
	std::unique_ptr<Player> player;

	//�G
	std::vector<std::unique_ptr<Enemy>> m_Enemies;

	ModelExpansionEffect* Effect;


	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//�X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;


	//������
	std::wstring m_str;


	//IDXGISwapChain* m_pSwapChain;
	//ID3D11RenderTargetView* m_pBackBuffer_TexRTV;

	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* m_pVertexLayout;

	ID3D11GeometryShader* m_pGeometryShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11SamplerState* m_pSampler;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
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
