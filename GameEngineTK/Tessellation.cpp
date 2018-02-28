#include "Tessellation.h"


Tessellation::Tessellation()
{
}


Tessellation::~Tessellation()
{
}

HRESULT Tessellation::InitD3D()
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
	


	return S_OK;

}

//シェーダーを作成　頂点レイアウトを定義
HRESULT Tessellation::InitShader()
{
	ID3DBlob *pCompiledShader = nullptr;
	//バーテックスシェーダー作成
	//ブロブからバーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader)))return E_FAIL;
	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成 
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))return FALSE;
	//ドメインシェーダー
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "DS", "ds_5_0", (void**)&m_pDomainShader, &pCompiledShader)))return E_FAIL;
	//ハルシェーダー作成 
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "HS", "hs_5_0", (void**)&m_pHullShader, &pCompiledShader)))return E_FAIL;
	//ピクセルシェーダ作成
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader)))return E_FAIL;
	if (pCompiledShader)
	{
		pCompiledShader->Release();
		pCompiledShader = nullptr;
	}
	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBuffer)))return E_FAIL;




	return S_OK;
}

//バーテックスバッファー作成
HRESULT Tessellation::InitPolygon()
{
	//コントロールポイントを定義
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-1, 1,0),
		D3DXVECTOR3(1, 1,0),
		D3DXVECTOR3(-1, -1, 0),
	};
	//上のコントロールポイントでバーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}


void Tessellation::Render(std::unique_ptr<FollowCamera>& camera)
{
	D3DXMATRIX mWorld;
	D3DXMATRIX mView = camera->GetView();
	D3DXMATRIX mProj = camera->GetProjection();
	//D3DXMatrixIdentity(&mWorld);
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixRotationY(&mWorld, timeGetTime() / 1000.0f);//単純にyaw回転させる

	//シェーダーの登録
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(m_pHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(m_pDomainShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = mWorld*mView*mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}

	//このコンスタントバッファーを、どのシェーダーで使うかを指定
	pDeviceContext->DSSetConstantBuffers(0, 1, &m_pConstantBuffer);//ドメインシェーダーで使う

																	 //バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//プリミティブをレンダリング
	pDeviceContext->Draw(3, 0);

}