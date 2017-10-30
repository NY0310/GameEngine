#include "OBJ.h"


OBJ::OBJ()
{

}



OBJ::~OBJ()
{
}

void OBJ::Init()
{
	shadermanager = ShaderManager::Get();
	InitD3D();

	m_vLight = D3DXVECTOR3(-1, 0, -1);
	D3DXVec3Normalize(&m_vLight, &m_vLight);

	birthcnt = 0;

}

HRESULT OBJ::InitD3D()
{
	auto& devices = Devices::Get();

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_DEBUG, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_DEBUG, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
	//ポリゴン作成
	if (FAILED(InitStaticMesh("Geometry+Normal+UV.obj", &m_Mesh)))
	{
		return E_FAIL;
	}
	//テクスチャー作成
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), "Hand_ColorMap.bmp", NULL, NULL, &m_pTexture, NULL)))
	{
		return E_FAIL;
	}





	////メッシュ作成
	//if (FAILED(InitStaticMesh("Geometry.obj", &m_Mesh)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

HRESULT OBJ::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
{
	auto& devices = Devices::Get();


	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	char key[110] = { 0 };
	while (!feof(fp))
	{
	D3DXVECTOR4 v(0, 0, 0, 0);
		//キーワード読み込み
		fscanf_s(fp, "%s", key, sizeof(key));
		//マリテリアル名
		if (strcmp(key,"newmtl")==0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			strcpy_s(m_Material.szName, key);
		}
		//Ka アンビエント
		if (strcmp(key,"Ka")==0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ka = v;
		}
		//Kb ディヒューズ
		if (strcmp(key,"Kd")==0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Kd = v;
		}
		//Ks スペキュラー
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
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), m_Material.szTextureName, NULL, NULL, &m_pTexture, NULL)))
			{
				return E_FAIL;
			}
		}

	}
	fclose(fp);

	return S_OK;
}


HRESULT OBJ::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{

	auto& devices = Devices::Get();


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

		//頂点 読み込み
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

			//pvVertexBuffer[dwFCount * 3].Pos = pvCoord[v1 - 1];
			//pvVertexBuffer[dwFCount * 3].Normal = D3DXVECTOR3(0,0,0);
			//pvVertexBuffer[dwFCount * 3].Tex = D3DXVECTOR2(0,0);
			//pvVertexBuffer[dwFCount * 3 + 1].Pos = pvCoord[v2 - 1];
			//pvVertexBuffer[dwFCount * 3 + 1].Normal = D3DXVECTOR3(0, 0, 0);
			//pvVertexBuffer[dwFCount * 3 + 1].Tex = D3DXVECTOR2(0, 0);
			//pvVertexBuffer[dwFCount * 3 + 2].Pos = pvCoord[v3 - 1];
			//pvVertexBuffer[dwFCount * 3 + 2].Normal = D3DXVECTOR3(0, 0, 0);
			//pvVertexBuffer[dwFCount * 3 + 2].Tex = D3DXVECTOR2(0, 0);

			test[dwFCount * 3 + 0] = pvVertexBuffer[dwFCount * 3 + 0];
			test[dwFCount * 3 + 1] = pvVertexBuffer[dwFCount * 3 + 1];
			test[dwFCount * 3 + 2] = pvVertexBuffer[dwFCount * 3 + 2];

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
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//インデックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;

	//一時的な入れ物は、もはや不要
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}

void OBJ::Render(std::unique_ptr<FollowCamera>& camera)
{
	auto& devices = Devices::Get();
	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());




	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->GetEyePos());

	//auto& devices = Devices::Get();


	D3DXMATRIX World;
	//D3DXMATRIX View;
	//D3DXMATRIX Proj;
	////ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, x, 1, 0);
	World = Tran;


	//D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->m_eyepos);
	//D3DXVECTOR3 vLookatPt = shadermanager.VectorToD3DXVECTOR3(camera->m_refpos);
	//D3DXVECTOR3 vUpVec = shadermanager.VectorToD3DXVECTOR3(camera->m_upvec);


	//D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	//D3DXMatrixPerspectiveFovRH(&Proj, camera->m_fovY, camera->m_aspect, camera->m_NearClip, camera->m_FarClip);



	//D3DXMATRIX CancelRotation = View;
	//CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0;
	//D3DXMatrixInverse(&CancelRotation, NULL, &CancelRotation);
	//World = CancelRotation * World;


	//使用するシェーダーの登録	
	devices.Context().Get()->VSSetShader(m_pVertexShader, NULL, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		birthcnt+= 10;
		//ワールドトランスフォームは個々で異なる
		D3DXMATRIX Scale, Tran, Rot;

		//ワールド行列計算
		D3DXMatrixScaling(&World, 5, 5, 5);
		//World *= Scale;
		 

		////ワールドトランスフォーム（絶対座標変換）
		D3DXMatrixRotationY(&Rot, birthcnt / 1000.0f);//単純にyaw回転させる
		World *= Rot;

		D3DXMatrixTranslation(&Tran, 0, 1, 0);

		World *= Tran;

		//ワールド行列を渡す
		//cb.mW = World;
		//D3DXMatrixTranspose(&cb.mW, &cb.mW);

		
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = World *View * Proj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		//ワールド、カメラ、射影行列を渡す
		//cb.mWVP = World*View*Proj;
		//D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

		//ライトの方向を渡す
		D3DXVECTOR3 vLightDir(-1, 0, -1);
		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);

		//ディフューズカラーを渡す
		cb.vDiffuse = m_Material.Kd;
		//スペキュラーを渡す
		cb.vSpecular = m_Material.Ks;

		//視点位置を渡す
		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);


		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);



		//memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		//devices.Context().Get()->Unmap(m_pConstantBuffer, 0);

	}
	m_pSampleLimear = nullptr;
	//m_pTexture = NULL;
	//テクスチャーをシェーダーに渡す
	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLimear);
	devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);

	//このコンスタントバッファーを使うシェーダーの登録
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	devices.Context().Get()->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//プリミティブをレンダリング
	devices.Context().Get()->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);


}
