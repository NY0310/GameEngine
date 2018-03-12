#include "BumpMapping.h"


//
//D3DXVECTOR4 CalcTangent
//頂点3つとUV座標３つと法線をもとに接線を計算する。返す4次元ベクトルのwは左か右手系かの判断用フラグ。
D3DXVECTOR4 CalcTangent(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3,
	D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR3 normal)
{
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 bitangent;

	D3DXVECTOR3 edge1 = v2 - v1;
	D3DXVECTOR3 edge2 = v3 - v1;
	D3DXVec3Normalize(&edge1, &edge1);
	D3DXVec3Normalize(&edge2, &edge2);
	D3DXVECTOR2 uvEdge1 = uv2 - uv1;
	D3DXVECTOR2 uvEdge2 = uv3 - uv1;
	D3DXVec2Normalize(&uvEdge1, &uvEdge1);
	D3DXVec2Normalize(&uvEdge2, &uvEdge2);

	float det = (uvEdge1.x*uvEdge2.y) - (uvEdge1.y*uvEdge2.x);
	det = 1.0f / det;

	tangent.x = (uvEdge2.y*edge1.x - uvEdge1.y*edge2.x)*det;
	tangent.y = (uvEdge2.y*edge1.y - uvEdge1.y*edge2.y)*det;
	tangent.z = (uvEdge2.y*edge1.z - uvEdge1.y*edge2.z)*det;

	bitangent.x = (-uvEdge2.x*edge1.x + uvEdge1.x*edge2.x)*det;
	bitangent.y = (-uvEdge2.x*edge1.y + uvEdge1.x*edge2.y)*det;
	bitangent.z = (-uvEdge2.x*edge1.z + uvEdge1.x*edge2.z)*det;

	D3DXVec3Normalize(&tangent, &tangent);
	D3DXVec3Normalize(&bitangent, &bitangent);

	D3DXVECTOR3 BiNormal;
	D3DXVec3Cross(&BiNormal, &normal, &tangent);

	float w = (D3DXVec3Dot(&BiNormal, &bitangent) >= 0.0f) ? 1.0f : -1.0f;

	return D3DXVECTOR4(tangent.x, tangent.y, tangent.z, w);
}



BumpMapping::BumpMapping()
{
	m_vLight = D3DXVECTOR3(0, 0.5, -1);
	D3DXVec3Normalize(&m_vLight, &m_vLight);
}


BumpMapping::~BumpMapping()
{
}

HRESULT BumpMapping::InitD3D()
{
	//シェーダー初期化
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}
	return S_OK;

}


HRESULT BumpMapping::InitShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = nullptr;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(shadermanager.MakeShader("BumpMapping.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader)))return E_FAIL;
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))return FALSE;
	//ブロブからピクセルシェーダ作成
	if (FAILED(shadermanager.MakeShader("BumpMapping.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader)))return E_FAIL;
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
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



HRESULT BumpMapping::InitPolygon()
{
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		// 座標、法線、UV、タンジェントの順
		D3DXVECTOR3(-0.5,0,-0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,1),D3DXVECTOR4(0,0,0,0),//頂点1, タンジェントは今はずべてゼロ（あとで計算）
		D3DXVECTOR3(-0.5,0,0.5), D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),D3DXVECTOR4(0,0,0,0),//頂点2
		D3DXVECTOR3(0.5,0,-0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1),D3DXVECTOR4(0,0,0,0), //頂点3
		D3DXVECTOR3(-0.5,0,0.5),D3DXVECTOR3(0,0,-1), D3DXVECTOR2(0,0),D3DXVECTOR4(0,0,0,0), //頂点4
		D3DXVECTOR3(0.5,0,0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,0),D3DXVECTOR4(0,0,0,0),//頂点5
		D3DXVECTOR3(0.5,0,-0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1),D3DXVECTOR4(0,0,0,0), //頂点6
	};
	//接線を計算
	{
		D3DXVECTOR3 v1(-0.5, -0.5, 0);
		D3DXVECTOR3 v2(-0.5, 0.5, 0);
		D3DXVECTOR3 v3(0.5, -0.5, 0);

		D3DXVECTOR2 t1(0, 1);
		D3DXVECTOR2 t2(0, 0);
		D3DXVECTOR2 t3(1, 1);

		D3DXVECTOR3 v4(-0.5, 0.5, 0);
		D3DXVECTOR3 v5(0.5, 0.5, 0);
		D3DXVECTOR3 v6(0.5, -0.5, 0);

		D3DXVECTOR2 t4(0, 0);
		D3DXVECTOR2 t5(1, 0);
		D3DXVECTOR2 t6(1, 1);

		D3DXVECTOR3 vN(0, 0, -1);
		D3DXVECTOR4 vTangent;

		//接線計算はトライアングル単位　１つ目のトライアングル
		vTangent = CalcTangent(v1, v2, v3, t1, t2, t3, vN);
		vertices[0].vTangent = vTangent;
		vertices[1].vTangent = vTangent;
		vertices[2].vTangent = vTangent;
		//接線計算はトライアングル単位　２つ目のトライアングル
		vTangent = CalcTangent(v4, v5, v6, t4, t5, t6, vN);
		vertices[3].vTangent = vTangent;
		vertices[4].vTangent = vTangent;
		vertices[5].vTangent = vTangent;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	//ノーマルマップのテクスチャー作成
	D3DX11_IMAGE_LOAD_INFO li;
	ZeroMemory(&li, sizeof(D3DX11_IMAGE_LOAD_INFO));
	li.Width = 512;
	li.Height = 512;
	li.MipLevels = 1;
	li.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	li.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	li.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	li.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(D3DX11CreateTextureFromFile(pDevice, L"WaterBump.bmp", &li, NULL, (ID3D11Resource**)&m_pNormalTexture, NULL)))
	{
		return E_FAIL;
	}

	//ノーマルマップのテクスチャーのシェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	D3D11_TEXTURE2D_DESC desc;
	m_pNormalTexture->GetDesc(&desc);

	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	HRESULT hr = pDevice->CreateShaderResourceView(m_pNormalTexture, &srvDesc, &m_pNormalTextureSRV);

	return S_OK;
}


void BumpMapping::Render(std::unique_ptr<FollowCamera>& camera)
{

	D3DXMATRIX mWorld;
	D3DXMATRIX mView = camera->GetView();
	D3DXMATRIX mProj = camera->GetProjection();
	D3DXMatrixIdentity(&mWorld);

	//D3DXMatrixTranslation(&mWorld, 3.0f, 8.0f, -3.0f);
	//ワールドトランスフォーム（絶対座標変換）
	//D3DXMatrixRotationY(&mWorld, timeGetTime() / 1000.0f);//単純にyaw回転させる

	//使用するシェーダの登録
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE SubRes;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubRes)))
	{
		//ワールド行列を渡す
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		//ワールドの逆行列の転置行列をシェーダーに渡す
		cb.mWIT = mWorld;
		D3DXMatrixInverse(&cb.mWIT, NULL, &cb.mWIT);
		D3DXMatrixTranspose(&cb.mWIT, &cb.mWIT);
		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = mWorld*mView*mProj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//ライト方向を渡す
		cb.vLightDir = (D3DXVECTOR4)m_vLight;
		//Ambientを渡す
		cb.Ambient = D3DXVECTOR4(0.0, 0.0, 0.4, 1);
		//Diffuseを渡す
		cb.Diffuse = D3DXVECTOR4(0.0, 0.0, 0.7, 1);
		//Specularを渡す
		cb.Specular = D3DXVECTOR4(1.5, 1.5, 1.5, 1);
		//カメラの位置(視点)をシェーダーに渡す
		D3DXVECTOR3 eyepos = (camera->GetEyePos());
		cb.Specular = D3DXVECTOR4(eyepos.x, eyepos.y, eyepos.z, 0);

		//波　関連
		static D3DXVECTOR4 WaveMove(0, 0, 0, 0);
		WaveMove.x += 0.0001f;
		WaveMove.y += 0.0005f;
		//波の位置変化量を渡す
		cb.WaveMove = WaveMove;


		memcpy_s(SubRes.pData, SubRes.RowPitch, (void*)&cb, sizeof(SHADER_CONSTANT_BUFFER));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);

	}
	//このコンスタントバッファーを使うシェーダーの登録
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pNormalTextureSRV);
	//プリミティブをレンダリング
	pDeviceContext->Draw(6, 0);



}

