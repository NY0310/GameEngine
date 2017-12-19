#include "OBJ.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

float RabdomRange(float min_value, float max_value)
{
	return min_value + (max_value - min_value) * rand() / RAND_MAX;
}


OBJ::OBJ()
{
	m_vLightPos = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	ZeroMemory(&m_mClipToUV, sizeof(D3DXMATRIX));
	m_mClipToUV._11 = 0.5;
	m_mClipToUV._22 = -0.5;
	m_mClipToUV._33 = 1;
	m_mClipToUV._41 = 0.5;
	m_mClipToUV._42 = 0.5;
	m_mClipToUV._44 = 1;



	line = new Line();
	line->InitD3D();


	// メンバ変数初期化
	world = D3DXVECTOR3(0, 0, 0);

	//地面上のランダムな位置に配置
	world.x = RabdomRange(-1, 1);
	world.y += 1.0f;
	world.z = RabdomRange(-1, 1);

}

void OBJ::SetPosition(const D3DXVECTOR3& position) {
	world = position;
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
	device->CreateTexture2D(&tdesc, nullptr, &m_pDepthMap_Tex);

	//深度マップテクスチャー用　レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(m_pDepthMap_Tex, &DescRT, &m_pDepthMap_TexRTV);





	//深度マップテクスチャ用　シェーダーリソースビュー(SRV)作成	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = tdesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(m_pDepthMap_Tex, &SRVDesc, &m_pDepthMap_TexSRV);

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

	device->CreateTexture2D(&descDepth, nullptr, &m_pDepthMap_DSTex);


	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	device->CreateDepthStencilView(m_pDepthMap_DSTex, nullptr, &m_pDepthMap_DSTexDSV);




	//インク用テクスチャを作成
	D3D11_TEXTURE2D_DESC inkdesc;
	ZeroMemory(&inkdesc, sizeof(D3D11_TEXTURE2D_DESC));
	inkdesc.Width = DEPTHTEX_WIDTH;
	inkdesc.Height = DEPTHTEX_HEIGHT;
	inkdesc.MipLevels = 1;
	inkdesc.ArraySize = 1;
	inkdesc.MiscFlags = 0;
	inkdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	inkdesc.SampleDesc.Count = 1;
	inkdesc.SampleDesc.Quality = 0;
	inkdesc.Usage = D3D11_USAGE_DEFAULT;
	inkdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	inkdesc.CPUAccessFlags = 0;

	device->CreateTexture2D(&inkdesc, nullptr, &inkTex);

	//インクテクスチャー用　レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC inkDescRT;
	inkDescRT.Format = inkdesc.Format;
	inkDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	inkDescRT.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(inkTex, &inkDescRT, &inkTexRTV);


	//深度テクスチャ用　深度ステンシルビューテクスチャー作成
	descDepth;
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
	device->CreateTexture2D(&descDepth, nullptr, &inkTexDS);

	////インク用テクスチャ　シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC inkSRVDesc;
	ZeroMemory(&inkSRVDesc, sizeof(inkSRVDesc));
	inkSRVDesc.Format = inkdesc.Format;
	inkSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	inkSRVDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(inkTex, &inkSRVDesc, &inkTexSRV);



	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	device->CreateDepthStencilView(inkTexDS, nullptr, &inkDSTexDSV);



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
	//深度テクスチャ用バーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("Geometry.hlsl", "VS_Depth", "vs_5_0", (void**)&m_pDepthVertexShader, &pCompiledShader)))return E_FAIL;
	if (FAILED(shadermanager.MakeShader("Geometry.hlsl", "PS_Depth", "ps_5_0", (void**)&m_pDepthPixelShader, &pCompiledShader)))return E_FAIL;

	SAFE_RELEASE(pCompiledShader);


	//インクテクスチャ用バーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("Campus.hlsl", "VS", "vs_5_0", (void**)&inkVertexShader, &pCompiledShader)))return E_FAIL;
	//インクテクスチャ用頂点インプットレイアウトをセット
	D3D11_INPUT_ELEMENT_DESC inkInputLayout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(inkInputLayout) / sizeof(inkInputLayout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device().Get()->CreateInputLayout(inkInputLayout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &inkVertexLayout)))return E_FAIL;
	//インクテクスチャ用ピクセルシェーダー作成
	if (FAILED(shadermanager.MakeShader("Campus.hlsl", "PS", "ps_5_0", (void**)&inkPixelShader, &pCompiledShader)))return E_FAIL;



	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, nullptr, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	//Zテクスチャ用コンスタントバッファー作成
	D3D11_BUFFER_DESC Zcb;
	Zcb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Zcb.ByteWidth = sizeof(ZTEXTURE_CONSTANT_BUFFER);
	Zcb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Zcb.MiscFlags = 0;
	Zcb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(devices.Device().Get()->CreateBuffer(&Zcb, nullptr, &m_pZTexConstantBuffer)))
	{
		return E_FAIL;
	}


	//インク用テクスチャ用コンスタントバッファ
	D3D11_BUFFER_DESC Inkcb;
	Inkcb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Inkcb.ByteWidth = sizeof(InkData);
	Inkcb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Inkcb.MiscFlags = 0;
	Inkcb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(devices.Device().Get()->CreateBuffer(&Inkcb, nullptr, &inkConstantBuffer)))
	{
		return E_FAIL;
	}


	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	device->CreateSamplerState(&SamDesc, &m_pSampleLimear);


	//ポリゴン作成
	if (FAILED(InitStaticMesh("Circle.obj", &m_Mesh)))
	{
		return E_FAIL;
	}
	//テクスチャー作成
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), "Hand_ColorMap.bmp", NULL, NULL, &m_pTexture, NULL)))
	{
		return E_FAIL;
	}

	//インクテクスチャを作成	
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), "pink1.png", nullptr, nullptr, &inkTexture, nullptr)))
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


void OBJ::CreateInkVertexBuffer(InkData& inkdata)
{



	//インクサイズを正規デバイス座標系にする
	float uvSize = inkdata.Scale;
	D3DXVECTOR2 Uv = inkdata.Uv;
	//
	//Square*  square = new Square(Geometry::VertexElement::Uv);
	//D3DXVECTOR3 position[] = {
	//	ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y - uvSize,0)),
	//	ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y + uvSize, 0)),
	//	ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y - uvSize,0)),
	//	ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y + uvSize,0)),
	//};

	//D3DXVECTOR2 tex[] = {
	//	D3DXVECTOR2(0,0),
	//	D3DXVECTOR2(1,1),
	//	D3DXVECTOR2(1,1),
	//	D3DXVECTOR2(1,0),
	//};

	CampusVertex vertex[] = {
		{ ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y - uvSize,0)),D3DXVECTOR2(0,1) },
		{ ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y + uvSize, 0)),	D3DXVECTOR2(0,0) },
		{ ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y - uvSize,0)),	D3DXVECTOR2(1,1) },
		{ ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y + uvSize,0)), 	D3DXVECTOR2(1,0) },
	};



	//上の頂点でバーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CampusVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertex;
	ID3D11Buffer*  canvasVertexBuffer;
	device->CreateBuffer(&bd, &InitData, &canvasVertexBuffer);
	inkdata.vertexBuffer = canvasVertexBuffer;

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
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			strcpy_s(m_Material.szName, key);
		}
		//Ka アンビエント
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ka = v;
		}
		//Kb ディヒューズ
		if (strcmp(key, "Kd") == 0)
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

void OBJ::Render(unique_ptr<FollowCamera>& camera)
{
	auto& devices = Devices::Get();





	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());




	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->GetEyePos());

	//auto& devices = Devices::Get();


	//D3DXMATRIX View;
	//D3DXMATRIX Proj;
	////ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//使用するシェーダーの登録	
	devices.Context().Get()->VSSetShader(m_pVertexShader, nullptr, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, nullptr, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//birthcnt+= 10;
		//ワールドトランスフォームは個々で異なる
		D3DXMATRIX Scale, Tran, Rot;
		//ワールド行列計算
		ObjScale = D3DXVECTOR3(0.1, 0.1, 0.1);
		D3DXMatrixScaling(&World, ObjScale.x, ObjScale.y, ObjScale.z);
		//World *= Scale;

		static float rotY = 0;
		rotY += 0.001f;
		////ワールドトランスフォーム（絶対座標変換）
		D3DXMatrixRotationY(&Rot, rotY);//単純にyaw回転させる
		World *= Rot;

		localPosition = Vector3(1, 1, 1);
		D3DXMatrixTranslation(&Tran, world.x, world.y, world.z);

		World *= Tran;
		worldmat = World;

		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = World  *View * Proj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		WVP = m;
		//ワールド、ライトビュー、射影行列を渡す
		cb.mWLP = World * mLight * Proj;

		//ワールド、ライトビュー、射影行列、テクスチャ行列
		cb.mWLPT = World * mLight * Proj * m_mClipToUV;

		//ライトの方向を渡す
		D3DXVECTOR3 vLightDir(-1, 0, -1);
		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);

		////ディフューズカラーを渡す
		//cb.vDiffuse = m_Material.Kd;
		////スペキュラーを渡す
		//cb.vSpecular = m_Material.Ks;

		//視点位置を渡す
		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);

	}
	//テクスチャーをシェーダーに渡す
	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLimear);
	devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);
	devices.Context().Get()->PSSetShaderResources(1, 1, &inkTexSRV);//全インクをレンダリングしたテクスチャ
	devices.Context().Get()->PSSetShaderResources(2, 1, &m_pDepthMap_TexSRV);//ライトビューでの深度テクスチャ作成
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

	line->Render(camera);
}


void OBJ::InkRender(unique_ptr<FollowCamera>& camera)
{
	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());

	//////ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);
	deviceContext->OMSetRenderTargets(1, &inkTexRTV, inkDSTexDSV);

	float ClearColor[4] = { 0,0,1,1 };
	deviceContext->ClearRenderTargetView(inkTexRTV, ClearColor);
	deviceContext->ClearDepthStencilView(inkDSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

																				  //このパスで使用するシェーダーの登録
	deviceContext->VSSetShader(inkVertexShader, nullptr, 0);
	deviceContext->PSSetShader(inkPixelShader, nullptr, 0);



	//テクスチャーをシェーダーに渡す
	deviceContext->PSSetSamplers(0, 1, &m_pSampleLimear);
	deviceContext->PSSetShaderResources(0, 1, &inkTexture);

	//このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, &inkConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &inkConstantBuffer);
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(inkVertexLayout);
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



	for (auto ink : inkData)
	{

		//インクの色を渡す
		D3DXVECTOR4 color = Colors::Green;
		//ink.Color = color;
		this->InkRender(camera, ink);
	}




}


void OBJ::InkRender(unique_ptr<FollowCamera>& camera, InkData& ink)
{
	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());





	D3D11_MAPPED_SUBRESOURCE pData;
	InkDataBuffer cb;
	if (SUCCEEDED(deviceContext->Map(inkConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//cb.Uv = ink.Uv;
		D3DXVECTOR4 color = Colors::Blue;
		cb.Color = ink.Color;
		//cb.Scale = ink.Scale;
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(InkDataBuffer));
		deviceContext->Unmap(inkConstantBuffer, 0);

	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(CampusVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &ink.vertexBuffer, &stride, &offset);

	//プリミティブをレンダリング
	deviceContext->Draw(4, 0);


}


void OBJ::ZTextureRender(unique_ptr<FollowCamera>& camera)
{
	////////ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);
	deviceContext->OMSetRenderTargets(1, &m_pDepthMap_TexRTV, m_pDepthMap_DSTexDSV);

	float ClearColor[4] = { 0,0,1,1 };
	deviceContext->ClearRenderTargetView(m_pDepthMap_TexRTV, ClearColor);
	deviceContext->ClearDepthStencilView(m_pDepthMap_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア




	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());



	//このパスで使用するシェーダーの登録
	deviceContext->VSSetShader(m_pDepthVertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pDepthPixelShader, nullptr, 0);
	//レンダリングターゲットを深度テクスチャに変更
	deviceContext->OMSetRenderTargets(1, &m_pDepthMap_TexRTV, m_pDepthMap_DSTexDSV);


	//// ビュートランスフォーム ここではライトからの視界
	////ライトを回転させる
	mLight;
	D3DXVECTOR3 vLight = m_vLightPos;


	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	ZTEXTURE_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(m_pZTexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		//ワールド、ライト、射影行列を渡す
		D3DXMATRIX m = worldmat * mLight * Proj;

		D3DXMatrixTranspose(&m, &m);
		cb.mWLP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ZTEXTURE_CONSTANT_BUFFER));
		deviceContext->Unmap(m_pZTexConstantBuffer, 0);

	}
	m_pSampleLimear = nullptr;
	//テクスチャーをシェーダーに渡す
	deviceContext->PSSetSamplers(0, 1, &m_pSampleLimear);
	deviceContext->PSSetShaderResources(2, 1, &m_pDepthMap_TexSRV);
	//このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, &m_pZTexConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pZTexConstantBuffer);
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	deviceContext->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//プリミティブをレンダリング
	deviceContext->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);



}





void OBJ::Render(std::unique_ptr<FollowCamera>& camera, D3DXVECTOR3&& worldPosition)
{
	D3DXMATRIX worldMatrix;
	D3DXMatrixTranslation(&worldMatrix, worldPosition.x, worldPosition.y, worldPosition.z);
	this->Render(camera);
	//this->Render(camera,inkTexture, worldMatrix);

	Matrix m;

}


//--------------------------------------------------------------------------------------
// 地形と線分の交差判定
// segment : 線分
// （出力）inter : 交点（ポリゴンの平面上で、点との再接近点の座標を返す）
//--------------------------------------------------------------------------------------
bool OBJ::IntersectSegment(const Segment& segment, unique_ptr<FollowCamera>& camera)
{

	if (m_Mesh.pIndexBuffer == nullptr) return false;

	// ヒットフラグを初期化
	bool hit = false;
	// 大きい数字で初期化
	float distance = 1.0e5;
	// 角度判定用に地面とみなす角度の限界値<度>
	const float limit_angle = XMConvertToRadians(30.0f);
	Vector3 l_inter;


	// 逆行列を計算
	D3DXMATRIX d3dWorldLocal;
	D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldmat);
	Matrix WorldLocal = shadermanager.D3DXMATRIXToMatrix(d3dWorldLocal);


	// コピー
	Segment localSegment = segment;
	// 線分をワールド座標からモデル座標系に引き込む
	localSegment.Start = Vector3::Transform(localSegment.Start, WorldLocal);
	localSegment.End = Vector3::Transform(localSegment.End, WorldLocal);
	// 線分の方向ベクトルを取得
	Vector3 segmentNormal = localSegment.End - localSegment.Start;
	segmentNormal.Normalize();

	// 全ての三角形について
	for (auto triangle : triangles)
	{
		float temp_distance;
		Vector3 temp_inter;

		// 上方向ベクトルと法線の内積
		// 長さが１のベクトル２同士の内積は、コサイン（ベクトルの内積の定義より）
		float cosine = -segmentNormal.Dot(triangle.Normal);
		//// コサイン値から、上方向との角度差を計算
		//float angle = acosf(cosine);
		//// 上方向との角度が限界角より大きければ、面の傾きが大きいので、地面とみなさずスキップ
		//if ( angle > limit_angle ) continue;

		//--高速版--
		const float limit_cosine = cosf(limit_angle);
		// コサインが１のときにベクトル間の角度は0度であり、ベクトルの角度差が大きいほど、コサインは小さいので、
		// コサイン値のまま比較すると、角度の比較の場合と大小関係が逆である
		// つまり、コサイン値が一定値より小さければ、面の傾きが大きいので、地面とみなさずスキップ
		//if (cosine < limit_cosine) continue;
		//--高速版ここまで--
		// 線分と三角形（ポリゴン）の交差判定
		if (CheckSegment2Triangle(localSegment, triangle, &temp_inter))
		{
			//hit = true;
			// 線分の始点と衝突点の距離を計算（めりこみ距離）
			temp_distance = Vector3::Distance(localSegment.Start, temp_inter);
			// めりこみ具合がここまでで最小なら
			{
				// 衝突点の座標、めりこみ距離を記録
				l_inter = temp_inter;
				distance = temp_distance;


				Vector3 p1 = triangle.P0;
				Vector3 p2 = triangle.P1;
				Vector3 p3 = triangle.P2;
				Vector3 p = l_inter;

				Vector2 uv1 = triangle.Uv0;
				Vector2 uv2 = triangle.Uv1;
				Vector2 uv3 = triangle.Uv2;

				//塗られるオブジェクトのワールド座標をかける
				//PerspectiveCollect(透視投影を考慮したUV補間)
				Matrix mvp = shadermanager.D3DXMATRIXToMatrix(worldmat) * camera->GetView()* camera->GetProjection();
				//各点をProjectionSpaceへの変換
				Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 0));
				Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 0));
				Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 0));
				Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 0));


				//通常座標への変換(ProjectionSpace)
				Vector2 p1_n = Vector2(p1_p.x, p1_p.y) / p1_p.w;
				Vector2 p2_n = Vector2(p2_p.x, p2_p.y) / p2_p.w;
				Vector2 p3_n = Vector2(p3_p.x, p3_p.y) / p3_p.w;
				Vector2 p_n = Vector2(p_p.x, p_p.y) / p_p.w;


				//頂点のなす三角形を点pにより3分割し、必要になる面積を計算
				float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
				float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
				float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
				//面積比からuvを補間
				float u = s1 / s;
				float v = s2 / s;
				float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
				Vector2 uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);

				InkData inkdata;
				inkdata.Uv = shadermanager.VectorToD3DXVECTOR2(uv);

				CreateInkVertexBuffer(inkdata);

				inkData.emplace_back(inkdata);
				return true;
			}
		}
	}



	return hit;
}



////--------------------------------------------------------------------------------------
//// 地形と円の交差判定
//// segment : 線分
//// （出力）inter : 交点（ポリゴンの平面上で、点との再接近点の座標を返す）
////--------------------------------------------------------------------------------------
bool OBJ::IntersectSphere(const Sphere& sphere, unique_ptr<FollowCamera>& camera)
{

	if (m_Mesh.pIndexBuffer == nullptr) return false;
	// 逆行列を計算
	D3DXMATRIX d3dWorldLocal;
	D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldmat);
	Matrix WorldLocal = shadermanager.D3DXMATRIXToMatrix(d3dWorldLocal);

	// ヒットフラグを初期化
	bool hit = false;
	// 大きい数字で初期化
	float over_length = 1.0e5;
	Vector3 l_inter;
	Vector3 l_normal;
	//	Vector3 l_down;
	// スケールを取得
	float scale = ObjScale.x;

	// 球をコピー
	Sphere localsphere = sphere;

	// スケール0の場合、判定しない
	if (scale <= 1.0e-10) return false;

	// 球の中心点をワールド座標からモデル座標系に引き込む
	localsphere.Center = Vector3::Transform(sphere.Center, WorldLocal);
	// 半径をワールドをワールド座標系からモデル座標系に変換
	localsphere.Radius = sphere.Radius / scale;
	// 三角形の数
	int nTri = m_Mesh.dwNumFace;
	// 全ての三角形について
	for (int i = 0; i < nTri; i++)
	{
		float temp_over_length;
		Vector3 temp_inter;

		Triangle& tri = triangles[i];

		// 三角形と球の当たり判定
		if (CheckSphere2Triangle(localsphere, tri, &temp_inter))
		{

			Vector3 p1 = triangles[i].P0;
			Vector3 p2 = triangles[i].P1;
			Vector3 p3 = triangles[i].P2;
			Vector3 p = localPosition;

			Vector2 uv1 = triangles[i].Uv0;
			Vector2 uv2 = triangles[i].Uv1;
			Vector2 uv3 = triangles[i].Uv2;

			//塗られるオブジェクトのワールド座標をかける
			//PerspectiveCollect(透視投影を考慮したUV補間)
			Matrix mvp = shadermanager.D3DXMATRIXToMatrix(worldmat) * camera->GetView()* camera->GetProjection();
			//各点をProjectionSpaceへの変換
			Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 1));
			Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 1));
			Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 1));
			Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 1));


			//通常座標への変換(ProjectionSpace)
			Vector2 p1_n = Vector2(p1_p.x, p1_p.y) / p1_p.w;
			Vector2 p2_n = Vector2(p2_p.x, p2_p.y) / p2_p.w;
			Vector2 p3_n = Vector2(p3_p.x, p3_p.y) / p3_p.w;
			Vector2 p_n = Vector2(p_p.x, p_p.y) / p_p.w;


			//頂点のなす三角形を点pにより3分割し、必要になる面積を計算
			float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
			float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
			float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
			//面積比からuvを補間
			float u = s1 / s;
			float v = s2 / s;
			float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
			Vector2 uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);

			InkData inkdata;
			//	inkdata.Uv = shadermanager.VectorToD3DXVECTOR2(uv);

			//	inkData.emplace_back(inkdata);
			return true;
		}
	}
	return hit;
}

void OBJ::MouseRay(unique_ptr<FollowCamera>& camera, unique_ptr<Player>&  player)
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	if (mouse->IsTriggered(MouseUtil::Button::Left))
	{
		Vector3 pos;
		int x = mouse->GetPos().x;
		int y = mouse->GetPos().y;
		Matrix view = camera->GetView();
		Matrix proj = camera->GetProjection();
		CalcScreenToXZ(&pos, x, y, 800, 600, &view, &proj);
		Segment segment;
		segment.End = camera->GetEyePos();
		segment.Start = pos;
		line->SetVertex(shadermanager.VectorToD3DXVECTOR3(segment.End), shadermanager.VectorToD3DXVECTOR3(segment.Start));
		IntersectSegment(segment, camera);
	}
	mouse = nullptr;
}


// XZ平面とスクリーン座標の交点算出関数
Vector3* OBJ::CalcScreenToXZ(
	Vector3* pout,
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	Matrix* View,
	Matrix* Prj
) {
	Vector3 nearpos;
	Vector3 farpos;
	Vector3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	ray.Normalize();

	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0) {
		// 床交点
		float Lray = ray.Dot(Vector3(0, 1, 0));
		float LP0 = (-nearpos).Dot(Vector3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}

	return pout;
}












Vector4 OBJ::MatrixTimes(const Matrix& matrix, const Vector4& vector)
{
	Vector4 vec;
	vec.x = matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * vector.w;
	vec.y = matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * vector.w;
	vec.z = matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * vector.w;
	vec.w = matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * vector.w;
	return vec;
}


// スクリーン座標をワールド座標に変換
Vector3* OBJ::CalcScreenToWorld(
	Vector3* pout,
	float Sx,  // スクリーンX座標
	float Sy,  // スクリーンY座標
	float fZ,  // 射影空間でのZ値（0〜1）
	float Screen_w,
	float Screen_h,
	Matrix* View,
	Matrix* Prj
) {
	// 各行列の逆行列を算出
	Matrix InvView, InvPrj, VP, InvViewport;
	//D3DXMatrixInverse(&InvView, NULL, View);
	//	↑の代わり
	XMMATRIX XMview = XMLoadFloat4x4(View);
	InvView = XMMatrixInverse(nullptr, XMview);

	//D3DXMatrixInverse(&InvPrj, NULL, Prj);
	//	↑の代わり
	XMMATRIX XMprj = XMLoadFloat4x4(Prj);
	InvPrj = XMMatrixInverse(nullptr, XMprj);

	//D3DXMatrixIdentity(&VP);
	//	↑の代わり
	VP = XMMatrixIdentity();

	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	//D3DXMatrixInverse(&InvViewport, NULL, &VP);
	XMMATRIX XMvp = XMLoadFloat4x4(&VP);
	InvViewport = XMMatrixInverse(nullptr, XMvp);

	//// 逆変換
	Matrix tmp = InvViewport * InvPrj * InvView;
	//D3DXVec3TransformCoord(pout, &Vector3(Sx, Sy, fZ), &tmp);

	XMVECTOR vec = XMLoadFloat3(&Vector3(Sx, Sy, fZ));

	XMMATRIX XMtmp = XMLoadFloat4x4(&tmp);

	XMStoreFloat3(pout, XMVector3TransformCoord(vec, XMtmp));

	return pout;
}


