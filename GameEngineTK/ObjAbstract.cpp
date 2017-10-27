//#include "ObjAbstract.h"
//
//
//ObjAbstract::ObjAbstract()
//{
//}
//
//
//ObjAbstract::~ObjAbstract()
//{
//}
//
//
//
//HRESULT ObjAbstract::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
//{
//
//	//マテリアルファイルを開いて内容を読み込む
//	FILE* fp = nullptr;
//	fopen_s(&fp, FileName, "rt");
//
//	char key[110] = { 0 };
//	while (!feof(fp))
//	{
//		D3DXVECTOR4 v(0, 0, 0, 0);
//		//キーワード読み込み
//		fscanf_s(fp, "%s", key, sizeof(key));
//		//マリテリアル名
//		if (strcmp(key, "newmtl") == 0)
//		{
//			fscanf_s(fp, "%s", key, sizeof(key));
//			strcpy_s(m_Material.szName, key);
//		}
//		//Ka アンビエント
//		if (strcmp(key, "Ka") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
//			m_Material.Ka = v;
//		}
//		//Kb ディヒューズ
//		if (strcmp(key, "Kd") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
//			m_Material.Kd = v;
//		}
//		//Ks スペキュラー
//		if (strcmp(key, "Ks") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
//			m_Material.Ks = v;
//		}
//		//map_Kd　テクスチャー
//		if (strcmp(key, "map_Kd") == 0)
//		{
//			fscanf_s(fp, "%s", &m_Material.szTextureName, sizeof(m_Material.szTextureName));
//			//テクスチャー作成
//			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, m_Material.szTextureName, nullptr, nullptr, &m_pTexture, nullptr)))
//			{
//				return E_FAIL;
//			}
//		}
//
//	}
//	fclose(fp);
//
//	return S_OK;
//}
//
//
//
//
//
//HRESULT ObjAbstract::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
//{
//
//	float x = 0, y = 0, z = 0;
//	int v1 = 0, v2 = 0, v3 = 0;
//	int vn1 = 0, vn2 = 0, vn3 = 0;
//	int vt1 = 0, vt2 = 0, vt3 = 0;
//	DWORD dwVCount = 0;//読み込みカウンター
//	DWORD dwVNCount = 0;//読み込みカウンター
//	DWORD dwVTCount = 0;//読み込みカウンター
//	DWORD dwFCount = 0;//読み込みカウンター
//	pMesh->dwNumVert = 0;
//	pMesh->dwNumFace = 0;
//
//
//	char key[200] = { 0 };
//	//OBJファイルを開いて内容を読み込む
//	FILE* fp = NULL;
//	fopen_s(&fp, FileName, "rt");
//
//	//事前に頂点数、ポリゴン数を調べる
//	while (!feof(fp))
//	{
//		//キーワード読み込み
//		fscanf_s(fp, "%s ", key, sizeof(key));
//		//マテリアル読み込み
//		if (strcmp(key, "mtllib") == 0)
//		{
//			fscanf_s(fp, "%s ", key, sizeof(key));
//			LoadMaterialFromFile(key, &m_Material);
//		}
//		//頂点
//		if (strcmp(key, "v") == 0)
//		{
//			pMesh->dwNumVert++;
//		}
//		//法線
//		if (strcmp(key, "vn") == 0)
//		{
//			dwVNCount++;
//		}
//		//テクスチャー座標
//		if (strcmp(key, "vt") == 0)
//		{
//			dwVTCount++;
//		}
//		//フェイス（ポリゴン）
//		if (strcmp(key, "f") == 0)
//		{
//			pMesh->dwNumFace++;
//		}
//	}
//
//	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
//	SimpleVertex* pvVertexBuffer = new SimpleVertex[pMesh->dwNumFace * 3];
//	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
//	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
//	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];
//	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
//
//													  //本読み込み	
//	fseek(fp, SEEK_SET, 0);
//	dwVCount = 0;
//	dwVTCount = 0;
//	dwVNCount = 0;
//	dwFCount = 0;
//
//	SimpleVertex test[6000];
//
//	while (!feof(fp))
//	{
//		//キーワード 読み込み
//		ZeroMemory(key, sizeof(key));
//		fscanf_s(fp, "%s ", key, sizeof(key));
//
//		//頂点 読み込み
//		if (strcmp(key, "v") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &x, &y, &z);
//			pvCoord[dwVCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
//			pvCoord[dwVCount].y = y;
//			pvCoord[dwVCount].z = z;
//
//			dwVCount++;
//		}
//		//法線 読み込み
//		if (strcmp(key, "vn") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &x, &y, &z);
//			pvNormal[dwVNCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
//			pvNormal[dwVNCount].y = y;
//			pvNormal[dwVNCount].z = z;
//			dwVNCount++;
//		}
//
//
//		//テクスチャー座標 読み込み
//		if (strcmp(key, "vt") == 0)
//		{
//			fscanf_s(fp, "%f %f", &x, &y);
//			pvTexture[dwVTCount].x = x;
//			pvTexture[dwVTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
//			dwVTCount++;
//		}
//		//フェイス 読み込み→頂点インデックスに
//		if (strcmp(key, "f") == 0)
//		{
//			//fscanf_s(fp, "%d// %d// %d//", &v1, &v2, &v3);
//			fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
//			piFaceBuffer[dwFCount * 3] = dwFCount * 3;
//			piFaceBuffer[dwFCount * 3 + 1] = dwFCount * 3 + 1;
//			piFaceBuffer[dwFCount * 3 + 2] = dwFCount * 3 + 2;
//			//頂点構造体に代入
//			pvVertexBuffer[dwFCount * 3].Pos = pvCoord[v1 - 1];
//			pvVertexBuffer[dwFCount * 3].Normal = pvNormal[vn1 - 1];
//			pvVertexBuffer[dwFCount * 3].Tex = pvTexture[vt1 - 1];
//			pvVertexBuffer[dwFCount * 3 + 1].Pos = pvCoord[v2 - 1];
//			pvVertexBuffer[dwFCount * 3 + 1].Normal = pvNormal[vn2 - 1];
//			pvVertexBuffer[dwFCount * 3 + 1].Tex = pvTexture[vt2 - 1];
//			pvVertexBuffer[dwFCount * 3 + 2].Pos = pvCoord[v3 - 1];
//			pvVertexBuffer[dwFCount * 3 + 2].Normal = pvNormal[vn3 - 1];
//			pvVertexBuffer[dwFCount * 3 + 2].Tex = pvTexture[vt3 - 1];
//
//			test[dwFCount * 3 + 0] = pvVertexBuffer[dwFCount * 3 + 0];
//			test[dwFCount * 3 + 1] = pvVertexBuffer[dwFCount * 3 + 1];
//			test[dwFCount * 3 + 2] = pvVertexBuffer[dwFCount * 3 + 2];
//
//			dwFCount++;
//		}
//	}
//
//	fclose(fp);
//
//	//バーテックスバッファーを作成
//	D3D11_BUFFER_DESC bd;
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(SimpleVertex) *pMesh->dwNumFace * 3;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.MiscFlags = 0;
//	D3D11_SUBRESOURCE_DATA InitData;
//	InitData.pSysMem = pvVertexBuffer;
//	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
//		return FALSE;
//
//	//インデックスバッファーを作成
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.MiscFlags = 0;
//	InitData.pSysMem = piFaceBuffer;
//	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
//		return FALSE;
//
//	//一時的な入れ物は、もはや不要
//	delete pvCoord;
//	delete pvNormal;
//	delete pvTexture;
//	delete[] pvVertexBuffer;
//	delete[] piFaceBuffer;
//
//
//
//	return S_OK;
//}
