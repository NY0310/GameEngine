#include "PaintSkinMesh.h"


PaintSkinMesh::PaintSkinMesh()
{
}

PaintSkinMesh::~PaintSkinMesh()
{
}




/// <summary>
/// //初期化
/// </summary>
/// <returns>実行結果</returns>
HRESULT PaintSkinMesh::Initialize(bool isPlane)
{
	ID3D11Device* device = Devices::Get().Device().Get();
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();

	matrixObject = make_unique<MatrixObject>();
	matrixObject->SetScale(D3DXVECTOR3(5, 5, 5));
	matrixObject->SetPosition(D3DXVECTOR3(1, 2.5, 1));
	//matrixObject->SetRotation(D3DXVECTOR3(1, 1, 1));
	campus = make_unique<Paint>();
	campus->InitD3D(isPlane);
	paintCollision = make_unique<PaintCollision>();
	mouseRay = MouseRay::GetInstance();

	//D3D11関連の初期化
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PaintSkinMesh.hlsl", "VSSkin", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(device->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf())))
		return FALSE;
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(vertexLayout.Get());

	//ブロブからピクセルシェーダー作成
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PaintSkinMesh.hlsl", "PSSkin", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;


	D3D11_BUFFER_DESC cb;
	//コンスタントバッファー0作成
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(LightAndEyeGlobal);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, nullptr, lightAndEyeBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}
	//コンスタントバッファー1作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SkinMeshGlobal);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, nullptr, skinMeshBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}
	//コンスタントバッファーボーン用　作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(BoneGlobal);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, nullptr, boneBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	device->CreateSamplerState(&SamDesc, sampleLinear.ReleaseAndGetAddressOf());

	return S_OK;
}


void PaintSkinMesh::UpDate()
{
	//UpDateTriangles();

	matrixObject->WorldMatrixCreate();
	paintCollision->SetTriangles(triangles);
	paintCollision->SetWorldMatrix(matrixObject->GetWorldMatrix());

	Segment* segment;
	D3DXVECTOR2 uv = D3DXVECTOR2(0, 0);
	segment = mouseRay->RayCreate();

	D3DXVECTOR4 color[7]{
		Colors::Blue,
		Colors::Red,
		Colors::Green,
		Colors::Yellow,
		Colors::Purple,
		Colors::Aqua,
		Colors::Orange
	};

	if (segment)
	{
		if (paintCollision->IntersectSegment(segment, uv))
		{
			static int a = 0;
			a++;

			if (a % 7)
			{
				campus->CreateInk(color[a % 7], uv, 1);
			}
			delete segment;
		}
	}

}


/// <summary>
/// 描画
/// </summary>
void PaintSkinMesh::Render()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	D3D11_MAPPED_SUBRESOURCE pData;
	FollowCamera* camera = FollowCamera::GetInstance();
	D3DXVECTOR3 eye = Math::VectorToD3DXVECTOR3(camera->GetEyePos());
	D3DXMATRIX view = Math::MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX proj = Math::MatrixToD3DXMATRIX(camera->GetProjection());

	//使用するシェーダーのセット
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);



	//アニメーションフレームを進める　スキンを更新
	static int iFrame = 0;
	iFrame += 1000;
	if (iFrame >= 3600)
	{
		iFrame = 0;
	}
	SetNewPoseMatrices(iFrame);
	if (SUCCEEDED(deviceContext->Map(boneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		BoneGlobal sg;
		for (int i = 0; i<numBone; i++)
		{
			D3DXMATRIX mat = GetCurrentPoseMatrix(i);
			D3DXMatrixTranspose(&mat, &mat);
			sg.boneMatrix[i] = mat;
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(BoneGlobal));
		deviceContext->Unmap(boneBuffer.Get(), 0);
	}
	deviceContext->VSSetConstantBuffers(2, 1, boneBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(2, 1, boneBuffer.GetAddressOf());

	//バーテックスバッファーをセット（バーテックスバッファーは一つでいい）
	UINT stride = sizeof(SkinVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(vertexLayout.Get());

	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//カメラ位置をシェーダーに渡す
	if (SUCCEEDED(deviceContext->Map(lightAndEyeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		LightAndEyeGlobal sg;
		sg.vLightDir = D3DXVECTOR4(1, 1, -1, 0.0f);
		sg.vEye = D3DXVECTOR4(eye.x, eye.y, eye.z, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(LightAndEyeGlobal));
		deviceContext->Unmap(lightAndEyeBuffer.Get(), 0);
	}
	deviceContext->VSSetConstantBuffers(0, 1, lightAndEyeBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, lightAndEyeBuffer.GetAddressOf());

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i<numMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (material[i].numFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		deviceContext->IASetIndexBuffer(pIndexBuffer[i].Get(), DXGI_FORMAT_R32_UINT, 0);

		//マテリアルの各要素と変換行列をシェーダーに渡す			
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(deviceContext->Map(skinMeshBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SkinMeshGlobal sg;
			sg.mW = matrixObject->GetWorldMatrix();
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			sg.mWVP = matrixObject->GetWorldMatrix()*view*proj;
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			sg.vAmbient = material[i].Ka;
			sg.vDiffuse = material[i].Kd;
			sg.vSpecular = material[i].Ks;
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinMeshGlobal));
			deviceContext->Unmap(skinMeshBuffer.Get(), 0);
		}
		deviceContext->VSSetConstantBuffers(1, 1, skinMeshBuffer.GetAddressOf());
		deviceContext->PSSetConstantBuffers(1, 1, skinMeshBuffer.GetAddressOf());
		//テクスチャーをシェーダーに渡す
		if (material[i].textureName[0] != NULL)
		{
			deviceContext->PSSetSamplers(0, 1, sampleLinear.GetAddressOf());
			deviceContext->PSSetShaderResources(0, 1, material[i].texture.GetAddressOf());
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			deviceContext->PSSetShaderResources(0, 1, Nothing);
		}
		deviceContext->PSSetShaderResources(1, 1, campus->GetInkTexSRV());

		//Draw
		deviceContext->DrawIndexed(material[i].numFace * 3, 0, 0);
	}
	//アニメ進行
	if (d3dxMesh->m_pAnimController)
	{
		d3dxMesh->m_pAnimController->AdvanceTime(0.001666, nullptr);
	}
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	d3dxMesh->UpdateFrameMatrices(d3dxMesh->m_pFrameRoot, &m);
}