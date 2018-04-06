#include "ShadowMap.h"

using namespace NYLibrary;
using namespace std;

unique_ptr<ShadowMap> ShadowMap::shadowMap;


/// <summary>
/// シャドウマップのインスタンスを取得する
/// </summary>
/// <returns>シャドウマップのインスタンス</returns>
ShadowMap * ShadowMap::GetInstance()
{
	if (!shadowMap.get())
	{
		shadowMap.reset(new ShadowMap);
	}

	return shadowMap.get();
}

/// <summary>
/// 初期化
/// </summary>
void ShadowMap::Initilize()
{
	CreateDSVTex2D();
	CreateRTVTex2D();
	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateShaderResourceView();
	CreateShader();
	CreateConstantBuffer();
	SamplerCreate();
}

/// <summary>
/// 終了
/// </summary>
void ShadowMap::Finalize()
{
	shadowMap.release();
	DSVTex2D.Reset();
	RTVTex2D.Reset();
	depthStencilView.Reset();
	renderTargetView.Reset();
	shaderResourceView.Reset();
	vertexShader.Reset();
	pixelShader.Reset();
}

/// <summary>
/// デプスステンシル用のテクスチャ作成
/// </summary>
void ShadowMap::CreateDSVTex2D()
{
	auto& devices = Devices::Get();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = devices.Width();
	descDepth.Height = devices.Height();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	devices.Device().Get()->CreateTexture2D(&descDepth, nullptr, DSVTex2D.ReleaseAndGetAddressOf());
}

/// <summary>
/// 深度マップ用テクスチャ
/// </summary>
void ShadowMap::CreateRTVTex2D()
{
	auto& devices = Devices::Get();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = devices.Width();
	descDepth.Height = devices.Height();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.MiscFlags = 0;
	descDepth.Format = DXGI_FORMAT_R32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	devices.Device().Get()->CreateTexture2D(&descDepth, nullptr, RTVTex2D.ReleaseAndGetAddressOf());

}


/// <summary>
/// 深度ステンシルビュー作成
/// </summary>
void ShadowMap::CreateDepthStencilView()
{
	auto& device = Devices::Get().Device();
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	device->CreateDepthStencilView(DSVTex2D.Get(), nullptr, depthStencilView.ReleaseAndGetAddressOf());
}

/// <summary>
/// レンダーターゲットビュー作成
/// </summary>
void ShadowMap::CreateRenderTargetView()
{
	auto& device = Devices::Get().Device();

	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = DXGI_FORMAT_R32_FLOAT;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(RTVTex2D.Get(), &DescRT, renderTargetView.ReleaseAndGetAddressOf());
}

/// <summary>
/// シェーダーリソースビューを作成
/// </summary>
void ShadowMap::CreateShaderResourceView()
{
	auto& device = Devices::Get().Device();

	//深度マップテクスチャ用　シェーダーリソースビュー(SRV)作成	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(RTVTex2D.Get(), &SRVDesc,shaderResourceView.GetAddressOf());
}

/// <summary>
/// シェーダー作成
/// </summary>
void ShadowMap::CreateShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//深度テクスチャ用バーテックスシェーダー作成
	MakeShader("Resources/HLSL/ShadowMap.hlsl", "VS_Depth", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader);
	MakeShader("Resources/HLSL/ShadowMap.hlsl", "PS_Depth", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader);
}

/// <summary>
/// コンスタントバッファー作成
/// </summary>
void ShadowMap::CreateConstantBuffer()
{
	auto& device = Devices::Get().Device();

	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&cb, nullptr, constantBuffer.ReleaseAndGetAddressOf());

}


/// <summary>
/// レンダリングターゲットをクリアする
/// </summary>
void ShadowMap::ClearRTVAndDSV()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	float ClearColor[4] = { 0,0,0,1};
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), ClearColor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア
}


/// <summary>
/// レンダーターゲットビューを設定
/// </summary>
void ShadowMap::OMSetRenderTargets()
{
	auto& deviceContext = Devices::Get().Context();
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(Devices::Get().Width());
	vp.Height = static_cast<float>(Devices::Get().Height());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);
	//レンダーターゲットを深度テクスチャーに変更
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(),	depthStencilView.Get());
}

/// <summary>
/// シャドウマップのシェーダーを設定する
/// </summary>
void ShadowMap::SetShader()
{
	auto& deviceContext = Devices::Get().Context();
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
}



void ShadowMap::SamplerCreate()
{
	auto& device = Devices::Get().Device();

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 1;
	desc.MipLODBias = 0;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = +FLT_MAX;
	
	 // サンプラーステートを生成.
	device->CreateSamplerState(&desc, sampler.GetAddressOf());

}