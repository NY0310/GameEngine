#include "Obj.h"


using namespace std;
using namespace NYLibrary;



std::map < LPSTR, Obj::MeshAndTriangles> Obj::modelDatas;
const float Obj::MAX_BREAK_CNT = 150.0f;//破壊カウントの上限


Obj::Obj(LPSTR FileName)
{
	ZeroMemory(&clipToUV, sizeof(D3DXMATRIX));
	clipToUV._11 = 0.5f;
	clipToUV._22 = -0.5f;
	clipToUV._33 = 1;
	clipToUV._41 = 0.5f;
	clipToUV._42 = 0.5f;
	clipToUV._44 = 1;
	LoadOBJFile(FileName);
}


Obj::~Obj()
{
}

void Obj::CreateAddChild()
{
	breakLerp = make_shared<Lerp>(0, BREAK_CNT,100.0f);
	breakLerp->DisableUpdate();
	std::function<void()> thisFunction = std::bind(&Obj::OnBreakEnd, this);
	breakLerp->addListener(thisFunction);
	AddChild(breakLerp);
}

void Obj::Initialize()
{
	////シェーダー作成
	if (!vertexShader.Get())
	{
		CreateShader();
	}
	//サンプラー作成
	CreateSampler();
	//コンスタントバッファ作成
	constantBuffer = CreateConstantBuffer(sizeof(SIMPLESHADER_CONSTANT_BUFFER));
	breakTime = 0;
}



HRESULT Obj::CreateShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(MakeShader("Resources/HLSL/OBJ.hlsl", "VS", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
	CreateVertexInputLayout(pCompiledShader);
	//ブロブからピクセルシェーダー作成
	if (FAILED(MakeShader("Resources/HLSL/OBJ.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
	//ジオメトリシェーダー作成
	if (FAILED(MakeShader("Resources/HLSL/OBJ.hlsl", "GS", "gs_5_0", (void**)geometryShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

	return S_OK;
}


HRESULT Obj::CreateVertexInputLayout(ID3DBlob *compiledshader)
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

/// <summary>
/// コンスタントバッファー作成
/// </summary>
/// <param name="size">サイズ</param>
/// <returns>作成したコンスタントバッファー</returns>
ID3D11Buffer* Obj::CreateConstantBuffer(UINT size)
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


HRESULT Obj::CreateSampler()
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



HRESULT Obj::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
{
	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = nullptr;
	LPSTR upName = "Resources/MTL/";
	char* buf = new char[100];

	strcpy(buf, upName);
	strcat(buf, FileName);
	fopen_s(&fp, buf, "rt");
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
	delete[] buf;
	return S_OK;
}


HRESULT Obj::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{
	//既に読み込んでいるか
	if (modelDatas.count(FileName) != 0)
	{
		triangles = modelDatas[FileName].triangles;
		SetPolygons(triangles);
		mesh = modelDatas[FileName].mesh;
		SetLocalSize(modelDatas[FileName].localSize);
		return S_OK;
	}

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
	Triangle tri;

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			if (x == 0.0f)
			{
				pvCoord[dwVCount].x = x;
			}
			else
			{
				pvCoord[dwVCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			}
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;


			dwVCount++;
		}
		//法線 読み込み
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			if (x == 0.0f)
			{
				pvNormal[dwVNCount].x = x;
			}
			else
			{
				pvNormal[dwVNCount].x = -x;//OBJは右手座標系なのでxあるいはｚを反転
			}
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;


			tri.normal = pvNormal[dwVNCount];
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
				ComputeTriangle((pvVertexBuffer[dwFCount * 3].Pos),pvVertexBuffer[dwFCount * 3 + 1].Pos, pvVertexBuffer[dwFCount * 3 + 2].Pos, &tri);
				tri.uv0 = pvTexture[vt1 - 1];
				tri.uv1 = pvTexture[vt2 - 1];
				tri.uv2 = pvTexture[vt3 - 1];
				triangles.emplace_back(tri);




				dwFCount++;
			}


			//法線 読み込み
			if (strcmp(key, "s") == 0)
			{
				fscanf_s(fp, "%f %f %f", &x, &y ,&z);
				SetLocalSizeX(x);
				SetLocalSizeY(y);
				SetLocalSizeZ(z);
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

		struct MeshAndTriangles data;
		data.mesh = mesh;
		data.triangles = triangles;
		data.localSize = GetLocalSize();
		//リストにモデル情報を格納する
		modelDatas[FileName] = data;
		SetPolygons(triangles);
		//一時的な入れ物は、もはや不要
		delete pvCoord;
		delete pvNormal;
		delete pvTexture;
		delete[] pvVertexBuffer;
		delete[] piFaceBuffer;

		return S_OK;
}



/// <summary>
/// 描画
/// </summary>
void Obj::Render()
{

	Light* light = Light::GetInstance();
	ShadowMap* shadowMap = ShadowMap::GetInstance();

	D3DXMATRIX View = camera->GetView();
	D3DXMATRIX Proj = camera->GetProjection();



	D3DXVECTOR3 vEyePt = camera->GetEyePos();

	////ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//使用するシェーダーの登録	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		//ワールド行列を渡す
		world = GetWorldMatrix();
		D3DXMatrixTranspose(&world, &world);
		//ワールド、カメラ、射影行列を渡す
		cb.wvp = GetWVP();
		D3DXMatrixTranspose(&cb.wvp, &cb.wvp);
		//ワールド、ライトビュー、射影行列を渡す
		cb.wlp = world * mLight * Proj;
		D3DXMatrixTranspose(&cb.wlp, &cb.wlp);
		//ワールド、ライトビュー、射影行列、テクスチャ行列
		cb.wlpt = world * mLight * Proj * clipToUV;
		D3DXMatrixTranspose(&cb.wlpt, &cb.wlpt);

		//ライトの方向を渡す
		cb.lightDir = D3DXVECTOR4(light->GetPosition().x, light->GetPosition().y, light->GetPosition().z, 1.0f);

		//視点位置を渡す
		cb.eyesPos = D3DXVECTOR4(camera->GetEyePos().x,camera->GetEyePos().y,camera->GetEyePos().z,0);



		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}

	//サンプラーを渡す
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());
	//モデルのテクスチャを渡す
	deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	//深度テクスチャを渡す
	deviceContext->PSSetShaderResources(2, 1, shadowMap->GetShaderResourceView().GetAddressOf());
	//このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf() );
	deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf() );
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
	//プリミティブをレンダリング
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);
	deviceContext->GSSetConstantBuffers(0, 1, nullptr);
	deviceContext->GSSetShader(nullptr, nullptr, 0);


}

/// <summary>
/// シャドウマップにライトビューから見た深度値を出力
/// </summary>
void Obj::ShadowRender()
{
	auto light = Light::GetInstance();
	ShadowMap* shadowMap = ShadowMap::GetInstance();



	D3DXMATRIX View = camera->GetView();
	D3DXMATRIX Proj = camera->GetProjection();


	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	ShadowMap::ConstantBuffer cb;
	if (SUCCEEDED(deviceContext->Map(shadowMap->GetConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX lightView;
		lightView._11 = 0.9999f;
		lightView._12 = 0.0f;
		lightView._13 = 0.0f;
		lightView._14 = 0.0f;
		lightView._21 = 0.0f;
		lightView._22 = -4.3711;
		lightView._23 = -0.9999f;
		lightView._24 = 0.0f;
		lightView._31 = 0.0f;
		lightView._32 = 0.999f;
		lightView._33 = -4.37f;
		lightView._34 = 0.0f;
		lightView._41 = 12.99;
		lightView._42 = 1.44;
		lightView._43 = 32.999f;
		lightView._44 = 0.999f;

		D3DXMATRIX a_world = GetWorldMatrix();
		D3DXMATRIX a_light = light->GetLightView();
		D3DXMATRIX a_view = camera->GetView();
		D3DXMATRIX a_projection = camera->GetProjection();

		//	ビュー行列の生成
		D3DXVECTOR3 eyepos(31,57,7.03f);
		D3DXVECTOR3 refpos(28,55,7.06f);
		D3DXVECTOR3 upvec(0,1,0);
		D3DXMatrixLookAtLH(&a_light, &eyepos, &refpos, &upvec);

		//ワールド、ライト、射影行列を渡す
		cb.wlp = GetWorldMatrix() * light->GetLightView() * camera->GetProjection();
	//	cb.wlp = GetWorldMatrix() *  * camera->GetProjection();
		//転置行列に変換
		D3DXMatrixTranspose(&cb.wlp, &cb.wlp);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ShadowMap::ConstantBuffer));
		deviceContext->Unmap(shadowMap->GetConstantBuffer().Get(), 0);

	}
	//このコンスタントバッファーを使うシェーダーの登録
	deviceContext->VSSetConstantBuffers(0, 1, shadowMap->GetConstantBuffer().GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, shadowMap->GetConstantBuffer().GetAddressOf());
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
	//プリミティブをレンダリング
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);

}
