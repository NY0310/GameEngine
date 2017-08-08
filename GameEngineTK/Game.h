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

#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����


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

	//	�����f��
	std::unique_ptr<DirectX::Model> m_modelBall;


	//	�L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> keyboard;


	//	�J����
	std::unique_ptr<FollowCamera> m_Camera;

	// ���݂̃J����
	Camera* m_CurrentCamera;

	////�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker KeybordTracker;
	std::unique_ptr<Player> player;

	//�G
	std::vector<std::unique_ptr<Enemy>> m_Enemies;



	//�X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;


	//������
	std::wstring m_str;




	Stage stage;


	// �z�[�~���O�e
	std::vector<std::unique_ptr<HomingBullet>> m_HomingBullets;
	LandShape m_LandShape;




	int clearcnt;
	int CLEARNUM = 15;

	ToManageParticle* tomanageparticle;
	OBJ* obj;
	ShaderManager shadermanager;//�V�F�[�_�[�֘A

};       
