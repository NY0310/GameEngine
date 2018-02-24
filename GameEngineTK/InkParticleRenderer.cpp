#include "InkParticleRenderer.h"

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
using namespace std;



InkParticleRenderer::~InkParticleRenderer()
{
	vertexShader.Reset();
	geometryShader.Reset();
	pixelShader.Reset();
	constantBuffer.Reset();
	vertexBuffer.Reset();
	vertexLayout.Reset();
	texture.Reset();
	sampler.Reset();
	blendState.Reset();
	depthStencilState.Reset();
}

/// <summary>
/// //初期化関数を全て実行する
/// </summary>
HRESULT InkParticleRenderer::Initialize()
{
	auto& devices = Devices::Get();



	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *compiledShader = nullptr;
	D3D11_BLEND_DESC bd;
	//バーテックスシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PointSprite.hlsl", "VS", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;


	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device()->CreateInputLayout(layout, numElements, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf())))
	{
		return FALSE;
	}

	SAFE_RELEASE(compiledShader);


	////ジオメトリシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PointSprite.hlsl", "GS_Point", "gs_5_0", (void**)geometryShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;
	SAFE_RELEASE(compiledShader);



	//ピクセルシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PointSprite.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;
	SAFE_RELEASE(compiledShader);



	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	if (FAILED(devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}


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

	if (FAILED(devices.Device().Get()->CreateBlendState(&bd, blendState.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}

	//バーテックスバッファー作成
	vertexData vertices[] =
	{
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertexData);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bufferDesc, &InitData, vertexBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}



	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, sampler.ReleaseAndGetAddressOf());

	//テクスチャー読み込み
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), L"Resources/PNG/Snow.png", nullptr, nullptr, texture.ReleaseAndGetAddressOf(), nullptr)))
	{
		MessageBoxA(0, "テクスチャーを読み込めません", "", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}




/// <summary>
/// 全パーティクルのワールド行列を指定して描画処理を呼ぶ
/// </summary>
/// <param name="camera">参照カメラ</param>
void InkParticleRenderer::Render(const vector<ConstantInkData> inkData)
{

	auto& devices = Devices::Get();
	devices.Context().Get()->VSSetShader(vertexShader.Get(), nullptr, 0);
	devices.Context().Get()->GSSetShader(geometryShader.Get(), nullptr, 0);
	devices.Context().Get()->PSSetShader(pixelShader.Get(), nullptr, 0);

	Matrix World, Scale, Tran, Rot, View, Proj , wvp;

	View = camera->GetView();
	Proj = camera->GetProjection();

	D3DXVECTOR3 vEyePt = Math::VectorToD3DXVECTOR3(camera->GetEyePos());


	//
	Matrix CancelRotation = View;
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0;
	
	CancelRotation.Invert();
	//バーテックスバッファーをセット
	UINT stride = sizeof(vertexData);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	//UINT mask = 0xffffffff;
	//devices.Context().Get()->OMSetBlendState(blendState.Get(), nullptr, mask);

	////深度テストを”無効”にする
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//depthStencilDesc.DepthEnable = false;

	//devices.Device().Get()->CreateDepthStencilState(&depthStencilDesc, depthStencilState.ReleaseAndGetAddressOf());
	//devices.Context().Get()->OMSetDepthStencilState(depthStencilState.Get(), 1);



	//パーティクル１粒を１枚ポイントスプライトとして５００枚描画
	for(auto data : inkData)
	{
		//ワールドトランスフォームは個々で異なる
		Scale =	Matrix::CreateScale(data.scale);
		Tran = Matrix::CreateTranslation(data.positon);
		World = Scale  * Tran;
		wvp = World * View * Proj;
		RenderSprite(wvp, data.color);
	}

	devices.Context().Get()->VSSetShader(nullptr, nullptr, 0);
	devices.Context().Get()->GSSetShader(nullptr, nullptr, 0);
	devices.Context().Get()->PSSetShader(nullptr, nullptr, 0);

}


/// <summary>
/// 描画処理
/// </summary>
/// <param name="WVP"></param>
void InkParticleRenderer::RenderSprite(Matrix& WVP , Vector4& color)
{
	auto& devices = Devices::Get();


	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		Matrix m = WVP;
		m.Invert();
		cb.wvp = m;
		cb.color = color;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer.Get(), 0);
	}


	//このコンスタントバッファーをどのシェーダーで使うか
	devices.Context().Get()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devices.Context().Get()->GSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());


	//頂点インプットレイアウトをセット
	devices.Context().Get()->IASetInputLayout(vertexLayout.Get());
	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//テクスチャーをシェーダーに渡す
	devices.Context().Get()->PSSetSamplers(0, 1, sampler.GetAddressOf());
	devices.Context().Get()->PSSetShaderResources(0, 1, texture.GetAddressOf());	//プリミティブをレンダリング


	devices.Context().Get()->Draw(1, 0);

}
