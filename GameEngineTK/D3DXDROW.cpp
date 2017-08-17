#include "D3DXDROW.h"

using namespace std;

D3DXDROW::D3DXDROW()
{
	birthcnt = 0;
}


D3DXDROW::~D3DXDROW()
{
}

HRESULT D3DXDROW::InitD3D()
{

	auto& devices = Devices::Get();


	//シェーダ初期化
	if (FAILED(InitShader())) {
		return E_FAIL;
	}

	//メッシュファイル読み込み
	m_d3dxread = new D3DXREAD();
	m_d3dxread->InitDx9();
	if (FAILED(m_d3dxread->LoadXMesh("Geometry+UV.x")))
	{
		return E_FAIL;
	}

	//この時点で、ファイルから取り出したメッシュデータが、Dx9のD3DXメッシュに入っている、
	//あとは、そこから好きな情報を取り出してDx11の自前メッシュに利用するだけ。
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	//バーテックスバッファーを作成

	//D3DXMESHの場合、ロックしないとD3DXバーテックスから取り出せないのでロックする。
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_d3dxread->GetpMesh()->GetVertexBuffer(&pVB);

	//DWORD dwStride = m_d3dxread->GetpMesh()->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MY_VERTEX *pvVertex = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pvVertex = (MY_VERTEX*)pVertices;
		//Dx9のD3DMESHのバーテックスバッファーからの情報で、Dx11のバーテックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) *m_d3dxread->GetpMesh()->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pvVertex;
		if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_Mesh.pVertexBuffer)))
			return FALSE;

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	//インデックスバッファーを作成

	//D3DXMESHの場合、ロックしないとD3DXインデックスバッファーから取り出せないのでロックする。
	int* pIndex = NULL;
	m_d3dxread->GetpMesh()->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);
	//Dx9のD3DMESHのインデックスバッファーからの情報で、Dx11のインデックスバッファを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * m_d3dxread->GetpMesh()->GetNumFaces() * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = pIndex;
	//InitData.SysMemPitch = 2;
	//InitData.SysMemSlicePitch = 0;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_Mesh.pIndexBuffer)))
		return FALSE;
	m_d3dxread->GetpMesh()->UnlockIndexBuffer();
	//テクスチャー作成
	if (m_d3dxread->m_TextureFileName[0][0] != NULL)
	{
		if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), m_d3dxread->m_TextureFileName[0], NULL, NULL, &m_pTexture, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;

}


HRESULT D3DXDROW::InitShader()
{
	auto& devices = Devices::Get();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;

}

void D3DXDROW::Render(unique_ptr<FollowCamera>& camera)
{
	auto& devices = Devices::Get();


	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;



	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->m_eyepos);
	D3DXVECTOR3 vLookatPt = shadermanager.VectorToD3DXVECTOR3(camera->m_refpos);
	D3DXVECTOR3 vUpVec = shadermanager.VectorToD3DXVECTOR3(camera->m_upvec);

	D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovRH(&Proj, camera->m_fovY, camera->m_aspect, camera->m_NearClip, camera->m_FarClip);



	//使用するシェーダーの登録
	devices.Context().Get()->VSSetShader(m_pVertexShader, NULL, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX Tran, Scale, Rot;
		//ワールド行列計算
		D3DXMatrixScaling(&World, 3, 3, 3);
		//World *= Scale;

		birthcnt += 10;
		////ワールドトランスフォーム（絶対座標変換）
		D3DXMatrixRotationY(&Rot, birthcnt / 1000.0f);//単純にyaw回転させる
		World *= Rot;

		D3DXMatrixTranslation(&Tran, 2, 1, 2);

		World *= Tran;

		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = World*View*Proj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
	}
	m_pSampleLinear = NULL;
	//テクスチャーをシェーダーに渡す
	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
	devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);

	//このコンスタントバッファーを使うシェーダーの登録
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	devices.Context().Get()->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//プリミティブをレンダリング
	devices.Context().Get()->DrawIndexed(m_d3dxread->GetpMesh()->GetNumFaces() * 3, 0, 0);

}
