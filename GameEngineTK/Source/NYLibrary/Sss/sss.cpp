#include "sss.h"

using namespace DirectX::SimpleMath;

sss::sss()
{
	modelAngele = 0.0f;
	angle = 0.0f;
	m_vLightPos = D3DXVECTOR3(0.0f, -5.0f, 0.0f);

}


sss::~sss()
{
}

HRESULT sss::InitD3D()
{
	//深度テクスチャ （レンダーターゲット）
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	tdesc.Width = DEPTHTEX_WIDTH;
	tdesc.Height = DEPTHTEX_HEIGHT;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.MiscFlags = 0;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	pDevice->CreateTexture2D(&tdesc, nullptr, &m_pDepthTexture);

	//深度テクスチャ用　レンダーターゲットビュー作成（レンダリング時用）
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	if (FAILED(pDevice->CreateRenderTargetView(m_pDepthTexture, &DescRT, &m_pDepthRTV)))
	{
		MessageBox(0, L"RTV fail", L"", MB_OK);
		return E_FAIL;
	}
	//深度テクスチャ用　シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = tdesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(pDevice->CreateShaderResourceView(m_pDepthTexture, &SRVDesc, &m_pDepthTextureView)))
	{
		MessageBox(0, L"SRV fail", L"", MB_OK);
		return E_FAIL;
	}
	//深度テクスチャ用　深度ステンシルビューテクスチャー作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = DEPTHTEX_WIDTH;
	descDepth.Height = DEPTHTEX_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthDepthStencil);

	//そのDSV作成
	pDevice->CreateDepthStencilView(m_pDepthDepthStencil, NULL, &m_pDepthDSV);

	//シェーダー初期化
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	m_pMesh = new MY_MESH;
	m_pMesh->dwNumFace = 0;
	if (FAILED(InitStaticMesh("dog.obj", m_pMesh)))
	{
		return E_FAIL;
	}

	return S_OK;
}



HRESULT sss::InitShader()
{
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//SSSシェーダー 初期化
	//バーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader[0],&pCompiledShader)))return E_FAIL;
	//頂点インプットレイアウト定義
	D3D11_INPUT_ELEMENT_DESC layout[]=
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウト作成
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout[0])))return FALSE;
	//ピクセルシェーダ作成
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "PS", "ps_5_0",(void**)&m_pPixelShader[0], &pCompiledShader)))return E_FAIL;


	//深度テクスチャ作成
	//バーテックスシェーダー
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "VS_Depth", "vs_5_0", (void**)&m_pVertexShader[1], &pCompiledShader)))return E_FAIL;
	//ピクセルシェーダー
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "PS_Depth", "ps_5_0", (void**)&m_pPixelShader[1], &pCompiledShader)))return E_FAIL;
	//深度テクスチャ用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pSamLinear);
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_SSS);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBuffer[0])))return E_FAIL;

	return S_OK;
}


void sss::ZTexRender(std::unique_ptr<FollowCamera>& camera)
{


	////////ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);
	pDeviceContext->OMSetRenderTargets(1, &m_pDepthRTV, m_pDepthDSV);

	float ClearColor[4] = { 0,0,1,1 };
	pDeviceContext->ClearRenderTargetView(m_pDepthRTV, ClearColor);
	pDeviceContext->ClearDepthStencilView(m_pDepthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	m_mView = camera->GetView();
	m_mProj = camera->GetProjection();


	//モデルワールド行列 
	D3DXMATRIX mR;
	D3DXMatrixIdentity(&mR);
	modelAngele += 0.01f;
	D3DXMatrixRotationY(&mR, modelAngele);
	//D3DXMatrixTranslation(&mR, 0.0f, 2.0f, 0.0f);

	m_pMesh->mWorld = mR;
	// ビュートランスフォーム ここではライトからの視界
	//ライトを回転させる
	D3DXMATRIX mLight;
	D3DXVECTOR3 vLight = m_vLightPos;
	D3DXMatrixRotationY(&mR, angle);
	//angle += 0.01f;
	D3DXVec3TransformCoord(&vLight, &vLight, &mR);


	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_SSS cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド・ライトビュー・プロジェクション行列をシェーダーに渡す
		D3DXMATRIX s;//重要！　丸め誤差によるテクセルの端でのアーティファクト防止
		D3DXMatrixScaling(&s, 3.1, 3.1, 3.1);
		cb.mWLP = s*m_pMesh->mWorld*mLight*m_mProj;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer[0], 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexLayout[0]);
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(SIMPLE_VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//使用するシェーダーの登録	
	pDeviceContext->VSSetShader(m_pVertexShader[1], NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader[1], NULL, 0);
	//プリミティブをレンダリング
	pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);

}

void sss::Render(std::unique_ptr<FollowCamera>& camera)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_SSS cb;

	//シェーダーのコンスタントバッファーに各種データを渡す
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド行列をシェーダーに渡す
		cb.mW = m_pMesh->mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		//ワールド・ビュー・プロジェクション行列をシェーダーに渡す
		cb.mWVP = m_pMesh->mWorld*m_mView*m_mProj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//ワールド・”ライトビュー”・プロジェクション行列をシェーダーに渡す
		cb.mWLP = m_pMesh->mWorld*mLight*m_mProj;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);
		//射影空間からテクスチャ座標空間へ変換する行列を渡す
		D3DXMATRIX m;
		ZeroMemory(&m, sizeof(D3DXMATRIX));
		m._11 = 0.5;
		m._22 = -0.5;
		m._33 = 1;
		m._41 = 0.5;
		m._42 = 0.5;
		m._44 = 1;
		cb.mWLPT = m_pMesh->mWorld*mLight*m_mProj*m;
		D3DXMatrixTranspose(&cb.mWLPT, &cb.mWLPT);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3DXVECTOR4 mat(0, 0, 0, 0);
		mat = D3DXVECTOR4(0.0, 0.0, 0.0, 1);
		cb.Ambient = mat;
		mat = D3DXVECTOR4(0.0, 1.0, 0.0, 1);
		cb.Diffuse = mat;
		mat = D3DXVECTOR4(0.5, 0.5, 0.5, 1);
		cb.Specular = mat;
		//ライトの位置をシェーダーに渡す
		cb.vLightPos = vLight;//vLightは上で、回転させたあとのライト現在位置

							  //視線ベクトルをエフェクト（シェーダー）に通知・適用
		cb.vEye = camera->GetEyePos();
		//どこまで透過するかの距離
		cb.g_Transparent = 2.4;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer[0], 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	//１パス目で作った深度テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSamLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pDepthTextureView);
	//バーテックスバッファーをセット
	UINT stride = sizeof(SIMPLE_VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexLayout[0]);
	//使用するシェーダーの登録	
	pDeviceContext->VSSetShader(m_pVertexShader[0], NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader[0], NULL, 0);
	//プリミティブをレンダリング	
	pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);
	

}



HRESULT sss::InitStaticMesh(LPSTR FileName, MY_MESH* pMesh)
{
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	char key[190] = { 0 };
	//ファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//事前に頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//頂点
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	SIMPLE_VERTEX* pvVertexBuffer = new SIMPLE_VERTEX[pMesh->dwNumVert];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[pMesh->dwNumVert];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

													  //本読み込み	
	fseek(fp, SEEK_SET, 0);
	DWORD dwVCount = 0;//読み込みカウンター
	DWORD dwVNCount = 0;//読み込みカウンター
	DWORD dwFCount = 0;//読み込みカウンター

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

		//フェイス 読み込み→頂点インデックスに
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			dwFCount++;
			//頂点構造体に代入
			pvVertexBuffer[v1 - 1].vPos = pvCoord[v1 - 1];
			pvVertexBuffer[v1 - 1].vNorm = pvNormal[vn1 - 1];
			pvVertexBuffer[v2 - 1].vPos = pvCoord[v2 - 1];
			pvVertexBuffer[v2 - 1].vNorm = pvNormal[vn2 - 1];
			pvVertexBuffer[v3 - 1].vPos = pvCoord[v3 - 1];
			pvVertexBuffer[v3 - 1].vNorm = pvNormal[vn3 - 1];
		}
	}

	fclose(fp);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SIMPLE_VERTEX) *pMesh->dwNumVert;
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
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}