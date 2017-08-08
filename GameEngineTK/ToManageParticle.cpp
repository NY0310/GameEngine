#include "ToManageParticle.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")



/// <summary>
/// //初期化関数を全て実行する
/// </summary>
void ToManageParticle::Init()
{
	shadermanager = ShaderManager::Get();

	this->InitD3D();
	this->InitModel();
}

/// <summary>
/// シェーダー、コンスタントバッファー作成
/// </summary>
/// <returns></returns>
HRESULT ToManageParticle::InitD3D()
{
	auto& devices = Devices::Get();



	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	D3D11_BLEND_DESC bd;
	//バーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("Shader.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader))) return E_FAIL;


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
	if (FAILED(shadermanager.MakeShader("Shader.hlsl", "GS_Point", "gs_5_0", (void**)&m_pGeometryShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);



	//ピクセルシェーダー作成
	if (FAILED(shadermanager.MakeShader("Shader.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader))) return E_FAIL;
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






	return S_OK;
}


/// <summary>
/// モデルの初期化
/// </summary>
/// <returns></returns>
HRESULT ToManageParticle::InitModel()
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

/// <summary>
/// 更新処理
/// </summary>
void ToManageParticle::Update()
{

	std::vector<PARTICLE*>::iterator partecle = m_pParticle.begin();





	for (std::vector<int>::iterator it = m_Particlecnt.begin(); it != m_Particlecnt.end(); )
	{
		(*it)++;
		(*partecle)->Run();

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
	for (vector<PARTICLE*>::iterator iterator = m_pParticle.begin(); iterator != m_pParticle.end(); iterator++)
			{ 				(*iterator)->Run();
			}
			

}


/// <summary>
/// 全パーティクルのワールド行列を指定して描画処理を呼ぶ
/// </summary>
/// <param name="camera">参照カメラ</param>
void ToManageParticle::Render(std::unique_ptr<FollowCamera>& camera)
{

	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, x, 0, 0);
	World = Tran;


	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->m_eyepos);
	D3DXVECTOR3 vLookatPt = shadermanager.VectorToD3DXVECTOR3(camera->m_refpos);
	D3DXVECTOR3 vUpVec = shadermanager.VectorToD3DXVECTOR3(camera->m_upvec);

	D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovRH(&Proj, camera->m_fovY, camera->m_aspect, camera->m_NearClip, camera->m_FarClip);



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
				D3DXMatrixScaling(&Scale, 5, 5, 5);
				D3DXVECTOR3 ParticlePos = (*iterator)->GetParticlePos(i);
				D3DXMatrixTranslation(&Tran, ParticlePos.x, ParticlePos.y, ParticlePos.z);
				World = Scale*CancelRotation*Tran;
				RenderSprite(World * View * Proj);
			}
		}
	}
}


/// <summary>
/// 描画処理
/// </summary>
/// <param name="WVP"></param>
void ToManageParticle::RenderSprite(D3DXMATRIX& WVP)
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


	devices.Context().Get()->VSSetShader(NULL, NULL, 0);
	devices.Context().Get()->GSSetShader(NULL, NULL, 0);
	devices.Context().Get()->PSSetShader(NULL, NULL, 0);

}

/// <summary>
/// パーティクル追加
/// </summary>
void ToManageParticle::AddParticle(Vector3 pos)
{
   	m_pParticle.push_back(new PARTICLE(MAX_PARTICLE, shadermanager.VectorToD3DXVECTOR3(pos)));
 	m_Particlecnt.push_back(0);
}
