#include "SimpleTextures.h"
using namespace std;
const DXGI_FORMAT SimpleTextures::FORMAT = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;



/// <summary>
/// コンストラクタ
/// </summary>
SimpleTextures::SimpleTextures(D3DXVECTOR2 viewSize) :
	viewSize(viewSize)
{}

/// <summary>
/// 初期化
/// </summary>
/// <param name="view">ビューポートの大きさ</param>
/// <returns>自クラスのインスタンス</returns>
void SimpleTextures::Initialize()
{
	//デバイス取得
	device  = Devices::Get().Device();
	//2Dテクスチャ作成
	CreateTexture2D();
	//シェーダーリソースビュー作成　
	CreateShaderResourceView();
	//レンダリングターゲットビュー作成
	CreateRenderTargetView();
	//レンダリングターゲットをクリア
	ClearRenderTargetView();
	//深度ステンシルビュー作成
	CreateDepthStencilView();
}


/// <summary>
/// 終了
/// </summary>
void SimpleTextures::Finalize()
{
	texture2D.Reset();
	depthTexture2D.Reset();
	shaderResourceView.Reset();
	renderTargetView.Reset();
	depthStencilView.Reset();
}

/// <summary>
/// レンダリングターゲットを使用する
/// </summary>
void SimpleTextures::SetRenderTargets()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	//////ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = Devices::Get().Width() * 2;
	vp.Height = Devices::Get().Height() * 2;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

}

/// <summary>
/// レンダリングターゲットをクリアする
/// </summary>
void SimpleTextures::ClearRenderTargetView()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	float ClearColor[4] = { 0,0,0,0 };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), ClearColor);

}

/// <summary>
/// 2Dテクスチャ作成
/// </summary>
void SimpleTextures::CreateTexture2D()
{
	//インク用テクスチャを作成
	D3D11_TEXTURE2D_DESC inkdesc;
	ZeroMemory(&inkdesc, sizeof(D3D11_TEXTURE2D_DESC));
	inkdesc.Width = viewSize.x;
	inkdesc.Height = viewSize.y;
	inkdesc.MipLevels = 1;
	inkdesc.ArraySize = 1;
	inkdesc.MiscFlags = 0;
	inkdesc.Format = FORMAT;
	inkdesc.SampleDesc.Count = 1;
	inkdesc.SampleDesc.Quality = 0;
	inkdesc.Usage = D3D11_USAGE_DEFAULT;
	inkdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	inkdesc.CPUAccessFlags = 0;
	device.Get()->CreateTexture2D(&inkdesc, nullptr, texture2D.ReleaseAndGetAddressOf());
}



/// <summary>
/// シェーダーリソースビュー作成　
/// </summary>
void SimpleTextures::CreateShaderResourceView()
{
	//インク用テクスチャ　シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC inkSRVDesc;
	ZeroMemory(&inkSRVDesc, sizeof(inkSRVDesc));
	inkSRVDesc.Format = FORMAT;
	inkSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	inkSRVDesc.Texture2D.MipLevels = 1;
	device.Get()->CreateShaderResourceView(texture2D.Get(), &inkSRVDesc, shaderResourceView.ReleaseAndGetAddressOf());
}

/// <summary>
/// レンダリングターゲットビュー作成
/// </summary>
void SimpleTextures::CreateRenderTargetView()
{
	D3D11_RENDER_TARGET_VIEW_DESC inkDescRT;
	inkDescRT.Format = FORMAT;
	inkDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	inkDescRT.Texture2D.MipSlice = 0;
	device.Get()->CreateRenderTargetView(texture2D.Get(), &inkDescRT, renderTargetView.ReleaseAndGetAddressOf());
}

/// <summary>
/// 深度ステンシルビュー作成
/// </summary>
void SimpleTextures::CreateDepthStencilView()
{
	//深度マップテクスチャをレンダーターゲットにする際のデプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = viewSize.x;
	descDepth.Height = viewSize.y;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device.Get()->CreateTexture2D(&descDepth, nullptr, depthTexture2D.ReleaseAndGetAddressOf());
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	device.Get()->CreateDepthStencilView(depthTexture2D.Get(), nullptr, depthStencilView.ReleaseAndGetAddressOf());
}
