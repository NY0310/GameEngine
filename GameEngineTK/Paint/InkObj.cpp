#include "InkObj.h"


using namespace std;
std::map < LPSTR, InkObj::MeshAndTriangles> InkObj::modelDatas;

InkObj::InkObj()
{
	matrixInkObject = std::make_unique<MatrixObject>();//行列
	// メンバ変数初期化
	matrixInkObject->SetPosition(D3DXVECTOR3(0, 0, 0));
	matrixInkObject->SetScale(D3DXVECTOR3(10, 10, 10));
	matrixInkObject->SetRotation(D3DXVECTOR3(0, 0, 0));
}


InkObj::~InkObj()
{
}

void InkObj::Initialize()
{
	InitD3D();
	LoadOBJFile("Resources/OBJ/InkObj.obj");
}



HRESULT InkObj::CreateShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/InkObj.hlsl", "VS", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
	CreateVertexInputLayout(pCompiledShader);
	//ブロブからピクセルシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/InkObj.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

	return S_OK;
}


HRESULT InkObj::CreateVertexInputLayout(ID3DBlob *compiledshader)
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(device->CreateInputLayout(layout, numElements, compiledshader->GetBufferPointer(), compiledshader->GetBufferSize(),vertexLayout.ReleaseAndGetAddressOf())))
	{
		return FALSE;
	}

	return S_OK;

}

ID3D11Buffer* InkObj::CreateConstantBuffer(UINT size)
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


HRESULT InkObj::CreateSampler()
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

HRESULT InkObj::InitD3D()
{

	//シェーダー作成
	CreateShader();
	//サンプラー作成
	//CreateSampler();
	//コンスタントバッファ作成
	constantBuffer = CreateConstantBuffer(sizeof(ConstantBuffer));

	return S_OK;
}

void InkObj::Update()
{
	matrixInkObject->WorldMatrixCreate();
}


HRESULT InkObj::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
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


HRESULT InkObj::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{
	////既に読み込んでいるか
	//if (modelDatas.count(FileName) != 0)
	//{
	//	mesh = modelDatas[FileName].mesh;
	//}

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
		////マテリアル読み込み
		//if (strcmp(key, "mtllib") == 0)
		//{
		//	fscanf_s(fp, "%s ", key, sizeof(key));
		//	LoadMaterialFromFile(key, &material);
		//}
		//頂点
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		////法線
		//if (strcmp(key, "vn") == 0)
		//{
		//	dwVNCount++;
		//}
		////テクスチャー座標
		//if (strcmp(key, "vt") == 0)
		//{
		//	dwVTCount++;
		//}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	D3DXVECTOR3* pvVertexBuffer = new D3DXVECTOR3[pMesh->dwNumFace * 3];
	//Vector3* pvCoord = new Vector3[pMesh->dwNumVert];
	//Vector3* pvNormal = new Vector3[dwVNCount];
	//Vector2* pvTexture = new Vector2[dwVTCount];
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
			pvVertexBuffer[dwVCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			pvVertexBuffer[dwVCount].y = y;
			pvVertexBuffer[dwVCount].z = z;

			dwVCount++;
		}
		////法線 読み込み
		//if (strcmp(key, "vn") == 0)
		//{
		//	fscanf_s(fp, "%f %f %f", &x, &y, &z);
		//	pvNormal[dwVNCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
		//	pvNormal[dwVNCount].y = y;
		//	pvNormal[dwVNCount].z = z;
		//	dwVNCount++;
		//}


		////テクスチャー座標 読み込み
		//if (strcmp(key, "vt") == 0)
		//{
		//	fscanf_s(fp, "%f %f", &x, &y);
		//	pvTexture[dwVTCount].x = x;
		//	pvTexture[dwVTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
		//	dwVTCount++;
		//}
		//フェイス 読み込み→頂点インデックスに
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d// %d// %d//", &v1, &v2, &v3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			//pvVertexBuffer[dwFCount * 3].pos = pvCoord[v1 - 1];
			//pvVertexBuffer[dwFCount * 3].normal = pvNormal[vn1 - 1];
			//pvVertexBuffer[dwFCount * 3].uv = pvTexture[vt1 - 1];
			//pvVertexBuffer[dwFCount * 3 + 1].pos = pvCoord[v2 - 1];
			//pvVertexBuffer[dwFCount * 3 + 1].normal = pvNormal[vn2 - 1];
			//pvVertexBuffer[dwFCount * 3 + 1].uv = pvTexture[vt2 - 1];
			//pvVertexBuffer[dwFCount * 3 + 2].pos = pvCoord[v3 - 1];
			//pvVertexBuffer[dwFCount * 3 + 2].normal = pvNormal[vn3 - 1];
			//pvVertexBuffer[dwFCount * 3 + 2].uv = pvTexture[vt3 - 1];
			dwFCount++;
		}
	}

	fclose(fp);

	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(D3DXVECTOR3) *pMesh->dwNumFace * 3;
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

	struct MeshAndTriangles data;
	data.mesh = mesh;
	//リストにモデル情報を格納する
	modelDatas[FileName] = data;

	//一時的な入れ物は、もはや不要
	//delete pvCoord;
	//delete pvNormal;
	//delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}





void InkObj::Render(const vector<ConstantInkData> inkData)
{
	D3DXMATRIX View = Math::MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = Math::MatrixToD3DXMATRIX(camera->GetProjection());


	//使用するシェーダーの登録	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(vertexLayout.Get());
	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mesh.pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	deviceContext->IASetIndexBuffer(mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	Matrix World, Scale, Tran, Rot, wvp;

	for (auto data : inkData)
	{
		wvp.Invert();
		//ワールドトランスフォームは個々で異なる
		Scale = Matrix::CreateScale(data.scale);
		Tran = Matrix::CreateTranslation(data.positon);
		Rot = Matrix::CreateRotationX(data.rotation.x);
		World = Scale * Tran;
		wvp = World * camera->GetView() * camera->GetProjection();
		Render(wvp, data.color);
	}

}

void InkObj::Render(Matrix& wvp, Vector4& color)
{
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBuffer cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		Matrix m = wvp;
		m.Transpose();

		//m._11 = 12;
		//m._12 = 0;
		//m._13 = 0;
		//m._14 = 3.4;
		//m._21 = 0;
		//m._22 = 12;
		//m._23 = 0.18;
		//m._24 = 5.9;
		//m._31 = 0;
		//m._32 = 0.1;
		//m._33 = 4.9;
		//m._34 = 0;
		//m._41 = 0;
		//m._42 = 0.1;
		//m._43 = -6.9;
		//m._44 = 5;

		cb.wvp = m;

		cb.color = color;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer));
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}
	//このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	//プリミティブをレンダリング
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);

}