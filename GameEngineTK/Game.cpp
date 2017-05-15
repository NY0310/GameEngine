//
// Game.cpp
//

#include "pch.h"
#include "Game.h"


extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
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
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
	//初期化はここに書く
	//new = std::make_unique
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());
	//射影行列
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	//デバックカメラを生成
	//m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	//エフェクトファクタリ
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//テクスチャパスをしてい
	m_factory->SetDirectory(L"Resources");
	//モデルの生成
	m_Skydome = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/Skydoome.cmo", *m_factory);
	//モデルの生成
	m_Ground = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/gropund200.cmo", *m_factory);

	m_ball = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/balll.cmo", *m_factory);

	m_Teapot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/Teapot2.cmo", *m_factory);

	m_Head = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/head.cmo", *m_factory);

	//for (int i = 0; i < MAX_GROUND; i++)
	//{

	//	for (int j = 0; j < MAX_GROUND; j++)
	//	{
	//		//ワールド行列を計算
	//		Matrix scalemat = Matrix::CreateScale(1.0f);
	//		////平行移動
	//		Matrix transmat = Matrix::CreateTranslation(i- MAX_GROUND/2, 0.0f, j -MAX_GROUND / 2);
	//		m_worldGround[i][j] = scalemat * transmat;
	//	}
	//}
	//ワールド行列を計算
	Matrix scalemat = Matrix::CreateScale(1.0f);
	////平行移動
	Matrix transmat = Matrix::CreateTranslation( MAX_GROUND / 2, 0.0f, MAX_GROUND / 2);
	m_worldGround = scalemat * transmat;
	m_rotation = 0.0f;

	for (int i = 0; i < 20; i++)
	{
		int a = rand() % 100;
		float b = rand() % 360;
		
		//ワールド行列を計算
		Matrix scalemat = Matrix::CreateScale(1.0f);
		////平行移動
		Matrix transmat = Matrix::CreateTranslation(a, 0.0f, 0.0f);
		rotmaty = Matrix::CreateRotationY(XMConvertToRadians(b + m_rotation));
		m_worldTeapot[i] = scalemat * transmat* rotmaty;

		m_teapotPos[i] = Vector3(m_worldTeapot[i]._41, m_worldTeapot[i]._42, m_worldTeapot[i]._43) ;
	}
	m_scale = 1;


	m_time = 0.0f;

	distance = 1.0f;

	//////////////////頭
	//ワールド行列を計算
	Matrix scalemat2 = Matrix::CreateScale(1.0f);
	////平行移動
	Matrix transmat2 = Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
	rotmaty = Matrix::CreateRotationY(XMConvertToRadians(0.0f));

	m_worldHead = scalemat2 * transmat2* rotmaty;

	//キーボードの生成
	keyboard = std::make_unique<Keyboard>();

	head_lotate = 0;



	//カメラの生成
	m_Camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);

	head_pos = Vector3(0, 0, 30);
}
;
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

static float Lerpcos(float time)
{
	return (1 - cosf(time) * (time * XM_PI)) / 2;
}


static float Lerp(float startPosition, float targetPosition, float t)
{
	float lerpPosition = 0.0f;

	lerpPosition = (1 - Lerpcos(t)) * startPosition + Lerpcos(t) * targetPosition;

	return lerpPosition;
}




// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;
	//毎フレーム処理を追加します
	//m_debugCamera->Update();

	m_rotation += 1.0f;
	//if (m_scale > 6)
	//{
	//	m_scale = 1;
	//}
	////平行移動
	//Matrix transmat = Matrix::CreateTranslation(1.0f,0,0);
	////ロール
	//Matrix rotmatz = Matrix::CreateRotationZ(XMConvertToRadians(15.0f));
	//
	////ヨー　(方位角)
	//Matrix rotmaty = Matrix::CreateRotationZ(XM_PIDIV4);
	////回転行列の合成
	//Matrix rotmat = rotmatz * rotmatx * rotmaty;

	//ワールド行列の合成(SRT)
	for (int i = 1; i <= 2; i++)
	{
		for (int j = 1; j <= MAX_BALL / 2; j++)
		{
			//ワールド行列を計算
			Matrix scalemat = Matrix::CreateScale(1.2f * i);
			////平行移動
			Matrix transmat = Matrix::CreateTranslation(20.0f * i, 0.0f,0.0f);
			//ヨー　(方位角)
			if (i == 2)
			{
				rotmaty = Matrix::CreateRotationY(XMConvertToRadians(36.0f * j + m_rotation * -1.0f));
			}
			else
			{
				rotmaty = Matrix::CreateRotationY(XMConvertToRadians(36.0f * j + m_rotation));
			}
			

			m_worldBall[MAX_BALL / 2 * (i- 1)+j - 1] = scalemat * transmat* rotmaty;
		}
	}


	for (int i = 0; i < 20; i++)
	{
		Matrix transmat = Matrix::CreateTranslation(m_teapotPos[i] * distance);
		////ワールド行列を計算
		Matrix scalemat = Matrix::CreateScale(m_scale);
		//////平行移動
		//Matrix transmat = Matrix::CreateTranslation(a, 0.0f, 0.0f);
		Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(m_rotation));
	
		m_worldTeapot[i] = scalemat *  rotmaty *transmat;
	}

	float Total = float(timer.GetTotalSeconds());


	//timeStep = (Total - m_time) / 10;
	
	//Total += XM_2PI / 180.0f;

	float kakudo;
	kakudo = Total * XM_2PI / 3;
	m_scale = cosf(kakudo) * (5 - 1)/2 + (5 + 1)/2;


	
	if (distance > 0)
	{
		distance -= 1.0f / 600.0f;

	}

	Keyboard::State kb = keyboard->GetState();

	if (kb.W)
	{
		//移動量のベクトル
		Vector3 movesV(0, 0, -0.1f);
		//移動ベクトルを角度回転させる
		movesV = Vector3::TransformNormal(movesV, m_worldHead);


		//自機の座標を移動
		head_pos += movesV;
	}
	if (kb.S)
	{
		//移動量のベクトル
		 Vector3 movesV(0, 0, 0.1f);
		//移動ベクトルを角度回転させる
		movesV = Vector3::TransformNormal(movesV, m_worldHead);
		//自機の座標を移動
		head_pos += movesV;
	}

	if (kb.A)
	{
		head_lotate++;
	}if (kb.D)
	{
		head_lotate--;
	}
	{//自機のワールド行列を計算
	 ////平行移動
		Matrix rotmatz = Matrix::CreateRotationY(XMConvertToRadians(head_lotate));
		Matrix transmat = Matrix::CreateTranslation(head_pos);
		m_worldHead = rotmatz * transmat;
	}
	//if (timeStep < 1.0f)
	//{
	//	m_scale = Lerp(1.0f,5.0f, timeStep);
	//}
	//else if (timeStep < 2.0f)
	//{
	//	m_scale = Lerp(5,1, timeStep - 1.0f);
	//}
	//else
	//{
	//	m_time = delta;
	//}


	//カメラの更新
	/*m_Camera->SetEyePos(head_pos+Vector3(sinf(XMConvertToRadians(head_lotate)),0.1f,cosf(XMConvertToRadians(head_lotate))));
	m_Camera->SetRefPos(head_pos);
	m_Camera->Update();
	m_view = m_Camera->GetViewMatrix();
	m_proj = m_Camera->GetProjectionMatrix()*/
	m_Camera->SetTargetAngle(XMConvertToRadians(head_lotate));
	m_Camera->SetTargetPos(head_pos);
	m_Camera->Update();

	m_view = m_Camera->GetViewMatrix();
	m_proj = m_Camera->GetProjectionMatrix();
}









// Draws the scene.
void Game::Render()
{
	//頂点座標
	uint16_t indices[] = {
		0,1,2,
		2,1,3
	};

	VertexPositionNormal vertices[]=
	{
		{Vector3(-1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f)},
		{ Vector3(-1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) }
	};
    // Don't try to render anything before the first Update.
	//updateを一度でも通ったか
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	//描画処理を書く
	//透明
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//奥行を考慮しない
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//背面カリングを有効にしない
	m_d3dContext->RSSetState(m_states->Wireframe());

	////ビュー行列を生成
	//m_view = Matrix::CreateLookAt(
	//	Vector3(0, 0, 2.f),//カメラ視点
	//	Vector3(0,0,0),//カメラ参照視点
	//    Vector3(1,0,0)
	//);//上方向ベクトル
	//デバックカメラ
	//m_view = m_debugCamera->GetCameraMatrix();

	//ビュー行列
	////視点座標（カメラの位置）
	//Vector3 eyepos(0, 0, 5.0f);
	////参照点/注視点(カメラの見ている先）
	//Vector3 refpos(0, 0, 0);
	////上方向ベクトル
	//Vector3 upvec(0, 1, 0);
//ななめVector3 upvec(1, 1, 0);
//角度を保ったまま長さを1にする	upvec.Normalize();

	////ビュー行列生成
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);


	////プロジェクション行列(射影行列)を計算(どっからどこまで表示するか)
	////垂直方向視野角(縦方向に何度までうつるか)
	//float fovY = XMConvertToRadians(60.0f);
	////アスペクト比(横縦の比率)
	//float aspect = (float)m_outputWidth/ m_outputHeight;
	////ニアクリップ(手前の表示限界距離)
	//float nearClip = 0.1f;
	////ファークリップ(奥の表示限界距離)
	//float farClip = 0.1f;
	////射影行列の生成
	//Matrix projmat = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearClip, farClip);


	//プロジェクション行列を生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		XM_PI / 4.f,//視野角(上下方向)
		float(m_outputWidth) / float(m_outputHeight),//アスペクト比(画面の幅と長さの比率を教える)
		0.1f,//ニアクリップ
		500.f//ファークリップ
	);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//描画をする
	m_batch->Begin();

	//モデルの描画
	m_Skydome->Draw(m_d3dContext.Get(), *m_states, m_world, m_view, m_proj);
	//モデルの描画
	/*m_Ground->Draw(m_d3dContext.Get(), *m_states, m_world, m_view, m_proj);*/
	////モデルの描画
	//for (int i = 0; i < MAX_BALL + 1; i++)
	//{
	//	m_ball->Draw(m_d3dContext.Get(), *m_states, m_worldBall[i], m_view, m_proj);

	//}

	//for (int i = 0; i < 20; i++)
	//{
	//	m_Teapot->Draw(m_d3dContext.Get(), *m_states, m_worldTeapot[i], m_view, m_proj);
	//}


	m_Head->Draw(m_d3dContext.Get(), *m_states, m_worldHead, m_view, m_proj);



	//for (int i = 0; i < MAX_GROUND; i++)
	//{

	//	for (int j = 0; j < MAX_GROUND; j++)
	//	{
	//		m_Ground->Draw(m_d3dContext.Get(), *m_states, m_worldGround[i][j], m_view, m_proj);
	//	}
	//}

	m_Ground->Draw(m_d3dContext.Get(), *m_states, m_worldGround, m_view, m_proj);


	//配列の数、頂点数
	m_batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);
	/*m_batch->DrawLine(
		VertexPositionColor(
		SimpleMath::Vector3(0,0,0), 
		SimpleMath::Color(1,1,1)),
		VertexPositionColor(
			SimpleMath::Vector3(600, 600, 0),
		SimpleMath::Color(1, 1, 1))
	);
	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Pink);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Orange);*/
	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Yellow);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Black);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Green);
	//m_batch->DrawTriangle(v1, v2, v3);

	m_batch->End();


	Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
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

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}