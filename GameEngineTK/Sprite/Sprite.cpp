#include "Sprite.h"


Sprite::Sprite()
{
	scale = Vector3::One;
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

HRESULT Sprite::Initialize()
{
	auto& devices = Devices::Get();

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *compiledShader = nullptr;
	D3D11_BLEND_DESC bd;
	//バーテックスシェーダー作成
	//if (dimension == Dimension2)
	//{
	//	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/Sprite.hlsl", "VS2D", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;
	//}
	//else
	//{
		if (FAILED(ShaderManager::MakeShader("Resources/HLSL/Sprite.hlsl", "VS3D", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;
	//}


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



	//ピクセルシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/Sprite.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;
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



	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, sampler.ReleaseAndGetAddressOf());

	return S_OK;

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

	//バーテックスバッファー作成
	CreateVertexBuffer();
}

void Sprite::Render()
{
	auto& devices = Devices::Get();

	Matrix World, Scale, Tran, Rot, View, Proj, wvp;

	View = camera->GetView();
	Proj = camera->GetProjection();


	//ワールドトランスフォームは個々で異なる
	Scale = Matrix::CreateScale(scale);
	Tran = Matrix::CreateTranslation(position);
	Rot = Matrix::CreateFromYawPitchRoll(rotation.x, rotation.y, rotation.z);

	devices.Context()->VSSetShader(vertexShader.Get(), nullptr, 0);
	devices.Context()->PSSetShader(pixelShader.Get(), nullptr, 0);


	World = Tran * Scale;
	wvp = World * View * Proj;
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		wvp.Invert();
		cb.wvp = wvp;
		cb.viewPortWidth = devices.Width();
		cb.viewPortHight = devices.Height();

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer.Get(), 0);
	}


	//このコンスタントバッファーをどのシェーダーで使うか
	devices.Context().Get()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());


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
		widthHalfSize = desc.Width/2;
		hightHalfSize = desc.Height/2;
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
HRESULT Sprite::CreateVertexBuffer()
{
	auto& devices = Devices::Get();
	UINT width = devices.Width() / 2;
	UINT hight = devices.Height() / 2;
	int shiftX = widthHalfSize * scale.x;
	int shiftY = widthHalfSize * scale.y;

	Matrix Scale = Matrix::CreateScale(scale);

	//バーテックスバッファー作成
	//頂点を定義
	VertexData vertices[] =
	{
		{ Vector3((position.x - shiftX) / width,(position.y - shiftY) / hight,0), Vector2(0,1) },//頂点1	
		{ Vector3((position.x - shiftX) / width,(position.y + shiftY) / hight, 0), Vector2(0, 0) }, //頂点2
		{ Vector3((position.x + shiftX) / width, (position.y - shiftY) / hight, 0), Vector2(1, 1) },  //頂点3
		{ Vector3((position.x + shiftX) / width,  (position.y + shiftY) / hight, 0), Vector2(1, 0) },//頂点4	
	};
	////頂点を定義
	//VertexData vertices[] =
	//{
	//	{ Vector3(-1,-1,0),Vector2(0,1) },//頂点1	
	//	{ Vector3(-1, 1, 0), Vector2(0, 0) }, //頂点2
	//	{ Vector3(1, -1, 0), Vector2(1, 1) },  //頂点3
	//	{ Vector3(1, 1, 0), Vector2(1, 0) },//頂点4	
	//};


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

}
