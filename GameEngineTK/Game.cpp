//
// Game.cpp
//

//#pragma comment(lib,"d3dx10.lib")
//#pragma comment(lib,"d3dx11.lib")

#include "Game.h"
#include <ctime>
#include <d3d11.h>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;
using Microsoft::WRL::ComPtr;



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


	// マウスライブラリにウィンドウハンドルを渡す
	MouseUtil::GetInstance()->SetWindow(devices.HWnd());

	// デバイスを生成する Create Device
	devices.CreateDevice();
	// リソースを生成する Create Resources
	devices.CreateResources();



	srand(static_cast<unsigned int>(time(nullptr)));

	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);


	//	初期化はここに書く
	//	キーボードの生成
	keyboard = std::make_unique<Keyboard>();
	//	カメラの生成
	m_Camera = FollowCamera::GetInstance();
	//	カメラにキーボードをセット
	m_Camera->SetKeyboard(keyboard.get());
	//m_spriteFont = std::make_unique<SpriteFont>(devices.Device().Get(), L"Resources/myfile.spritefont");

	//D3D11_RASTERIZER_DESC rdc;
	//ZeroMemory(&rdc, sizeof(rdc));
	//rdc.CullMode = D3D11_CULL_NONE;
	//rdc.FillMode = D3D11_FILL_WIREFRAME;
	//rdc.FrontCounterClockwise = TRUE;
	//ID3D11RasterizerState* m_pRasterizerState;

	//devices.Device().Get()->CreateRasterizerState(&rdc, &m_pRasterizerState);
	//devices.Context().Get()->RSSetState(m_pRasterizerState);


	{//OBj3dのシステム初期化
		Obj3d::CommonDef def;
		def.pCamera = m_Camera;
		def.pDevice = devices.Device().Get();
		def.pDeviceContext = devices.Context().Get();
		//	3Dオブジェクトの静的メンバ変数を初期化
		Obj3d::InitializeCommon(def);
	}


	//土地あたり判定
	LandShapeCommonDef lscDef;
	lscDef.pDevice = devices.Device().Get();
	lscDef.pDeviceContext = devices.Context().Get();
	lscDef.pCamera = m_Camera;
	//土地のあたり判定共通初期化
	LandShape::InitializeCommon(lscDef);


	m_LandShape.Initialize(L"Skydome2", L"Skydome2");
	m_LandShape.SetScale(1.1);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(devices.Context().Get());



	/*void const* shaderByteCode;
	size_t byteCodeLength;*/


	//	汎用ステートを生成	q
	m_states = std::make_unique<CommonStates>(devices.Device().Get());

	devices.Context().Get()->RSSetState(m_states->CullClockwise());


	//	デバッグカメラを生成
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	//	エフェクトファクトリの作成
	m_factory = std::make_unique<EffectFactory>(devices.Device().Get());
	//	テクスチャのパスを指定
	m_factory->SetDirectory(L"Resources");
	//	天球モデルの生成
	//	m_objSkydome.LoadModel(L"Skydome.cmo");



	player = std::make_unique<Player>(keyboard.get());
	player->Initialize();
	//追従カメラにプレイヤーをセット
	m_Camera->SetPlayer(player.get()); 



	clearcnt = 0;


	m_str = L"CLEAR";

	stage.Initialize();


	const int objnum =4;
	D3DXVECTOR3 position[objnum] = {
		D3DXVECTOR3(0,1,-2),
		D3DXVECTOR3(2,2,-2),
		D3DXVECTOR3(-2,0,-2),
		D3DXVECTOR3(-2,2,-2),
	};

	D3DXVECTOR3 rot[objnum] = {
		D3DXVECTOR3(0,0,0),
		D3DXVECTOR3(1,0.6,0.3),
		D3DXVECTOR3(0.5,0,1),
		D3DXVECTOR3(1.4,0,1),
	};
	obj.resize(objnum);

	static int cnt = 0;
for (auto& data : obj)
	{
		data = new PaintObj();
		data->Initialze();
		data->LoadOBJFile("Resources/OBJ/Geometry+Normal+UV.obj");
		data->LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
		data->matrixObject->SetPosition(position[cnt]);
		data->matrixObject->SetRotation(rot[cnt]);
		cnt++;
		}
	shadermanager = ShaderManager::Get();


	////D3DXMESHライブラリを使用するクラス生成
	//m_pMesh = new CD3DXMESH;
	////初期化
	//m_pMesh->Init("Resources/X/RobotA_1motion_2truck.x");
	skinmesh = new CD3DXSKINMESH();
	skinmesh->Initialize();
	skinmesh->CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");

		
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

	// TODO: Add your game logic here.
	MouseUtil::GetInstance()->Update();


	//	毎フレーム処理を書く
	m_debugCamera->Update();

	stage.GetLandShape().Update();

	//	キーボードの取得
	Keyboard::State key = keyboard->GetState();


	////キーボードの様態を取得
	Keyboard::State kb = keyboard->GetState();
	//プレイヤのアップデート関数
	player->Update();

	////キーボードトラッカーの更新
	KeybordTracker.Update(kb);


	m_LandShape.Update();

	if (player->Gethitcnt() == player->GetMAX_HOMING())
	{
		if (key.IsKeyDown(Keyboard::Keys::R))
		{
			player->ResetCnt();
			FireHomingBullets(player->GetTrans());
		}
	}









	//
	{//	自機に追従するカメラ

		m_Camera->Update();
	}



	for (auto& data : obj)
		data->UpDate();
}

// Draws the scene.
void Game::Render()
{

	auto& devices = Devices::Get();



	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));

	//devices.Context().Get()->RSSetState(m_states->CullCounterClockwise());

	devices.Context().Get()->RSSetState(m_states->CullNone());


	//#if 0
	//アルファ値を有効にする
	devices.Context().Get()->OMSetBlendState(m_states->Opaque(), nullptr, 0xffffffff);

	//pSss->ZTexRender(m_Camera);
	for (auto& data : obj)
	data->InkRender();
	//obj->ZTextureRender(m_Camera);
	Clear();
	//pBumpMapping->Render(m_Camera);
	//pSss->Render(m_Camera);


	//for (std::vector<std::unique_ptr<HomingBullet>>::iterator it = m_HomingBullets.begin();
	//	it != m_HomingBullets.end();
	//	it++)
	//{
	//	(*it)->Draw();
	//}


	//stage.GetLandShape().Draw();

	//m_LandShape.Draw();
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::SkyBlue);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::SandyBrown);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::PaleGoldenrod);

	CommonStates states(devices.Device().Get());

	devices.SpriteBatch()->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());


	//// 全パーツ分行列更新
	//for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	//{
	//	Enemy* enemy = it->get();
	//	(*it)->Draw();
	//}


	//player->Draw();

	//if (clearcnt == CLEARNUM)
	//{
	//	m_Enemies.clear();
	//	//スプライトフォントの描画
	// 		m_spriteFont->DrawString(devices.SpriteBatch().get(), m_str.c_str(), XMFLOAT2(200, 200));

	//}



	////キーボードの様態を取得
	//Keyboard::State kb = keyboard->GetState();
	////キーボードトラッカーの更新
	//KeybordTracker.Update(kb);

	//if (kb.D)
	//{
	//	m_pMesh->ChangeAnimSet(1);
	//}




	//tomanageparticle->Render(m_Camera);

	devices.SpriteBatch().get()->End();
	//アルファ値を無効にする
	devices.Context().Get()->OMSetBlendState(m_states->Opaque(), nullptr, 0xffffffff);
	//pTessellation->Render(m_Camera);
	//pDisplacementMapping->Render(m_Camera);
	//skinmesh->m_View = shaderManader.MatrixToD3DXMATRIX(m_Camera->GetView());
	//skinmesh->m_Eye = shaderManader.VectorToD3DXVECTOR3(m_Camera->GetEyePos());
	//skinmesh->m_Proj = shaderManader.MatrixToD3DXMATRIX(m_Camera->GetProjection());
	////再生開始箇所を変えることによりモーションを切り替える
	//if (GetKeyState(VK_F1) & 0x80)//握る
	//{
	//	skinmesh->m_pD3dxMesh->m_pAnimController->SetTrackPosition(0, 0);
	//}
	//if (GetKeyState(VK_F2) & 0x80)//振る
	//{
	//	skinmesh->m_pD3dxMesh->m_pAnimController->SetTrackPosition(0, 1.3);
	//}

	//	skinmesh->Render();

	////D3DXMESHライブラリを使用してXファイルを描画するクラス
//	m_pMesh->Render( D3DXVECTOR3(1, 1, -1));
	/*m_pMesh->GetfYaw() += 0.0002;
	m_pMesh->GetAnimController()->AdvanceTime(0.007, NULL);*/

	for (auto& data : obj)
	data->Render();


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




void Game::FireHomingBullets(const DirectX::SimpleMath::Vector3 pos)
{
	// 敵の数分だけ発射
	unsigned int enemyNum = m_Enemies.size();
	for (unsigned int i = 0; i < enemyNum; i++)
	{
		Enemy* enemy = m_Enemies[i].get();

		// 弾生成
		std::unique_ptr<HomingBullet> bullet = std::make_unique<HomingBullet>();
		bullet->Initialize();
		// 上に発射
		bullet->Fire(pos, Vector3::UnitY);
		bullet->SetTarget(enemy);

		m_HomingBullets.push_back(std::move(bullet));
	}
}


