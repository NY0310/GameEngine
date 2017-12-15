#include "DisplacementMapping.h"




DisplacementMapping::DisplacementMapping()
{
	//WaveMove = D3DXVECTOR4(0, 0, 0, 0);
	//WaveHeight = D3DXVECTOR4(0, 0, 0, 0);

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
	//メッシュ作成
	if (FAILED(InitStaticMesh("Quad_Lo.obj", &m_Mesh)))
	{
		return E_FAIL;
	}
	//ハイトマップ
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, "WaterBump.bmp", nullptr, nullptr, &m_pNormalTexture, nullptr)))return E_FAIL;
	////深度マップテクスチャ
	//if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, "china_DensityMap.tif", nullptr, nullptr, &m_pDensityTexture, nullptr)))return E_FAIL;
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, "SnowHeight.bmp", nullptr, nullptr, &m_pSnowHeightTexture, nullptr)))
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
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

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
	if (FAILED(pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBuffer)))
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

HRESULT DisplacementMapping::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD dwVCount = 0;//読み込みカウンター
	DWORD dwVNCount = 0;//読み込みカウンター
	DWORD dwVTCount = 0;//読み込みカウンター
	DWORD dwFCount = 0;//読み込みカウンター
	pMesh->dwNumFace = 0;
	pMesh->dwNumVert = 0;
	char key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//事前に頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル読み込み
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(key, &m_Material);
		}
		//頂点
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		//法線
		if (strcmp(key, "vn") == 0)
		{
			dwVNCount++;
		}
		//テクスチャー座標
		if (strcmp(key, "vt") == 0)
		{
			dwVTCount++;
		}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	SimpleVertex* pvVertexBuffer = new SimpleVertex[pMesh->dwNumVert];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

													  //本読み込み	
	fseek(fp, SEEK_SET, 0);
	dwVCount = 0;
	dwVNCount = 0;
	dwVTCount = 0;
	dwFCount = 0;

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		//頂点 読み込み
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = x;
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;
			dwVCount++;
		}

		//法線 読み込み
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = x;
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}

		//テクスチャー座標 読み込み
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[dwVTCount].x = 1 - x;//OBJファイルはX成分が逆なので合わせる
			pvTexture[dwVTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
			dwVTCount++;
		}

		//フェイス 読み込み→頂点インデックスに
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			dwFCount++;
			//頂点構造体に代入
			pvVertexBuffer[v1 - 1].vPos = pvCoord[v1 - 1];
			pvVertexBuffer[v1 - 1].vNorm = pvNormal[vn1 - 1];
			pvVertexBuffer[v1 - 1].vTex = pvTexture[vt1 - 1];
			pvVertexBuffer[v2 - 1].vPos = pvCoord[v2 - 1];
			pvVertexBuffer[v2 - 1].vNorm = pvNormal[vn2 - 1];
			pvVertexBuffer[v2 - 1].vTex = pvTexture[vt2 - 1];
			pvVertexBuffer[v3 - 1].vPos = pvCoord[v3 - 1];
			pvVertexBuffer[v3 - 1].vNorm = pvNormal[vn3 - 1];
			pvVertexBuffer[v3 - 1].vTex = pvTexture[vt3 - 1];
		}
	}

	fclose(fp);

	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) *pMesh->dwNumVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//インデックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;

	//一時的な入れ物は、もはや不要
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}

HRESULT DisplacementMapping::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMaterial)
{
	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	char key[110] = { 0 };
	D3DXVECTOR4 v(0, 0, 0, 0);
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(m_Material.szName, key);
		}
		//Ka　アンビエント
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ka = v;
		}
		//Kd　ディフューズ
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Kd = v;
		}
		//Ks　スペキュラー
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ks = v;
		}
		//map_Kd　テクスチャー
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &m_Material.szTextureName, sizeof(m_Material.szTextureName));
			//テクスチャー作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, m_Material.szTextureName, nullptr, nullptr, &m_pTexture, nullptr)))
			{
				return E_FAIL;
			}
		}
	}
	fclose(fp);

	return S_OK;
}

void DisplacementMapping::Render(std::unique_ptr<FollowCamera>& camera)
{

	/*WaveMove.x += 0.0004f;
	WaveMove.y += 0.0002f;*/


	D3DXMATRIX mWorld;
	D3DXMATRIX mView = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX mProj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixIdentity(&mWorld);
	//ワールドトランスフォーム（絶対座標変換）
	//D3DXMatrixTranslation(&mWorld, 0, -10000, 0);
	
	D3DXMatrixRotationX(&mWorld, 2.9f);
//	D3DXMatrixScaling(&mWorld, 15, 15, 15);

	//ワールドトランスフォームは個々で異なる
	D3DXMATRIX Scale, Tran, Rot;
	D3DXMatrixScaling(&Scale, 7, 5, 5);
	D3DXMatrixTranslation(&Tran,0, -1.3, 0);
	mWorld *= Scale * Tran;
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
		//ワールド行列を渡す
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		//モデルから見た視点（つまり、モデルの逆ワールドをかけた視点）
		cb.vEyePos = shadermanager.VectorToD3DXVECTOR3(DirectX::SimpleMath::Vector3(camera->GetEyePos().x, camera->GetEyePos().y + 10, camera->GetEyePos().z));
		D3DXMATRIX Inv;
		D3DXMatrixInverse(&Inv, nullptr, &(mWorld*mView));
		//指定された行列により 3D ベクトルを変換し、その結果を w = 1 に射影する。
		D3DXVec3TransformCoord(&cb.vEyePos, &cb.vEyePos, &Inv);
		//最小距離、最大距離
		cb.fMinDistance = 0.5f;
		cb.iMaxDevide = 4.5f;
		//最大分割数
		cb.iMaxDevide = 64;
		//ライトの方向
		cb.LightDir = D3DXVECTOR4(1, 1, 1, 0);
		//雪の成長係数を渡す
		static float Glow = 0;
		Glow -= 0.0004;
		if (Glow>1.0f) Glow = 0;
		cb.Glow.x = Glow;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//テクスチャーをドメインシェーダーに渡す
	pDeviceContext->DSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->DSSetShaderResources(0, 1, &m_pNormalTexture);
	//テクスチャーをピクセルシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pNormalTexture);
	//テクスチャーをドメインシェーダーに渡す
	pDeviceContext->DSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->DSSetShaderResources(1, 1, &m_pSnowHeightTexture);

	//コンスタントバッファーをシェーダに渡す
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->DSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->HSSetConstantBuffers(0, 1, &m_pConstantBuffer);//ハルシェーダーで使う
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);//ピクセルシェーダーで使う

	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(SimpleVertex);
	offset = 0;
	pDeviceContext->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//プリミティブをレンダリング
	pDeviceContext->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);




}
