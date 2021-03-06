#include "Paint.h"

using namespace std;
ComPtr<ID3D11VertexShader> Paint::inkVertexShader;
ComPtr<ID3D11PixelShader> Paint::inkPixelShader;
ComPtr<ID3D11VertexShader> Paint::DripVertexShader;
ComPtr<ID3D11PixelShader>  Paint::DripPixelShader;
ComPtr<ID3D11VertexShader> Paint::DripPlaneVertexShader;
ComPtr<ID3D11PixelShader>  Paint::DripPlanePixelShader;
ComPtr<ID3D11VertexShader> Paint::updateVertexShader;
ComPtr<ID3D11PixelShader> Paint::updatePixelShader;
ComPtr<ID3D11InputLayout> Paint::inkVertexLayout;
bool Paint::isFirst = true;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="isPlane">平面か</param>
Paint::Paint(bool isPlane ):isPlane(isPlane)
{
	if (isFirst)
	{
		inkVertexShader.Reset();
		inkPixelShader.Reset();
		DripVertexShader.Reset();
		DripPixelShader.Reset();
		DripPlaneVertexShader.Reset();
		DripPlanePixelShader.Reset();
		DripPlanePixelShader.Reset();
		updateVertexShader.Reset();
		updatePixelShader.Reset();
		inkVertexLayout.Reset();
	}
	inkConstantBuffer.Reset();
	sampleLimear.Reset();
	inkNormalMap.Reset();
	dripVertexBuffer.Reset();
}


/// <summary>
/// デストラクタ
/// </summary>
Paint::~Paint()
{
	inkTexture.Reset();
	sampleLimear.Reset();
	inkNormalMap.Reset();
	dripVertexBuffer.Reset();
	textures->Finalize();
	textures.release();
	dripTextures.release();
}

/// <summary>
/// 初期化
/// </summary>
void Paint::Initialize()
{
	textures = make_unique<CampusTextures>(D3DXVECTOR2(Devices::Get().Width() * 2.0f, Devices::Get().Height() * 2.0f));
	textures->Initialize();
	dripTextures = make_unique<CampusTextures>(D3DXVECTOR2(Devices::Get().Width() * 2.0f, Devices::Get().Height() * 2.0f));
	dripTextures->Initialize();

	if (isFirst)
	{
		isFirst = false;
		ID3DBlob *pCompiledShader = nullptr;
		//インクテクスチャ用バーテックスシェーダー作成
		MakeShader("Resources/HLSL/Campus.hlsl", "VS", "vs_5_0", (void**)inkVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader);
		//インクテクスチャ用頂点インプットレイアウトをセット
		D3D11_INPUT_ELEMENT_DESC inkInputLayout[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = sizeof(inkInputLayout) / sizeof(inkInputLayout[0]);
		//頂点インプットレイアウトを作成
		device->CreateInputLayout(inkInputLayout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), inkVertexLayout.ReleaseAndGetAddressOf());
		//インクテクスチャ用ピクセルシェーダー作成
		MakeShader("Resources/HLSL/Campus.hlsl", "PS", "ps_5_0", (void**)inkPixelShader.ReleaseAndGetAddressOf(), &pCompiledShader);


		//垂らすシェーダー
		//バーテックスシェーダー
		MakeShader("Resources/HLSL/PlaneDrip.hlsl", "VS", "vs_5_0", (void**)DripPlaneVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader);
		//ピクセルシェーダー
		MakeShader("Resources/HLSL/PlaneDrip.hlsl", "PS", "ps_5_0", (void**)DripPlanePixelShader.ReleaseAndGetAddressOf(), &pCompiledShader);
	
		MakeShader("Resources/HLSL/Drip.hlsl", "VS", "vs_5_0", (void**)DripVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader);
		//ピクセルシェーダー
		MakeShader("Resources/HLSL/Drip.hlsl", "PS", "ps_5_0", (void**)DripPixelShader.ReleaseAndGetAddressOf(), &pCompiledShader);

	

		//更新するシェーダー
		//バーテックスシェーダー
		MakeShader("Resources/HLSL/PaintUpdate.hlsl", "VS", "vs_5_0", (void**)updateVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader);
		//ピクセルシェーダー
		MakeShader("Resources/HLSL/PaintUpdate.hlsl", "PS", "ps_5_0", (void**)updatePixelShader.ReleaseAndGetAddressOf(), &pCompiledShader);

	}




	//インク用テクスチャ用コンスタントバッファ
	D3D11_BUFFER_DESC Inkcb;
	Inkcb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Inkcb.ByteWidth = sizeof(D3DXVECTOR4);
	Inkcb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Inkcb.MiscFlags = 0;
	Inkcb.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&Inkcb, nullptr, inkConstantBuffer.ReleaseAndGetAddressOf());
	

	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	device->CreateSamplerState(&SamDesc, sampleLimear.ReleaseAndGetAddressOf());

	//インクテクスチャを作成	
	D3DX11CreateShaderResourceViewFromFileA(device, "Resources/PNG/pink1.png", nullptr, nullptr, inkTexture.ReleaseAndGetAddressOf(), nullptr);

	//テクスチャー作成
	D3DX11CreateShaderResourceViewFromFileA(device, "Resources/HeightMap/BrushNormal.jpg", nullptr, nullptr, inkNormalMap.ReleaseAndGetAddressOf(), nullptr);



	CreateVertexBuffer();

}




/// <summary>
/// インクを生成する
/// </summary>
/// <param name="Color">色</param>
/// <param name="uv">テクスチャ座標</param>
/// <param name="size">大きさ</param>
void Paint::CreateInk(D3DXVECTOR4 Color, D3DXVECTOR2 uv, float size)
{
	InkData inkdata;
	inkdata.Color = Color;
	inkdata.Uv = uv;
	inkdata.Size = size;
	inkdata.vertexBuffer = CreateVertexBuffer(inkdata);
	inkData.emplace_back(inkdata);
}

//バーテックスバッファー作成
void Paint::CreateVertexBuffer()
{
	//頂点を定義
	PaintVertex vertices[] =
	{
		{D3DXVECTOR3(-1,-1,0),D3DXVECTOR2(0,1)},//頂点1	
		{D3DXVECTOR3(-1, 1, 0), D3DXVECTOR2(0, 0)}, //頂点2
		{D3DXVECTOR3(1, -1, 0), D3DXVECTOR2(1, 1)},  //頂点3
		{D3DXVECTOR3(1, 1, 0), D3DXVECTOR2(1, 0)},//頂点4	
	};
	//上の頂点でバーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PaintVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	device->CreateBuffer(&bd, &InitData, dripVertexBuffer.ReleaseAndGetAddressOf());
}

/// <summary>
/// インクのuv値と大きさからバーテックスバッファーを生成
/// </summary>
/// <param name="inkdata"><インクのデータ/param>
/// <returns>生成したバーテックスバッファー</returns>
ID3D11Buffer* Paint::CreateVertexBuffer(InkData & inkdata)
{
	//インクサイズを正規デバイス座標系にする
	float uvSize = inkdata.Size;
	D3DXVECTOR2 Uv = inkdata.Uv;

	PaintVertex vertex[] = {
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y + uvSize,0)),D3DXVECTOR2(0,1) },
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y - uvSize,0)),D3DXVECTOR2(0,0) },
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y + uvSize,0)),D3DXVECTOR2(1,1) },
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y - uvSize,0)),D3DXVECTOR2(1,0) },
	};


	//上の頂点でバーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PaintVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertex;
	ID3D11Buffer*  canvasVertexBuffer = nullptr;
	device->CreateBuffer(&bd, &InitData, &canvasVertexBuffer);
	return canvasVertexBuffer;
}

/// <summary>
/// インクの描画
/// </summary>
void Paint::ClearRenderConfig()
{
	textures->SetRenderTargets();
	InkRender();
	
	
	DripRender();

	dripTextures->SetRenderTargets();
	UpDateRender();
	//インクデータを初期化
	inkData.clear();
}

/// <summary>
/// キャンパスに付着したインクをクリアする
/// </summary>
void Paint::ClearCampus()
{
	textures->ClearRenderTargetView();
	dripTextures->ClearRenderTargetView();
}


/// <summary>
/// 新規インクの描画
/// </summary>
void Paint::InkRender()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト

	deviceContext->VSSetShader(inkVertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(inkPixelShader.Get(), nullptr, 0);


	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, inkTexture.GetAddressOf());//インクのレクスチャ
	deviceContext->PSSetShaderResources(1, 1, dripTextures->GetShaderResourceView().GetAddressOf());//インクのレクスチャ
	deviceContext->PSSetShaderResources(2, 1, inkNormalMap.GetAddressOf());//インクのノーマルマップ	

	//テクスチャーをシェーダーに渡す
	//このパスで使用するシェーダーの登録
	//このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, inkConstantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, inkConstantBuffer.GetAddressOf());
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(inkVertexLayout.Get());
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



	for (auto ink : inkData)
	{

		//インクの色を渡す
		this->InkRender(ink);
	}

}


/// <summary>
/// 新規インクの描画
/// </summary>
/// <param name="ink">インク一つ分のデータ</param>
void Paint::InkRender(InkData& ink)
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト




	D3D11_MAPPED_SUBRESOURCE pData;
	D3DXVECTOR4 cb;
	if (SUCCEEDED(deviceContext->Map(inkConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		cb  = ink.Color;
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(D3DXVECTOR4));
		deviceContext->Unmap(inkConstantBuffer.Get(), 0);

	}

	//バーテックスバッファーをセット
	UINT stride = sizeof(PaintVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &ink.vertexBuffer, &stride, &offset);


	deviceContext->Draw(4, 0);

}

/// <summary>
/// インク垂れ流し
/// </summary>
void Paint::DripRender()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト

	if (isPlane)
	{
		deviceContext->VSSetShader(DripPlaneVertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(DripPlanePixelShader.Get(), nullptr, 0);

	}
	else
	{
		deviceContext->VSSetShader(DripVertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(DripPixelShader.Get(), nullptr, 0);

	}


	//サンプラーとテクスチャをシェーダーに渡す
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());

	if (inkData.size() == 0)
	{
		deviceContext->PSSetShaderResources(0, 1, dripTextures->GetShaderResourceView().GetAddressOf());//インクのレクスチャ
	}
	else
	{
		deviceContext->PSSetShaderResources(0, 1, textures->GetShaderResourceView().GetAddressOf());//インクのレクスチャ
	}


	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(inkVertexLayout.Get());
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//バーテックスバッファーをセット
	UINT stride = sizeof(PaintVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, dripVertexBuffer.GetAddressOf(), &stride, &offset);


	//プリミティブをレンダリング
	deviceContext->Draw(4, 0);
}

/// <summary>
/// インクの垂れ流しを実装するために更新する
/// </summary>
void Paint::UpDateRender()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト


	deviceContext->VSSetShader(updateVertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(updatePixelShader.Get(), nullptr, 0);

	//サンプラーとテクスチャをシェーダーに渡す
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, textures->GetShaderResourceView().GetAddressOf());//インクのレクスチャ


	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(inkVertexLayout.Get());
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//バーテックスバッファーをセット
	UINT stride = sizeof(PaintVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, dripVertexBuffer.GetAddressOf(), &stride, &offset);


	//プリミティブをレンダリング
	deviceContext->Draw(4, 0);

}






