#include "InkObj.h"


using namespace std;

InkObj::InkObj()
{
	vertexShader.Reset();
	pixelShader.Reset();
	constantBuffer.Reset();
	vertexLayout.Reset();
	triangles.clear();
}




HRESULT InkObj::CreateShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(MakeShader("Resources/HLSL/InkObj.hlsl", "VS", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
	CreateVertexInputLayout(pCompiledShader);
	//ブロブからピクセルシェーダー作成
	if (FAILED(MakeShader("Resources/HLSL/InkObj.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

	return S_OK;
}


HRESULT InkObj::CreateVertexInputLayout(ID3DBlob *compiledshader)
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(device->CreateInputLayout(layout, numElements, compiledshader->GetBufferPointer(), compiledshader->GetBufferSize(), &vertexLayout)))
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



void InkObj::Initialize()
{
	//シェーダー作成
	CreateShader();
	//コンスタントバッファ作成
	constantBuffer = CreateConstantBuffer(sizeof(InkData));
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


			Triangle tri;
			ComputeTriangle((pvVertexBuffer[dwFCount * 3]), pvVertexBuffer[dwFCount * 3 + 1], pvVertexBuffer[dwFCount * 3 + 2], &tri);


			triangles.emplace_back(tri);

		}
		//フェイス 読み込み→頂点インデックスに
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d %d %d", &v1, &v2, &v3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
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


	//一時的な入れ物は、もはや不要
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}




void InkObj::Render(vector<InkData> inkDataList)
{
	//使用するシェーダーの登録	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	//このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(vertexLayout.Get());
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

	for (auto data : inkDataList)
	{

		////ワールドトランスフォーム
		static float x = 0;
		x += 0.00001;
		D3DXMATRIX World;


		Render(data);
	}

}

void InkObj::Render(InkData inkData)
{

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	InkData cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMatrixTranspose(&cb.wvp ,&inkData.wvp);

		cb.color = inkData.color;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(InkData));
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}
	//プリミティブをレンダリング
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);

}
