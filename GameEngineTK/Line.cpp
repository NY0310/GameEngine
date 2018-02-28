#include "Line.h"


Line::Line()
{
}


Line::~Line()
{
}

HRESULT Line::InitD3D()
{
	ID3DBlob *pCompiledShader = nullptr;
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/Line.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader)))return E_FAIL;
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(device->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/Line.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader)))return E_FAIL;

	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(device->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	InitModel();
}

HRESULT Line::InitModel()
{
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.2f, 0.7f, 0.0f),
		D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(device->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	return S_OK;
}

void Line::Render()
{
	//使用するシェーダーのセット
	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	D3DXMATRIX Tran;
	D3DXMATRIX World;

	D3DXMatrixTranslation(&Tran, 0, 0, 0);
	World = Tran;
	// プロジェクショントランスフォーム
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = World *  camera->GetView()*  camera->GetProjection();
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		deviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(m_pVertexLayout);
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//プリミティブをレンダリング
	deviceContext->Draw(2, 0);

}

void Line::SetVertex(D3DXVECTOR3 pos, D3DXVECTOR3 pos1)
{
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(pos.x,pos.y,pos.z - 1),D3DXVECTOR3(pos1.x,pos1.y,pos1.z),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	device->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);	//バーテックスバッファーをセット

}
