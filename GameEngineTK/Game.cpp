//
// Game.cpp
//

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")

#include "Game.h"
#include <ctime>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;
using Microsoft::WRL::ComPtr;




Game::Game(): 
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
	m_Camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	//	カメラにキーボードをセット
	m_Camera->SetKeyboard(keyboard.get());
	m_spriteFont = std::make_unique<SpriteFont>(devices.Device().Get(), L"Resources/myfile.spritefont");


	{//OBj3dのシステム初期化
		Obj3d::CommonDef def;
		def.pCamera = m_Camera.get();
		def.pDevice = devices.Device().Get();
		def.pDeviceContext = devices.Context().Get();
	 //	3Dオブジェクトの静的メンバ変数を初期化
		Obj3d::InitializeCommon(def);
	}
	

	//土地あたり判定
	LandShapeCommonDef lscDef;
	lscDef.pDevice = devices.Device().Get();
	lscDef.pDeviceContext = devices.Context().Get();
	lscDef.pCamera = m_Camera.get();
	//土地のあたり判定共通初期化
	LandShape::InitializeCommon(lscDef);


	m_LandShape.Initialize(L"Skydome2", L"Skydome2");
	m_LandShape.SetScale(1.1);
	
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(devices.Context().Get());


	m_effect = std::make_unique<BasicEffect>(devices.Device().Get());

	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1.0f));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		devices.Device().Get()->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

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

	//敵の生成
	int enemyNum = 5;

	m_Enemies.resize(enemyNum);

	for (int i = 0; i < enemyNum; i++)
	{
		m_Enemies[i] = std::make_unique<Enemy>(keyboard.get());
		//m_Enemies[i]->Initialize();
	}


	clearcnt = 0;
	

	m_str = L"CLEAR";

	
	this->InitD3D();
	this->InitModel();


	//article = new PARTICLE(MAX_PARTICLE, VectorToD3DXVECTOR3(Vector3(0.0f, 0.0f, 0.0f)));

}


HRESULT Game::InitD3D()
{
	
	auto& devices = Devices::Get();

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	D3D11_BLEND_DESC bd;
	//バーテックスシェーダー作成
	if (FAILED(MakeShader("Shader.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader))) return E_FAIL;


	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	SAFE_RELEASE(pCompiledShader);


	////ジオメトリシェーダー作成
	if (FAILED(MakeShader("Shader.hlsl", "GS_Point", "gs_5_0", (void**)&m_pGeometryShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);



	//ピクセルシェーダー作成
	if (FAILED(MakeShader("Shader.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);



	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}


	//深度テストを”無効”にする
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = false;

	devices.Device().Get()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
	devices.Context().Get()->OMSetDepthStencilState(m_DepthStencilState, 1);

	//アルファブレンド用ブレンドステート作成
	//pngファイル内にアルファ情報がある。アルファにより透過するよう指定している
	
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(devices.Device().Get()->CreateBlendState(&bd, &m_pBlendState)))
	{
		return E_FAIL;
	}

	UINT mask = 0xffffffff;
	devices.Context().Get()->OMSetBlendState(m_pBlendState, NULL, mask);






	///////////////////OBJフォーマット//////////////////////////////////

	////hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	//ID3DBlob *pCompiledShader2 = NULL;
	//ID3DBlob *pErrors = NULL;
	////ブロブからバーテックスシェーダー作成
	//if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	//{
	//	MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
	//	return E_FAIL;
	//}
	//SAFE_RELEASE(pErrors);

	//if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader2->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	//{
	//	SAFE_RELEASE(pCompiledShader2);
	//	MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
	//	return E_FAIL;
	//}

	////頂点インプットレイアウトを定義	
	//D3D11_INPUT_ELEMENT_DESC layout2[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//numElements = sizeof(layout2) / sizeof(layout2[0]);
	////頂点インプットレイアウトを作成
	//if (FAILED(devices.Device().Get()->CreateInputLayout(layout2, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	//{
	//	return FALSE;
	//}
	////ブロブからピクセルシェーダー作成
	//if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "OBJ_PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	//{
	//	MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
	//	return E_FAIL;
	//}
	//SAFE_RELEASE(pErrors);
	//if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	//{
	//	SAFE_RELEASE(pCompiledShader);
	//	MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
	//	return E_FAIL;
	//}
	//SAFE_RELEASE(pCompiledShader);
	////コンスタントバッファー作成
	//D3D11_BUFFER_DESC cb2;
	//cb2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cb2.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	//cb2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cb2.MiscFlags = 0;
	//cb2.Usage = D3D11_USAGE_DYNAMIC;

	//if (FAILED(devices.Device().Get()->CreateBuffer(&cb2, NULL, &m_OBJpConstantBuffer)))
	//{
	//	return E_FAIL;
	//}


	////メッシュ作成
	//if (FAILED(InitStaticMesh("Geometry.obj", &m_Mesh)))
	//{
	//	return E_FAIL;
	//}
	stage.Initialize();

	return S_OK;

}




//HRESULT MAIN::MakeShader(LPSTR szFileName,LPSTR szFuncName,LPSTR szProfileName,void** ppShader,ID3DBlob** ppBlob)
//hlslファイルを読み込みシェーダーを作成する
HRESULT Game::MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
{
	
	auto& devices = Devices::Get();


	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		if (FAILED(devices.Device().Get()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		if (FAILED(devices.Device().Get()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		if (FAILED(devices.Device().Get()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		if (FAILED(devices.Device().Get()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		if (FAILED(devices.Device().Get()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		if (FAILED(devices.Device().Get()->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader))) return E_FAIL;
	}


	return S_OK;
}


//
//HRESULT MAIN::InitModel()
//ポリゴン、メッシュなどのジオメトリ関連を初期化（ここでは”点”用）
HRESULT Game::InitModel()
{
	auto& devices = Devices::Get();


	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, &m_pSampler);
	//テクスチャー読み込み
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), L"particle.png", NULL, NULL, &m_pTexture, NULL)))
	{
		MessageBoxA(0, "テクスチャーを読み込めません", "", MB_OK);
		return E_FAIL;
	}


	return S_OK;
}


HRESULT Game::InitStaticMesh(LPSTR FileName, MY_MESH* pMesh)
{

	auto& devices = Devices::Get();


	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	char key[190] = { 0 };
	//ファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//まずは頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//頂点
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}
	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	D3DXVECTOR3* pvVertexBuffer = new D3DXVECTOR3[pMesh->dwNumVert];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

													  //本読み込み
	fseek(fp, SEEK_SET, 0);
	DWORD dwVCount = 0;//読み込みカウンター
	DWORD dwFCount = 0;//読み込みカウンター
	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));
		//頂点 読み込み
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvVertexBuffer[dwVCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			pvVertexBuffer[dwVCount].y = y;
			pvVertexBuffer[dwVCount].z = z;
			dwVCount++;
		}
		//フェイス（ポリゴン） 読み込み
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d// %d// %d//", &v1, &v2, &v3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			dwFCount++;
		}

	}

	fclose(fp);
	//バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(D3DXVECTOR3) *pMesh->dwNumVert * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//インデックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;

	//一時的な入れ物は、もはや不要
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}




void Game::RenderSprite(D3DXMATRIX& WVP)
{
	auto& devices = Devices::Get();


	//使用するシェーダーのセット
	devices.Context().Get()->VSSetShader(m_pVertexShader, NULL, 0);
	devices.Context().Get()->GSSetShader(m_pGeometryShader, NULL, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, NULL, 0);


	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = WVP;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
	}


	//このコンスタントバッファーをどのシェーダーで使うか
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);


	//頂点インプットレイアウトをセット
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//テクスチャーをシェーダーに渡す
	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampler);
	devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);	//プリミティブをレンダリング
	
	//devices.Context().Get()->RSSetState(m_states->CullNone());
	
	devices.Context().Get()->Draw(1, 0);



	for (vector<PARTICLE*>::iterator iterator = m_pParticle.begin(); iterator !=  m_pParticle.end(); iterator++)
	{
		if ((*iterator) != nullptr)
		(*iterator)->Run();
	}
	

	//article->Run();
	
	


	devices.Context().Get()->VSSetShader(NULL, NULL, 0);
	devices.Context().Get()->GSSetShader(NULL, NULL, 0);
	devices.Context().Get()->PSSetShader(NULL, NULL, 0);

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

	{//弾丸と敵のあたり判定
		for (std::vector<SphereNode*>::iterator Sphereit = player->GetCollisionNodeBullet().begin(); Sphereit != player->GetCollisionNodeBullet().end(); )
		{
			if (m_Enemies.size() == 0)
			{
				break;
			}
			for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); )
			{

				Enemy* enemy = it->get();
				//敵の判定球を取得
				const Sphere& enemySphere = enemy->GetCollisionNodeBody();

				//二つの球が当たっていたら
				if (CheckSpere2Sphere(*(*Sphereit), enemySphere))
				{
					(*it)->SetHp((*it)->GetHp() - 10);

					//当たった弾を消す
					Sphereit = player->GetCollisionNodeBullet().erase(Sphereit);
			

					if(player->Gethitcnt() != player->GetMAX_HOMING())
					player->Sethitcnt(player->Gethitcnt() + 1);
					//hpがなくなったら敵を殺す
					if ((*it)->GetHp() <= 0)
					{
						//パーティクル
						Vector3 enemypos = (*it)->GetTrans();
						enemypos.y += 1;
						m_pParticle.push_back(new PARTICLE(MAX_PARTICLE, VectorToD3DXVECTOR3(enemypos)));
						m_Particlecnt.push_back(0);


						//削除する敵をターゲットにしているホーミング弾を削除	
						for (std::vector<std::unique_ptr<HomingBullet>>::iterator ithoming = m_HomingBullets.begin();
							ithoming != m_HomingBullets.end();)
						{
							if ((*it).get() == (*ithoming)->GetEnemy())
							{
								ithoming = m_HomingBullets.erase(ithoming);
							}
							else
							{
								ithoming++;
							}
						}

						it = m_Enemies.erase(it);
		
			
					
						m_Enemies.push_back(move(std::make_unique<Enemy>(keyboard.get())));
						clearcnt++;
					}
					break;
					
				}
				else
				{
					//消さなかったので
					it++;
					//何にも当たらなかったので消す
					if (m_Enemies.end() == it)
					{
							//球を消したのでループから抜ける
  							Sphereit++;
					}
				}
			}
		}

	}




	{//敵弾丸とプレイヤのあたり判定
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end();it++)
		{
			for (std::vector<SphereNode*>::iterator Sphereit = (*it)->GetCollisionNodeBullet().begin(); Sphereit != (*it)->GetCollisionNodeBullet().end(); )
			{
					//敵の判定球を取得
					const Sphere& playerSphere = player->GetCollisionNodeBody();

					//二つの球が当たっていたら
					if (CheckSpere2Sphere(*(*Sphereit), playerSphere))

					{
						player->SetHp(player->GetHp() - 10);

						//当たった弾を消す
						Sphereit = (*it)->GetCollisionNodeBullet().erase(Sphereit);
				
						//hpがなくなったら敵を殺す
						if (player->GetHp() <= 0)
						{
							//パーティクル
							Vector3 playerpos = player->GetTrans();
							playerpos.y += 1;
							m_pParticle.push_back(new PARTICLE(MAX_PARTICLE, VectorToD3DXVECTOR3(playerpos)));
							m_Particlecnt.push_back(0);
						}

					}
					else
					{
							//球を消したのでループから抜ける
							Sphereit++;
					}

			}

		}

	}






	
	//
	{//	自機に追従するカメラ
		
		m_Camera->Update();
		m_view = m_Camera->GetView();
		m_proj = m_Camera->GetProjection();
	}

	//m_objSkydome.Update();

	//for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	//{
	//	it->Update();
	//}


	std::vector<PARTICLE*>::iterator partecle = m_pParticle.begin();



	std::vector<int> veccnt;
	std::vector<int>::iterator cntit;


	int cnt = 0;
	for (std::vector<int>::iterator it = m_Particlecnt.begin(); it != m_Particlecnt.end(); )
	{
		(*it)++;

		if ((*it) == 90)
		{	 
			partecle = m_pParticle.erase(partecle);
			it = m_Particlecnt.erase(it);
		}
		else
		{
			partecle++;
			it++;
		}
	}

	for (std::vector<int>::iterator it = veccnt.begin(); it != veccnt.end(); it++) {
		m_pParticle[(*it)]->Run();
		
	}


	const Vector3 vel = player->GetVelocity();
	if (vel.y <= 0)
	{//自機が地面に乗る処理
	 //自機の頭から足元への線分
		Segment player_segment;
		//自機のワールド座標
		Vector3 trans = player->GetTrans();
		player_segment.Start = trans + Vector3(0, 1, 0);
		player_segment.End = trans + Vector3(0, -0.5f, 0);

		//交点座標
		Vector3 inter;

		//地形と線分のあたり判定(レイキャスト Ray)
		if (stage.GetLandShape().IntersectSegment(player_segment, &inter))
		{
			trans.y = inter.y;
			//落下を終了
			player->StopJump();
		}
		else
		{
			player->StartFall();
		}

		//自機を移動
		player->SetTrans(trans);
		//ワールド行列の更新
	//	player->Calc();
	}



	//自機の天球へのめりこみを解消する
	{
		Sphere sphere = player->GetCollisionNodeBody();

		//自機のワールド座標
		Vector3 trans = player->GetTrans();

		Vector3 sphere2player = trans - sphere.Center;
		//めりこみ排斥ベクトル
		Vector3 reject;

		if (m_LandShape.IntersectSphere(sphere, &reject))
		{
			sphere.Center += reject;

		}
		
			player->SetTrans(sphere.Center + sphere2player);



	}


	//敵の天球へのめりこみを解消する

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)

	{
		Vector3 pos = (*it)->GetTrans();
		pos.x = abs(pos.x);
		pos.y = abs(pos.y);
		pos.z = abs(pos.z);

		if ((pos.x >= 75) || (pos.z >= 75))
		{

			//自機の天球へのめりこみを解消する
			{
				Sphere sphere = (*it)->GetCollisionNodeBody();

				//自機のワールド座標
				Vector3 trans = (*it)->GetTrans();

				Vector3 sphere2player = trans - sphere.Center;
				//めりこみ排斥ベクトル
				Vector3 reject;

				if (m_LandShape.IntersectSphere(sphere, &reject))
				{
					
					
					//削除する敵をターゲットにしているホーミング弾を削除	
					for (std::vector<std::unique_ptr<HomingBullet>>::iterator ithoming = m_HomingBullets.begin();
						ithoming != m_HomingBullets.end();)
					{
						if ((*it).get() == (*ithoming)->GetEnemy())
						{
							ithoming = m_HomingBullets.erase(ithoming);
						}
						else
						{
							ithoming++;
						}
					}
					it = m_Enemies.erase(it);
					m_Enemies.push_back(move(std::make_unique<Enemy>(keyboard.get())));

				}
				else
				{
					//it++;
				}


			}


		}
		else
		{
		//	it++;
		}
	}



	for (std::vector<std::unique_ptr<HomingBullet>>::iterator it = m_HomingBullets.begin();
		it != m_HomingBullets.end();
		)
	{
		if ((*it)->Update())
		{
			
			it = m_HomingBullets.erase(it);

		}
		else
		{
			it++;
		}
	}
	

	// 全パーツ分行列更新
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	{
		Enemy* enemy = it->get();
		(*it)->Update();
	}



}

// Draws the scene.
void Game::Render()
{

	auto& devices = Devices::Get();
	

	uint16_t indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	VertexPositionNormal vertices[] =
	{
		//	座標
		{ Vector3(-1.0f, +1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
		{ Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
		{ Vector3(+1.0f, +1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
		{ Vector3(+1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
	};

	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

//#if 0
	for (std::vector<std::unique_ptr<HomingBullet>>::iterator it = m_HomingBullets.begin();
		it != m_HomingBullets.end();
		it++)
	{
		(*it)->Draw();
	}


	// TODO: Add your rendering code here.
	//	描画処理を書く
	//m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//m_d3dContext->RSSetState(m_states->Wireframe());

	//	ビュー行列を生成
	//m_view = Matrix::CreateLookAt(
	//	Vector3(0.f, 0.f, 2.f),							//	カメラ視点
	//	Vector3(0, 0, 0),								//	カメラ参照点
	//	Vector3(0, 1, 0)								//	画面上方向ベクトル
	//);

	//	デバッグカメラからビュー行列を取得
	//m_view = m_debugCamera->GetCameraMatrix();

	////	カメラの位置(視点座標）
	//Vector3 eyepos(0, 0, 5);
	////	カメラの見ている先(注視点/参照点)
	//Vector3 refpos(0, 0, 0);
	////	カメラの上方向ベクトル
	//static float angle = 0.0f;
	//angle += 0.1f;
	//Vector3 upvec(cosf(angle), sinf(angle), 0);
	//upvec.Normalize();
	////	ビュー行列の生成
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);

	////	プロジェクション行列を生成
	////	垂直方向視野角
	//float fovY = XMConvertToRadians(60.0f);
	////	アスペクト比(横縦の比率）
	//float aspect = (float)m_outputWidth / m_outputHeight;
	////	ニアクリップ(手前の表示限界距離)
	//float nearclip = 0.1f;
	////	ファークリップ(奥の表示限界距離)
	//float farclip = 10.0f;

	//m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearclip, farclip);

	//m_proj = Matrix::CreatePerspectiveFieldOfView(
	//	XM_PI / 4.f,									//	視野角(上下方向）
	//	float(m_outputWidth) / float(m_outputHeight),	//	アスペクト比
	//	0.1f,											//	ニアクリップ
	//	500.f);											//	ファークリップ

	//m_effect->SetView(m_view);
	//m_effect->SetProjection(m_proj);

	//m_effect->Apply(m_d3dContext.Get());
	//m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//	天球モデルの描画
	//m_objSkydome.Draw();

	//for (int i = 0; i < 40000; i++)
	//{
	//	Matrix matrix = Matrix::CreateTranslation(i / 200 - 100, 0.0f, i % 200 - 100);

	//	//	地面モデルの描画
	//	m_modelGround->Draw(m_d3dContext.Get(), *m_states, matrix, m_view, m_proj);
	//}

	////	地面モデルの描画
	//m_modelGround->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	stage.GetLandShape().Draw();

	m_LandShape.Draw();
	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::SkyBlue);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::SandyBrown);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::PaleGoldenrod);
	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::SkyBlue);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::SandyBrown);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::PaleGoldenrod);

	//m_batch->DrawTriangle(v1, v2, v3);

	//m_batch->End();
	CommonStates states(devices.Device().Get());

	devices.SpriteBatch()->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());


	// 全パーツ分行列更新
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	{
		Enemy* enemy = it->get();
		(*it)->Draw();


	}

	player->Draw();

	if (clearcnt == CLEARNUM)
	{
		m_Enemies.clear();
		//スプライトフォントの描画
  		m_spriteFont->DrawString(devices.SpriteBatch().get(), m_str.c_str(), XMFLOAT2(200, 200));

	}

	devices.SpriteBatch().get()->End();
	
	//m_spriteBatch->End();
//#endif
	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, x, 0, 0);
	World = Tran;


	if (KeybordTracker.IsKeyPressed(Keyboard::Keys::Z))
	{
		a+= 0.1f;
	}


	////// ビュートランスフォーム
	//D3DXVECTOR3 vEyePt(0.0f, 0.5f, -2.0f); //視点位置
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	//D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	//// プロジェクショントランスフォーム
//	D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f);

	D3DXVECTOR3 vEyePt = VectorToD3DXVECTOR3(m_Camera->m_eyepos);
	D3DXVECTOR3 vLookatPt = VectorToD3DXVECTOR3(m_Camera->m_refpos);
	D3DXVECTOR3 vUpVec = VectorToD3DXVECTOR3(m_Camera->m_upvec);
	//vEyePt.z = -vEyePt.z;
	//vLookatPt.z = -vLookatPt.z;
	//D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	//D3DXMatrixPerspectiveFovLH(&Proj, m_Camera->m_fovY, m_Camera->m_aspect, m_Camera->m_NearClip, m_Camera->m_FarClip);

	D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovRH(&Proj, m_Camera->m_fovY, m_Camera->m_aspect, m_Camera->m_NearClip, m_Camera->m_FarClip);

	//D3DXMatrixPerspectiveFovLH(&Proj, XMConvertToRadians(30.0f), (float)800/600, 0.1f, 1000.0f);

	////深度テストを”無効”にする
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//depthStencilDesc.DepthEnable = false;

	//devices.Device().Get()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
	//devices.Context().Get()->OMSetDepthStencilState(m_DepthStencilState, 1);
	//
	D3DXMATRIX CancelRotation = View;
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0;
	D3DXMatrixInverse(&CancelRotation, NULL, &CancelRotation);
	World = CancelRotation * World;

	//パーティクル１粒を１枚ポイントスプライトとして５００枚描画

	for (vector<PARTICLE*>::iterator iterator = m_pParticle.begin(); iterator != m_pParticle.end(); iterator++)
	{
		if ((*iterator) != nullptr)
		{
			for (int i = 0; i < MAX_PARTICLE; i++)
			{
				//ワールドトランスフォームは個々で異なる
				D3DXMATRIX Scale, Tran;
				D3DXMatrixScaling(&Scale,5, 5, 5);
				D3DXVECTOR3 ParticlePos = (*iterator)->GetParticlePos(i);
				D3DXMatrixTranslation(&Tran, ParticlePos.x, ParticlePos.y, ParticlePos.z);
				//D3DXMatrixTranslation(&Tran, 0.0f, 0.0f, 0.0f);
				World = Scale*CancelRotation*Tran;

				//RenderSprite(World* MatrixToD3DXMATRIX( m_view)* MatrixToD3DXMATRIX (m_proj));
				RenderSprite(World * View * Proj);
				//RenderSprite(World * Proj);
				//RenderSprite(World);
			}
			//D3DXVECTOR3 ParticlePos = (*iterator)->GetParticlePos(7);
		}
	}
	




//	auto& devices = Devices::Get();
//
//	
//	//使用するシェーダーの登録	
//	devices.Context().Get()->VSSetShader(m_pVertexShader, NULL, 0);
//	devices.Context().Get()->PSSetShader(m_pPixelShader, NULL, 0);
//	//シェーダーのコンスタントバッファーに各種データを渡す	
//	D3D11_MAPPED_SUBRESOURCE pData;
//	SIMPLESHADER_CONSTANT_BUFFER cb;
//	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
//	{
//		//ワールド、カメラ、射影行列を渡す
//		D3DXMATRIX m = World * View * Proj;
//		D3DXMatrixTranspose(&m, &m);
//		cb.mWVP = m;
//
//		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
//		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
//	}
//	//このコンスタントバッファーを使うシェーダーの登録
//	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//	//頂点インプットレイアウトをセット
//	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
//	//プリミティブ・トポロジーをセット
//	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	//バーテックスバッファーをセット
//	UINT stride = sizeof(D3DXVECTOR3);
//	UINT offset = 0;
//	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
//	//インデックスバッファーをセット
//	stride = sizeof(int);
//	offset = 0;
//	devices.Context().Get()->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	//プリミティブをレンダリング
//	devices.Context().Get()->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);
//



	Present();
}
//
//// Helper method to clear the back buffers.
void Game::Clear()
{
	auto& devices = Devices::Get();


	// Clear the views.
	devices.Context().Get()->ClearRenderTargetView(devices.RenderTargetView().Get(), Colors::CornflowerBlue);
	devices.Context().Get()->ClearDepthStencilView(devices.DepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	devices.Context().Get()->OMSetRenderTargets(1, devices.RenderTargetView().GetAddressOf(), devices.DepthStencilView().Get());

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


D3DXMATRIX Game::MatrixToD3DXMATRIX(DirectX::SimpleMath::Matrix matrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	D3DXMATRIX d3dxmatrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			d3dxmatrix.m[i][j] = matrix.m[i][j];
		}
	}

	return d3dxmatrix;
}

D3DXVECTOR3 Game::VectorToD3DXVECTOR3(DirectX::SimpleMath::Vector3 vector3)
{
	float x = vector3.x;
	float y = vector3.y;
	float z = vector3.z;
	D3DXVECTOR3 d3dxvector3 = D3DXVECTOR3(x, y, z);
	return d3dxvector3;

}

Vector3 Game::TD3DXVECTOR3ToVector(D3DXVECTOR3 d3dxvector3)
{
	float x = d3dxvector3.x;
	float y = d3dxvector3.y;
	float z = d3dxvector3.z;
	Vector3 vector3 = Vector3(x, y, z);
	return vector3;
}

Matrix Game::D3DXMATRIXToMatrix(D3DXMATRIX d3dxmatrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	Matrix matrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			matrix.m[i][j] = d3dxmatrix.m[i][j];
		}
	}


	return matrix;
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


// These are the resources that depend on the device.
//void Game::CreateDevice()
//{
////	UINT creationFlags = 0;
////
////#ifdef _DEBUG
////	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
////#endif
////
////	static const D3D_FEATURE_LEVEL featureLevels [] =
////	{
////		// TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
////		D3D_FEATURE_LEVEL_11_1,
////		D3D_FEATURE_LEVEL_11_0,
////		D3D_FEATURE_LEVEL_10_1,
////		D3D_FEATURE_LEVEL_10_0,
////		D3D_FEATURE_LEVEL_9_3,
////		D3D_FEATURE_LEVEL_9_2,
////		D3D_FEATURE_LEVEL_9_1,
////	};
////
////	// Create the DX11 API device object, and get a corresponding context.
////	HRESULT hr = D3D11CreateDevice(
////		nullptr,                                // specify nullptr to use the default adapter
////		D3D_DRIVER_TYPE_HARDWARE,
////		nullptr,
////		creationFlags,
////		featureLevels,
////		_countof(featureLevels),
////		D3D11_SDK_VERSION,
////		devices.Device.Get().ReleaseAndGetAddressOf(),   // returns the Direct3D device created
////		&m_featureLevel,                        // returns feature level of device created
////		m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
////		);
////
////	if (hr == E_INVALIDARG)
////	{
////		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
////		hr = D3D11CreateDevice(nullptr,
////			D3D_DRIVER_TYPE_HARDWARE,
////			nullptr,
////			creationFlags,
////			&featureLevels[1],
////			_countof(featureLevels) - 1,
////			D3D11_SDK_VERSION,
////			devices.Device.Get().ReleaseAndGetAddressOf(),
////			&m_featureLevel,
////			m_d3dContext.ReleaseAndGetAddressOf()
////			);
////	}
////
////	DX::ThrowIfFailed(hr);
////
////#ifndef NDEBUG
////	ComPtr<ID3D11Debug> d3dDebug;
////	if (SUCCEEDED(devices.Device.Get().As(&d3dDebug)))
////	{
////		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
////		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
////		{
////#ifdef _DEBUG
////			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
////			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
////#endif
////			D3D11_MESSAGE_ID hide [] =
////			{
////				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
////				// TODO: Add more message IDs here as needed.
////			};
////			D3D11_INFO_QUEUE_FILTER filter = {};
////			filter.DenyList.NumIDs = _countof(hide);
////			filter.DenyList.pIDList = hide;
////			d3dInfoQueue->AddStorageFilterEntries(&filter);
////		}
////	}
////#endif
////
////	// DirectX 11.1 if present
////	if (SUCCEEDED(devices.Device.Get().As(&devices.Device.Get()1)))
////		(void)m_d3dContext.As(&m_d3dContext1);
////
////	// TODO: Initialize device dependent objects here (independent of window size).
////}
////
////// Allocate all memory resources that change on a window SizeChanged event.
////void Game::CreateResources()
////{
////	// Clear the previous window size specific context.
////	ID3D11RenderTargetView* nullViews [] = { nullptr };
////	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
////	m_renderTargetView.Reset();
////	m_depthStencilView.Reset();
////	m_d3dContext->Flush();
////
////	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
////	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
////	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
////	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
////	UINT backBufferCount = 2;
////
////	// If the swap chain already exists, resize it, otherwise create one.
////	if (m_swapChain)
////	{
////		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);
////
////		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
////		{
////			// If the device was removed for any reason, a new device and swap chain will need to be created.
////			OnDeviceLost();
////
////			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
////			// and correctly set up the new device.
////			return;
////		}
////		else
////		{
////			DX::ThrowIfFailed(hr);
////		}
////	}
////	else
////	{
////		// First, retrieve the underlying DXGI Device from the D3D Device.
////		ComPtr<IDXGIDevice1> dxgiDevice;
////		DX::ThrowIfFailed(devices.Device.Get().As(&dxgiDevice));
////
////		// Identify the physical adapter (GPU or card) this device is running on.
////		ComPtr<IDXGIAdapter> dxgiAdapter;
////		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
////
////		// And obtain the factory object that created it.
////		ComPtr<IDXGIFactory1> dxgiFactory;
////		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));
////
////		ComPtr<IDXGIFactory2> dxgiFactory2;
////		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
////		{
////			// DirectX 11.1 or later
////
////			// Create a descriptor for the swap chain.
////			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
////			swapChainDesc.Width = backBufferWidth;
////			swapChainDesc.Height = backBufferHeight;
////			swapChainDesc.Format = backBufferFormat;
////			swapChainDesc.SampleDesc.Count = 1;
////			swapChainDesc.SampleDesc.Quality = 0;
////			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
////			swapChainDesc.BufferCount = backBufferCount;
////
////			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
////			fsSwapChainDesc.Windowed = TRUE;
////
////			// Create a SwapChain from a Win32 window.
////			DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
////				devices.Device.Get().Get(),
////				m_window,
////				&swapChainDesc,
////				&fsSwapChainDesc,
////				nullptr,
////				m_swapChain1.ReleaseAndGetAddressOf()
////				));
////
////			DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
////		}
////		else
////		{
////			DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
////			swapChainDesc.BufferCount = backBufferCount;
////			swapChainDesc.BufferDesc.Width = backBufferWidth;
////			swapChainDesc.BufferDesc.Height = backBufferHeight;
////			swapChainDesc.BufferDesc.Format = backBufferFormat;
////			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
////			swapChainDesc.OutputWindow = m_window;
////			swapChainDesc.SampleDesc.Count = 1;
////			swapChainDesc.SampleDesc.Quality = 0;
////			swapChainDesc.Windowed = TRUE;
////
////			DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(devices.Device.Get().Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
////		}
////
////		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
////		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
////	}
////
////	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
////	ComPtr<ID3D11Texture2D> backBuffer;
////	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));
////
////	// Create a view interface on the rendertarget to use on bind.
////	DX::ThrowIfFailed(devices.Device.Get()->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));
////
////	// Allocate a 2-D surface as the depth/stencil buffer and
////	// create a DepthStencil view on this surface to use on bind.
////	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
////
////	ComPtr<ID3D11Texture2D> depthStencil;
////	DX::ThrowIfFailed(devices.Device.Get()->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));
////
////	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
////	DX::ThrowIfFailed(devices.Device.Get()->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));
////
////	// TODO: Initialize windows-size dependent objects here.
////}
////
////void Game::OnDeviceLost()
////{
////	// TODO: Add Direct3D resource cleanup here.
////
////	m_depthStencilView.Reset();
////	m_renderTargetView.Reset();
////	m_swapChain1.Reset();
////	m_swapChain.Reset();
////	m_d3dContext1.Reset();
////	m_d3dContext.Reset();
////	devices.Device.Get()1.Reset();
////	devices.Device.Get().Reset();
////
////	CreateDevice();
////
////	CreateResources();
//}