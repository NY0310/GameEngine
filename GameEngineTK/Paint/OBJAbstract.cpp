#include "OBJAbstract.h"


using namespace std;

OBJAbstract::OBJAbstract()
{
	m_vLightPos = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	ZeroMemory(&m_mClipToUV, sizeof(D3DXMATRIX));
	m_mClipToUV._11 = 0.5;
	m_mClipToUV._22 = -0.5;
	m_mClipToUV._33 = 1;
	m_mClipToUV._41 = 0.5;
	m_mClipToUV._42 = 0.5;
	m_mClipToUV._44 = 1;

	// メンバ変数初期化
	world = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	rotation = D3DXVECTOR3(0, 0, 0);
	IsUseQuternion = false;
}


OBJAbstract::~OBJAbstract()
{
}

void OBJAbstract::Init()
{
	InitD3D();

	m_vLight = D3DXVECTOR3(-1, 0, -1);
	D3DXVec3Normalize(&m_vLight, &m_vLight);
}

/// <summary>
/// 全行列作成
/// </summary>
void OBJAbstract::AllMatrixCreate()
{
	D3DXMatrixTranslation(&worldMatrix, this->world.x, this->world.y, this->world.z);
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
	if (IsUseQuternion)
	{
		D3DXMatrixRotationQuaternion(&rotationMatrix, &quaternion);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.x, rotation.y, rotation.z);
	}
}

HRESULT OBJAbstract::CreateShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "VS", "vs_5_0", (void**)&vertexShader, &pCompiledShader)))return E_FAIL;
	CreateVertexInputLayout(pCompiledShader);
	//ブロブからピクセルシェーダー作成
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "PS", "ps_5_0", (void**)&pixelShader, &pCompiledShader)))return E_FAIL;

	return S_OK;
}

HRESULT OBJAbstract::CreateDepthTextureShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//深度テクスチャ用バーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "VS_Depth", "vs_5_0", (void**)&depthVertexShader, &pCompiledShader)))return E_FAIL;
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "PS_Depth", "ps_5_0", (void**)&depthPixelShader, &pCompiledShader)))return E_FAIL;
	return S_OK;
}

HRESULT OBJAbstract::CreateVertexInputLayout(ID3DBlob *compiledshader)
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(device->CreateInputLayout(layout, numElements, compiledshader->GetBufferPointer(), compiledshader->GetBufferSize(), &vertexLayout)))
	{
		return FALSE;
	}

	return S_OK;

}

ID3D11Buffer* OBJAbstract::CreateConstantBuffer(UINT size )
{
	ID3D11Buffer* buffer;
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = size;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&cb, nullptr, &buffer);
	
	return buffer;
}

HRESULT OBJAbstract::CreateDepthTexture()
{
	//深度マップテクスチャーを作成
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
	device->CreateTexture2D(&tdesc, nullptr, &depthMapTex);

	//深度マップテクスチャー用　レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(depthMapTex, &DescRT, &depthMapTexRTV);





	//深度マップテクスチャ用　シェーダーリソースビュー(SRV)作成	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = tdesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(depthMapTex, &SRVDesc, &depthMapTexSRV);

	//深度マップテクスチャをレンダーターゲットにする際のデプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
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

	device->CreateTexture2D(&descDepth, nullptr, &depthMapDSTex);


	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	device->CreateDepthStencilView(depthMapDSTex, nullptr, &depthMapDSTexDSV);

	return S_OK;

}

HRESULT OBJAbstract::CreateSampler()
{
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	device->CreateSamplerState(&SamDesc, &sampleLimear);

	

	return S_OK;

}

HRESULT OBJAbstract::InitD3D()
{

	//シェーダー作成
	CreateShader();
	//深度シェーダー作成
	CreateDepthTextureShader();
	//深度テクスチャ作成
	CreateDepthTexture();
	//サンプラー作成
	CreateSampler();
	//コンスタントバッファ作成
	constantBuffer = CreateConstantBuffer(sizeof(SIMPLESHADER_CONSTANT_BUFFER));
	//深度テクスチャコンスタントバッファ作成
	zTexConstantBuffer = CreateConstantBuffer(sizeof(ZTEXTURE_CONSTANT_BUFFER));

	return S_OK;
}




HRESULT OBJAbstract::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
{
	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rt");

	char key[110] = { 0 };
	while (!feof(fp))
	{
		D3DXVECTOR4 v(0, 0, 0, 0);
		//キーワード読み込み
		fscanf_s(fp, "%s", key, sizeof(key));
		//マリテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			strcpy_s(material.szName, key);
		}
		//Ka アンビエント
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			material.Ka = v;
		}
		//Kb ディヒューズ
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			material.Kd = v;
		}
		//Ks スペキュラー
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			material.Ks = v;
		}
		//map_Kd　テクスチャー
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &material.szTextureName, sizeof(material.szTextureName));
			//テクスチャー作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device, material.szTextureName, nullptr, nullptr, &texture, nullptr)))
			{
				return E_FAIL;
			}
		}

	}
	fclose(fp);

	return S_OK;
}


HRESULT OBJAbstract::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{

	float x = 0, y = 0, z = 0;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD dwVCount = 0;//読み込みカウンター
	DWORD dwVNCount = 0;//読み込みカウンター
	DWORD dwVTCount = 0;//読み込みカウンター
	DWORD dwFCount = 0;//読み込みカウンター
	pMesh->dwNumVert = 0;
	pMesh->dwNumFace = 0;


	char key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = nullptr;
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
			LoadMaterialFromFile(key, &material);
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
	SimpleVertex* pvVertexBuffer = new SimpleVertex[pMesh->dwNumFace * 3];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

													  //本読み込み	
	fseek(fp, SEEK_SET, 0);
	dwVCount = 0;
	dwVTCount = 0;
	dwVNCount = 0;
	dwFCount = 0;

	SimpleVertex test[6000];

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;

			dwVCount++;
		}
		//法線 読み込み
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}


		//テクスチャー座標 読み込み
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[dwVTCount].x = x;
			pvTexture[dwVTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
			dwVTCount++;
		}
		//フェイス 読み込み→頂点インデックスに
		if (strcmp(key, "f") == 0)
		{
			//fscanf_s(fp, "%d// %d// %d//", &v1, &v2, &v3);
			fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			piFaceBuffer[dwFCount * 3] = dwFCount * 3;
			piFaceBuffer[dwFCount * 3 + 1] = dwFCount * 3 + 1;
			piFaceBuffer[dwFCount * 3 + 2] = dwFCount * 3 + 2;
			//頂点構造体に代入
			pvVertexBuffer[dwFCount * 3].Pos = pvCoord[v1 - 1];
			pvVertexBuffer[dwFCount * 3].Normal = pvNormal[vn1 - 1];
			pvVertexBuffer[dwFCount * 3].Tex = pvTexture[vt1 - 1];
			pvVertexBuffer[dwFCount * 3 + 1].Pos = pvCoord[v2 - 1];
			pvVertexBuffer[dwFCount * 3 + 1].Normal = pvNormal[vn2 - 1];
			pvVertexBuffer[dwFCount * 3 + 1].Tex = pvTexture[vt2 - 1];
			pvVertexBuffer[dwFCount * 3 + 2].Pos = pvCoord[v3 - 1];
			pvVertexBuffer[dwFCount * 3 + 2].Normal = pvNormal[vn3 - 1];
			pvVertexBuffer[dwFCount * 3 + 2].Tex = pvTexture[vt3 - 1];

			Triangle tri;
			ComputeTriangle(shadermanager.D3DXVECTOR3ToVector(pvVertexBuffer[dwFCount * 3].Pos), shadermanager.D3DXVECTOR3ToVector(pvVertexBuffer[dwFCount * 3 + 1].Pos), shadermanager.D3DXVECTOR3ToVector(pvVertexBuffer[dwFCount * 3 + 2].Pos), &tri);
			tri.Uv0 = shadermanager.D3DXVECTOR2ToVector(pvTexture[vt1 - 1]);
			tri.Uv1 = shadermanager.D3DXVECTOR2ToVector(pvTexture[vt2 - 1]);
			tri.Uv2 = shadermanager.D3DXVECTOR2ToVector(pvTexture[vt3 - 1]);


			triangles.emplace_back(tri);
			dwFCount++;
		}
	}

	fclose(fp);

	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) *pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(device->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//インデックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	if (FAILED(device->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;



	//一時的な入れ物は、もはや不要
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}




void OBJAbstract::Render()
{
	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());



	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->GetEyePos());

	////ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//使用するシェーダーの登録	
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		AllMatrixCreate();
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		world = scaleMatrix * rotationMatrix *  worldMatrix;
		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = world  *View * Proj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//ワールド、ライトビュー、射影行列を渡す
		cb.mWLP = world * mLight * Proj;

		//ワールド、ライトビュー、射影行列、テクスチャ行列
		cb.mWLPT = world * mLight * Proj * m_mClipToUV;

		//ライトの方向を渡す
		D3DXVECTOR3 vLightDir(-1, 0, -1);
		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);

		//視点位置を渡す
		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(constantBuffer, 0);

	}
	//テクスチャーをシェーダーに渡す
	deviceContext->PSSetSamplers(0, 1, &sampleLimear);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	//deviceContext->PSSetShaderResources(2, 1, &depthMapTexSRV);//ライトビューでの深度テクスチャ作成
															   //このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(vertexLayout);
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mesh.pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	deviceContext->IASetIndexBuffer(mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//プリミティブをレンダリング
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);


}

void OBJAbstract::ZTextureRender()
{

	//////////ビューポートの設定
	//D3D11_VIEWPORT vp;
	//vp.Width = DEPTHTEX_WIDTH;
	//vp.Height = DEPTHTEX_HEIGHT;
	//vp.MinDepth = 0.0f;
	//vp.MaxDepth = 1.0f;
	//vp.TopLeftX = 0;
	//vp.TopLeftY = 0;
	//deviceContext->RSSetViewports(1, &vp);
	//deviceContext->OMSetRenderTargets(1, &depthMapTexRTV, depthMapDSTexDSV);

	//float ClearColor[4] = { 0,0,1,1 };
	//deviceContext->ClearRenderTargetView(depthMapTexRTV, ClearColor);
	//deviceContext->ClearDepthStencilView(depthMapDSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア




	//D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	//D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());



	////このパスで使用するシェーダーの登録
	//deviceContext->VSSetShader(depthVertexShader, nullptr, 0);
	//deviceContext->PSSetShader(depthPixelShader, nullptr, 0);
	////レンダリングターゲットを深度テクスチャに変更
	//deviceContext->OMSetRenderTargets(1, &depthMapTexRTV, depthMapDSTexDSV);


	////// ビュートランスフォーム ここではライトからの視界
	//////ライトを回転させる
	//mLight;
	//D3DXVECTOR3 vLight = m_vLightPos;


	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	//D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	////シェーダーのコンスタントバッファーに各種データを渡す	
	//D3D11_MAPPED_SUBRESOURCE pData;
	//ZTEXTURE_CONSTANT_BUFFER cb;
	//if (SUCCEEDED(deviceContext->Map(zTexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	//{

	//	//ワールド、ライト、射影行列を渡す
	//	D3DXMATRIX m = worldMatrix * mLight * Proj;

	//	D3DXMatrixTranspose(&m, &m);
	//	cb.mWLP = m;

	//	memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ZTEXTURE_CONSTANT_BUFFER));
	//	deviceContext->Unmap(zTexConstantBuffer, 0);

	//}
	//sampleLimear = nullptr;
	////テクスチャーをシェーダーに渡す
	//deviceContext->PSSetSamplers(0, 1, &sampleLimear);
	//deviceContext->PSSetShaderResources(2, 1, &depthMapTexSRV);
	////このコンスタントバッファーを使うシェーダーの登録
	//deviceContext->VSSetConstantBuffers(0, 1, &zTexConstantBuffer);
	//deviceContext->PSSetConstantBuffers(0, 1, &zTexConstantBuffer);
	////頂点インプットレイアウトをセット
	//deviceContext->IASetInputLayout(vertexLayout);
	////プリミティブ・トポロジーをセット
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////バーテックスバッファーをセット
	//UINT stride = sizeof(SimpleVertex);
	//UINT offset = 0;
	//deviceContext->IASetVertexBuffers(0, 1, &mesh.pVertexBuffer, &stride, &offset);
	////インデックスバッファーをセット
	//stride = sizeof(int);
	//offset = 0;
	//deviceContext->IASetIndexBuffer(mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	////プリミティブをレンダリング
	//deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);

}
