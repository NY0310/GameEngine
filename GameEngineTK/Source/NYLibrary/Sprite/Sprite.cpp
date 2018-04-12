#include "Sprite.h"

using namespace std;
using namespace NYLibrary;

ComPtr<ID3D11VertexShader> Sprite::vertexShader2D;//バーテックスシェーダ
ComPtr<ID3D11VertexShader> Sprite::vertexShader3D;//バーテックスシェーダ
ComPtr<ID3D11PixelShader> Sprite::pixelShader;//ピクセルシェーダ
ComPtr<ID3D11InputLayout> Sprite::vertexLayout;//頂点インップットレイアウト
ComPtr<ID3D11Buffer>Sprite::constantBuffer3D;
ComPtr<ID3D11Buffer> Sprite::constantBuffer2D;


Sprite::Sprite(LPCWSTR FileName, Dimension dimension)
	:dimension(dimension)
{
	LoadTexture(FileName);
}

Sprite::~Sprite()
{
	vertexShader2D.Reset();
	vertexShader3D.Reset();
	pixelShader.Reset();
	constantBuffer2D.Reset();
	constantBuffer3D.Reset();
	vertexBuffer.Reset();
	vertexLayout.Reset();
	texture.Reset();
	sampler.Reset();
}

void Sprite::Initialize()
{
	transparency = 1;
	anchor = D3DXVECTOR2(0.5f, 0.5f);

	if (vertexShader2D.Get())
		return;
	auto& devices = Devices::Get();
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *compiledShader = nullptr;
	//バーテックスシェーダー作成
	MakeShader("Resources/HLSL/Sprite.hlsl", "VS2D", "vs_5_0", (void**)vertexShader2D.ReleaseAndGetAddressOf(), &compiledShader);

	MakeShader("Resources/HLSL/Sprite.hlsl", "VS3D", "vs_5_0", (void**)vertexShader3D.ReleaseAndGetAddressOf(), &compiledShader);
	


	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成

	devices.Device()->CreateInputLayout(layout, numElements, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf());

	SAFE_RELEASE(compiledShader);



	//ピクセルシェーダー作成
	MakeShader("Resources/HLSL/Sprite.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &compiledShader);
	SAFE_RELEASE(compiledShader);



	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer3D);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer3D.ReleaseAndGetAddressOf());

	//コンスタントバッファー作成　ここでは変換行列渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer2D);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer2D.ReleaseAndGetAddressOf());



	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, sampler.ReleaseAndGetAddressOf());


}

/// <summary>
/// テクスチャを読み込む
/// </summary>
/// <param name="FileName">ファイル名</param>
/// <returns>実行結果</returns>	
HRESULT Sprite::LoadTexture(LPCWSTR FileName)
{
	auto& devices = Devices::Get();


	//テクスチャー読み込み
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), FileName, nullptr, nullptr, texture.ReleaseAndGetAddressOf(), nullptr)))
	{
		MessageBoxA(0, "テクスチャーを読み込めません", "", MB_OK);
		return E_FAIL;
	}

	//テクスチャのサイズを取得
	GetTextureSize(texture.Get());


	return S_OK;
}

void Sprite::Render()
{
	auto& devices = Devices::Get();


	if (dimension == Dimension2)
	{
		CreateVertexBuffer2D();
		devices.Context()->VSSetShader(vertexShader2D.Get(), nullptr, 0);
		SetConstantBuffer3D();
		SetConstantBuffer2D();
	}
	else
	{
		CreateVertexBuffer3D();
		devices.Context()->VSSetShader(vertexShader3D.Get(), nullptr, 0);
		SetConstantBuffer3D();
	}
	devices.Context()->PSSetShader(pixelShader.Get(), nullptr, 0);

	//頂点インプットレイアウトをセット
	devices.Context().Get()->IASetInputLayout(vertexLayout.Get());
	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	devices.Context().Get()->PSSetSamplers(0, 1, sampler.GetAddressOf());
	devices.Context().Get()->PSSetShaderResources(0, 1, texture.GetAddressOf());	//プリミティブをレンダリング

	//バーテックスバッファーをセット
	UINT stride = sizeof(VertexData);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1,vertexBuffer.GetAddressOf(), &stride, &offset);

	devices.Context().Get()->Draw(4, 0);

}

void Sprite::SetConstantBuffer3D()
{
	auto& devices = Devices::Get();

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBuffer3D cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer3D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		cb.wvp = GetWVP();
		D3DXMatrixTranspose(&cb.wvp, &cb.wvp);
		cb.color = D3DXVECTOR4(GetColor().x, GetColor().y, GetColor().z,GetIsUseColor());
		cb.transparency = transparency;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer3D.Get(), 0);
	}


	//このコンスタントバッファーをどのシェーダーで使うか
	devices.Context().Get()->VSSetConstantBuffers(0, 1, constantBuffer3D.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(0, 1, constantBuffer3D.GetAddressOf());

}



void Sprite::SetConstantBuffer2D()
{
	auto& devices = Devices::Get();

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBuffer2D cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		cb.world = GetWorldMatrix();
		D3DXMatrixTranspose(&cb.world, &cb.world);

		cb.viewPortX = devices.Width();
		cb.viewPortY = devices.Height();

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer2D.Get(), 0);
	}


	//このコンスタントバッファーをどのシェーダーで使うか
	devices.Context().Get()->VSSetConstantBuffers(1, 1, constantBuffer2D.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(1, 1, constantBuffer2D.GetAddressOf());

}




/// <summary>
/// テクスチャのサイズを取得
/// </summary>
/// <param name="srv">シェーダーリソースビュー</param>
void Sprite::GetTextureSize(ID3D11ShaderResourceView * srv)
{
	ID3D11Resource* redource;
	srv->GetResource(&redource);
	D3D11_RESOURCE_DIMENSION type;
	redource->GetType(&type);
	if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		ID3D11Texture2D* tex2D = reinterpret_cast<ID3D11Texture2D*>(redource);
		D3D11_TEXTURE2D_DESC desc;
		tex2D->GetDesc(&desc);
		widthSize = desc.Width;
		hightSize = desc.Height;
	}
	else
	{
		MessageBoxA(0, "テクスチャーのサイズを取得できませんでした。", "", MB_OK);
	}

}

/// <summary>
/// バーテックスバッファー作成
/// </summary>
/// <returns>実行結果</returns>
HRESULT Sprite::CreateVertexBuffer2D()
{
	auto& devices = Devices::Get();
	float width = static_cast<float>(devices.Width());
	float hight = static_cast<float>(devices.Height());
	float shiftX = static_cast<float>(widthSize  / 2);
	float shiftY = static_cast<float>(hightSize  / 2);

	//頂点を定義
	VertexData vertices[] =
	{
		D3DXVECTOR3(-shiftX * (1 - anchor.x), shiftY * anchor.y,0),D3DXVECTOR2(0,1),//頂点1,
		D3DXVECTOR3(-shiftX * (1 - anchor.x),-shiftY * (1 - anchor.y),0), D3DXVECTOR2(0,0),//頂点2
		D3DXVECTOR3(shiftX *  anchor.x, shiftY * anchor.y,0),D3DXVECTOR2(1,1), //頂点3
		D3DXVECTOR3(shiftX *  anchor.x,-shiftY * (1 - anchor.y),0),D3DXVECTOR2(1,0), //頂点4
	};




	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexData) * 4;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bufferDesc, &InitData, vertexBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}
	return S_OK;
}


HRESULT Sprite::CreateVertexBuffer3D()
{
	auto& devices = Devices::Get();
	float width = static_cast<float>(devices.Width());
	float hight = static_cast<float>(devices.Height());

	//頂点を定義
	VertexData vertices[] =
	{
		{ D3DXVECTOR3(-widthSize / width,-hightSize / hight,1),D3DXVECTOR2(0,1) },//頂点1	
		{ D3DXVECTOR3(-widthSize / width, hightSize / hight, 1), D3DXVECTOR2(0, 0) }, //頂点2
		{ D3DXVECTOR3(widthSize / width, -hightSize / hight, 1), D3DXVECTOR2(1, 1) },  //頂点3
		{ D3DXVECTOR3(widthSize / width, hightSize / hight, 1), D3DXVECTOR2(1, 0) },//頂点4	
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexData) * 4;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bufferDesc, &InitData, vertexBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;

}
