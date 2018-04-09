//
// Game.cpp
//

#include "Game.h"
#include <ctime>
#include <d3d11.h>
#include <Winuser.h>

extern void ExitGame();

using namespace std;
using Microsoft::WRL::ComPtr;
using namespace Scene;


Game::Game() :
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{

	auto& devices = Devices::Get();
	devices.HWnd(window);
	devices.Width(width);
	devices.Height(height);
	// �f�o�C�X11�𐶐����� Create Device
	devices.CreateDevice();
	// ���\�[�X�𐶐����� Create Resources
	devices.CreateResources();
	// �f�o�C�X9�𐶐�����
	devices.CreateDevice9();
	srand(static_cast<unsigned int>(time(nullptr)));
	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);
	//	�ėp�X�e�[�g�𐶐�
	m_states = std::make_unique<CommonStates>(devices.Device().Get());

	//�t�H���[�J�����擾
	camera = FollowCamera::GetInstance();
	//�}�E�X�擾
	mouse = MouseUtil::GetInstance();
	//�L�[�{�[�h�擾
	keyBoard = KeyboardUtil::GetInstance();

	//�����蔻��擾
	collisionManager = CollisionManager::GetInstance();
	collisionManager->SetCollisitonIntervalFrame(COLLISITON_INTERVAL);
	//���C�g�̎擾
	light = Light::GetInstance();


	//�V�[���}�l�[�W���擾����
	sceneManager = make_unique<SceneManager>();
	//�^�C�g���V�[�����쐬
	sceneManager->ReplaceScene(SceneKind::Game);
	sceneManager->LoopCreateAddChild();
	//������
	sceneManager->LoopInitialize();

}



// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{

	auto& device = Devices::Get();
	device.Device9()->ShowCursor(false);
	//�V�[���}�l�[�W���̍X�V(����ŃQ�[�����̍X�V�������S�ČĂ΂��)
	sceneManager->LoopUpdate();
	//�t�H���[�J�����X�V
	camera->Update();
	//�}�E�X�X�V
	mouse->Update();
	//�L�[�{�[�h�X�V
	keyBoard->Update();
	//���C�g�̍X�V
	light->Calc();
	//�����蔻��X�V
	collisionManager->Update();
	auto& devices = Devices::Get();
	//�}�E�X�̍��W���Œ�
	SetCursorPos(devices.Width(), devices.Height());
	//�}�E�X�J�[�\����\��
	ShowCursor(false);

}

// Draws the scene.
void Game::Render()
{
	//�ŏ��̍X�V�O�ɉ����\�����Ȃ��ł�������
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	auto& devices = Devices::Get();


	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));

	devices.Context().Get()->RSSetState(m_states->CullNone());

	//�X�N���[���A�e�ȊO�̕`��
	sceneManager->LoopClearRenderConfig();



	//////�e�̕`��
	//sceneManager->LoopShadowRender();

	//#if 0
	////�A���t�@�l��L���ɂ���
	devices.Context().Get()->OMSetBlendState(m_states->AlphaBlend(), nullptr, 0xffffffff);


	Clear();




	////�`��
	sceneManager->LoopRender();

	Present();
}
//
//// Helper method to clear the back buffers.
void Game::Clear()
{
	auto& devices = Devices::Get();
	

	// Clear the views.
	devices.Context().Get()->OMSetRenderTargets(1, devices.RenderTargetView().GetAddressOf(), devices.DepthStencilView().Get());
	devices.Context().Get()->ClearRenderTargetView(devices.RenderTargetView().Get(), Colors::CornflowerBlue);
	devices.Context().Get()->ClearDepthStencilView(devices.DepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set the viewport.
	CD3D11_VIEWPORT  viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	devices.Context().Get()->RSSetViewports(1, &viewport);

	devices.Viewport(viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	auto& devices = Devices::Get();
	HRESULT hr = devices.SwapChain().Get()->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		devices.OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}
//
void Game::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	Devices::Get().CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

