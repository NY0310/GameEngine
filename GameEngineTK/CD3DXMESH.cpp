#include "CD3DXMESH.h"

using namespace std;

/// <summary>
/// コンストラクタ
/// </summary>
CD3DXMESH::CD3DXMESH()
{
	ZeroMemory(this, sizeof(CD3DXMESH));
	birthcnt = 0;
	m_fScale = 1.0f;
	m_Texture = false;
}

/// <summary>
/// デストラクタ
/// </summary>
CD3DXMESH::~CD3DXMESH()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pD3d9);

}

HRESULT CD3DXMESH::Init(LPSTR FileName)
{

	if (FAILED(InitDx9()))
	{
		return E_FAIL;
	}
	if (FAILED(LoadXMesh(FileName)))
	{
		return E_FAIL;
	}

	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	return S_OK;
}

/// <summary>
/// Dx9の初期化処理
/// D3DXのパーサーを使うためには、Dx9のデバイスが必要なので作成する。
/// </summary>
/// <returns>成功、失敗原因</returns>
HRESULT CD3DXMESH::InitDx9()
{

	auto& device = Devices::Get();


	// 「Direct3D」オブジェクトの作成
	if (NULL == (m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBoxA(0, "Direct3D9の作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	//「DIRECT3Dデバイス」オブジェクトの作成
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;



	if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBoxA(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBoxA(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}


	return S_OK;
}

HRESULT CD3DXMESH::LoadXMesh(LPSTR FileName)
{
	auto& devices = Devices::Get();


	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromXA(FileName, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		m_pDevice9, NULL, &pD3DXMtrlBuffer, NULL,
		&m_dwNumMaterial, &m_pMesh)))
	{
		MessageBoxA(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}

	//この時点で、ファイルから取り出したメッシュデータが、Dx9のD3DXメッシュに入っている、
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//あとは、そこから好きな情報を取り出してDx11の自前メッシュに利用するだけ。
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterial = new MY_MATERIAL[m_dwNumMaterial];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];

	for (DWORD i = 0; i<m_dwNumMaterial; i++)
	{
		//アンビエント
		m_pMaterial[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterial[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterial[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterial[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//ディフューズ
		m_pMaterial[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterial[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterial[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterial[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//スペキュラー
		m_pMaterial[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterial[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterial[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterial[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;
		//テクスチャーがあれば
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			m_Texture = true;
			strcpy(m_pMaterial[i].szTextureName, d3dxMaterials[i].pTextureFilename);
			//テクスチャーを作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(),
				m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))
			{
				return E_FAIL;
			}
		}
		
	}

	//インデックスバッファーを作成

	//メッシュの属性情報を得る。属性情報でインデックスバッファーから細かいマテリアルごとのインデックスバッファを分離できる
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr)))
	{
		MessageBoxA(0, "属性テーブル取得失敗", "", MB_OK);
		return E_FAIL;
	}
	//D3DXMESHの場合、ロックしないとD3DXインデックスバッファーから取り出せないのでロックする。
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);

	//属性ごとのインデックスバッファを作成
	for (DWORD i = 0; i<m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;
		//Dx9のD3DMESHのインデックスバッファーからの情報で、Dx11のインデックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pAttrTable[i].FaceCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];//大きいインデックスバッファ内のオフセット(*3を忘れずに）

		if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			return FALSE;
		}
		m_pMaterial[m_AttrID[i]].dwNumFace = pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;
	m_pMesh->UnlockIndexBuffer();

	pD3DXMtrlBuffer->Release();

	//バーテックスバッファーを作成

	//D3DXMESHの場合、ロックしないとD3DXバーテックスバッファーから取り出せないのでロックする。
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MY_VERTEX_TEX* pvVertex = NULL;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pvVertex = (MY_VERTEX_TEX*)pVertices;
		//Dx9のD3DMESHのバーテックスバッファーからの情報で、Dx11のバーテックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pvVertex;

		//テクスチャー座標がマイナス対策
		if (m_Texture)
		{
			for (int i = 0; i<m_pMesh->GetNumVertices(); i++)
			{
				if (pvVertex[i].vTex.x<0)
				{
					pvVertex[i].vTex.x += 1;
				}
				if (pvVertex[i].vTex.y<0)
				{
					pvVertex[i].vTex.y += 1;
				}
			}
		}

		if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
			return FALSE;

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return S_OK;
}




HRESULT CD3DXMESH::InitShader()
{
	auto& devices = Devices::Get();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	
	//ブロブからバーテックスシェーダー作成
	if (m_Texture)
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			if (pErrors != NULL)
			{
				string error_str((char*)pErrors->GetBufferPointer());
				OutputDebugStringA((char*)pErrors->GetBufferPointer());
			}
			if (pErrors) pErrors->Release();
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS_NoTeX", "vs_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}
	}
		
	SAFE_RELEASE(pErrors);

	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	

	//頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[3];
	if (m_Texture)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = 3;
		memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = 2;
		memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}

	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if (m_Texture)
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS_NoTex", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//コンスタントバッファー作成 変換行列渡し
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}

	//コンスタントバッファー作成  マテリアル渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer2)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CD3DXMESH::Render(unique_ptr<FollowCamera>& camera , D3DXVECTOR3& vLight)
{
	auto& devices = Devices::Get();


	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixScaling(&mScale, m_fScale, m_fScale, m_fScale);
	D3DXMatrixRotationY(&mYaw, m_fYaw);
	D3DXMatrixRotationX(&mPitch, m_fPitch);
	D3DXMatrixRotationZ(&mRoll, m_fRoll);
	D3DXMatrixTranslation(&mTran, m_vPos.x, m_vPos.y, m_vPos.z);

	mWorld = mScale*mYaw*mPitch*mRoll*mTran;
	D3DXMATRIX View;
	D3DXMATRIX Proj;




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
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SIMPLECONSTANT_BUFFER0 sg;
		//ワールド行列を渡す
		sg.mW = mWorld;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);
		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = mWorld*View*Proj;
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
		//ライトの方向を渡す
		sg.vLightDir = D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);
		//視点位置を渡す
		sg.vEyes = D3DXVECTOR4(camera->GetEyepos().x, camera->GetEyepos().y, camera->GetEyepos().z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
	}
	m_pSampleLinear = NULL;
	////テクスチャーをシェーダーに渡す
	//devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
	//devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);

	//このコンスタントバッファーを使うシェーダーの登録
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	//属性の数だけ、それぞれの属性のインデックスバッファ－を描画
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		//使用されていない対策
		if (m_pMaterial[m_AttrID[i]].dwNumFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		devices.Context().Get()->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient = m_pMaterial[m_AttrID[i]].Ambient;//アンビエントををシェーダーに渡す
			sg.vDiffuse = m_pMaterial[m_AttrID[i]].Diffuse;//ディフューズカラーをシェーダーに渡す
			sg.vSpecular = m_pMaterial[m_AttrID[i]].Specular;//スペキュラーをシェーダーに渡す
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER1));
			devices.Context().Get()->Unmap(m_pConstantBuffer2, 0);
		}
		devices.Context().Get()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer2);
		devices.Context().Get()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer2);
		
		m_pMaterial[m_AttrID[i]].pTexture = nullptr;
		if (m_pMaterial[m_AttrID[i]].pTexture)
		{
			devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
			devices.Context().Get()->PSSetShaderResources(0, 1, &m_pMaterial[m_AttrID[i]].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			devices.Context().Get()->PSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブをレンダリング
		devices.Context().Get()->DrawIndexed(m_pMaterial[m_AttrID[i]].dwNumFace * 3, 0, 0);

	}



}
