#include "Sprite.h"

using namespace std;
using namespace NYLibrary;

Sprite::Sprite(LPCWSTR FileName, Dimension dimension)
	:dimension(dimension)
{
	LoadTexture(FileName);
	transparency = 0;
}

Sprite::~Sprite()
{
	vertexShader.Reset();
	pixelShader.Reset();
	constantBuffer.Reset();
	vertexBuffer.Reset();
	vertexLayout.Reset();
	texture.Reset();
	sampler.Reset();
}

void Sprite::Initialize()
{
	transparency = 1;
	vertexBufferPosition = D3DXVECTOR2(0, 0);
	auto& devices = Devices::Get();
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *compiledShader = nullptr;
	//バーテックスシェーダー作成
	if (dimension == Dimension2)
	{
		MakeShader("Resources/HLSL/Sprite.hlsl", "VS2D", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader);
	}
	else
	{
		MakeShader("Resources/HLSL/Sprite.hlsl", "VS3D", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader);
	}


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
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer.ReleaseAndGetAddressOf());



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
	if (dimension == Dimension3)
	CreateVertexBuffer3D();

	return S_OK;
}

void Sprite::Render()
{
	auto& devices = Devices::Get();

	devices.Context()->VSSetShader(vertexShader.Get(), nullptr, 0);
	devices.Context()->PSSetShader(pixelShader.Get(), nullptr, 0);

	SetConstantBuffer();


	//頂点インプットレイアウトをセット
	devices.Context().Get()->IASetInputLayout(vertexLayout.Get());
	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	devices.Context().Get()->PSSetSamplers(0, 1, sampler.GetAddressOf());
	devices.Context().Get()->PSSetShaderResources(0, 1, ShadowMap::GetInstance()->GetShaderResourceView().GetAddressOf());	//プリミティブをレンダリング

	//バーテックスバッファーをセット
	UINT stride = sizeof(VertexData);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1,vertexBuffer.GetAddressOf(), &stride, &offset);

	devices.Context().Get()->Draw(4, 0);

}

void Sprite::SetConstantBuffer()
{
	auto& devices = Devices::Get();

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		cb.wvp = GetWVP();
		D3DXMatrixTranspose(&cb.wvp, &cb.wvp);
		cb.transparency = transparency;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer.Get(), 0);
	}


	//このコンスタントバッファーをどのシェーダーで使うか
	devices.Context().Get()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

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
	float shiftX = static_cast<float>(widthSize / 2);
	float shiftY = static_cast<float>(hightSize / 2);
	//バーテックスバッファー作成
	//頂点を定義
	VertexData vertices[] =
	{
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x - shiftX) / width,(vertexBufferPosition.y - shiftY) / hight,0)), D3DXVECTOR2(0,1) },//頂点1	
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x - shiftX) / width,(vertexBufferPosition.y + shiftY) / hight, 0)), D3DXVECTOR2(0, 0) }, //頂点2
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x + shiftX) / width, (vertexBufferPosition.y - shiftY) / hight, 0)), D3DXVECTOR2(1, 1) },  //頂点3
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x + shiftX) / width,  (vertexBufferPosition.y + shiftY) / hight, 0)), D3DXVECTOR2(1, 0) },//頂点4	
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
