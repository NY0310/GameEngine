#include "PaintObj.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


PaintObj::PaintObj(bool isPlane)
{
	campus = make_unique<Paint>();
	campus->InitD3D(isPlane);
	paintCollision = make_unique<PaintCollision>();
	matrixObject->SetPosition(D3DXVECTOR3(0, 1.5, 0));
	matrixObject->SetScale(D3DXVECTOR3(7, 7, 7));
}


void PaintObj::Update()
{
	matrixObject->Update();
	paintCollision->SetTriangles(triangles);
	paintCollision->SetWorldMatrix(matrixObject->GetWorldMatrix());

	//Segment* segment;
	//D3DXVECTOR2 uv = D3DXVECTOR2(0,0);


	//if (segment)
	//{
	//	if (paintCollision->IntersectSegment(segment, uv))
	//	{
	//		static int a = 0;
	//		a++;

	//		if (a % 7)
	//		{
	//			campus->CreateInk(color[a % 7], uv, 1);
	//		}
	//		delete segment;
	//	}
	//}

}

void PaintObj::Render()
{


	D3DXMATRIX View = Math::MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = Math::MatrixToD3DXMATRIX(camera->GetProjection());



	D3DXVECTOR3 vEyePt = Math::VectorToD3DXVECTOR3(camera->GetEyePos());

	////ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//使用するシェーダーの登録	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		world = matrixObject->GetWorldMatrix();
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = world  *View * Proj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
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
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}
	//テクスチャーをシェーダーに渡す
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	deviceContext->PSSetShaderResources(1, 1, campus->GetInkTexSRV());//全インクをレンダリングしたテクスチャ
	deviceContext->PSSetShaderResources(2, 1, depthMapTexSRV.GetAddressOf());//ライトビューでの深度テクスチャ作成
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
	//プリミティブをレンダリング
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);
}





