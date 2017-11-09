#include "DisplacementMapping.h"




DisplacementMapping::DisplacementMapping()
{
}


DisplacementMapping::~DisplacementMapping()
{
}

HRESULT DisplacementMapping::InitD3D()
{
	//シェーダー初期化
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}
	//テクスチャー
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, "taisetsu.tif", NULL, NULL, &m_pHeightTexture, NULL)))
	{
		return E_FAIL;
	}
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamDesc.MaxAnisotropy = 16;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pDevice->CreateSamplerState(&SamDesc, &m_pSamPoint);

	return S_OK;
}

HRESULT DisplacementMapping::InitShader()
{
	ID3DBlob *pCompiledShader = nullptr;
	//バーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader)))return E_FAIL;
	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(layout,numElements,pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),&m_pVertexLayout)))return E_FAIL;
	//ハルシェーダー作成
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "HS", "hs_5_0", (void**)&m_pHullShader, &pCompiledShader)))return E_FAIL;
	//ドメインシェーダー作成 
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "DS", "ds_5_0", (void**)&m_pDomainShader, &pCompiledShader)))return E_FAIL;
	//ピクセルシェーダ 
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader)))return E_FAIL;
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}


	return S_OK;
}

HRESULT DisplacementMapping::InitPolygon()
{
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-1, 0,1),D3DXVECTOR2(0,0),
		D3DXVECTOR3(1, 0,1),D3DXVECTOR2(0,1),
		D3DXVECTOR3(1, 0,-1),D3DXVECTOR2(1,1),
		D3DXVECTOR3(-1, 0, -1),D3DXVECTOR2(1,0),
	};

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))return E_FAIL;

	return S_OK;
}

void DisplacementMapping::Render(std::unique_ptr<FollowCamera>& camera)
{
	D3DXMATRIX mWorld;
	D3DXMATRIX mView = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX mProj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixIdentity(&mWorld);
	//ワールドトランスフォーム（絶対座標変換）
//	D3DXMatrixRotationY(&mWorld, timeGetTime() / 1000.0f);//単純にyaw回転させる
	//シェーダーの登録　	
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(m_pHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(m_pDomainShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	//シェーダーにコンスタントバッファーを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX m = mWorld*mView*mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;


		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//テクスチャをドメインシェーダーに渡す 
	pDeviceContext->DSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->DSSetShaderResources(0, 1, &m_pHeightTexture);
	//テクスチャをピクセルシェーダに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pHeightTexture);
	//コンスタントバッファーをシェーダに渡す
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->DSSetConstantBuffers(0, 1, &m_pConstantBuffer);//ドメインシェーダーで使う
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	//プリミティブをレンダリング
	pDeviceContext->Draw(4, 0);

}
