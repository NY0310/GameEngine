#include "PaintObj.h"
#include "../InkSegmentCollider/InkSegmentCollider.h"

using namespace std;
using namespace NYLibrary;


PaintObj::PaintObj(bool isPlane)
	:isPlane(isPlane)
{

}




void PaintObj::CreateAddChild()
{
	campus = make_shared<Paint>();
	AddChild(campus);

}

void PaintObj::Initialize()
{
	Obj::Initialize();
}

void PaintObj::Update()
{
	//matrixObject->Calc();
	//paintCollision->SetTriangles(triangles);
	//paintCollision->SetWorldMatrix(matrixObject->GetWorldMatrix());

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



	D3DXMATRIX View = camera->GetView();
	D3DXMATRIX Proj = camera->GetProjection();



	D3DXVECTOR3 vEyePt = camera->GetEyePos();

	////ワールドトランスフォーム
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//ID3D11VertexShader* VSShader;
	//deviceContext->VSGetShader(&VSShader,nullptr,nullptr);
	//if (VSShader != vertexShader.Get())
	//{
		//使用するシェーダーの登録	
		deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//}
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		//world = matrixObject->GetWorldMatrix();
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = GetWVP();
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


void PaintObj::OnCollisiton(Collider * collider)
{
	InkSegmentCollider* ink = dynamic_cast<InkSegmentCollider*>(collider);
	TrianglePolygonListCollider* triangleList = GetComponent<TrianglePolygonListCollider>();
	if (ink && triangleList)
	{
		campus->CreateInk(ink->GetColor(), CalcInkCollisionUv(*ink, triangleList->GetCollisionTriangle(), triangleList->GetInter()), 0.5f);
	}

	PlaneCollider* planeCollider = GetComponent<PlaneCollider>();
	if (ink && planeCollider)
	{
		campus->CreateInk(ink->GetColor(), D3DXVECTOR2(planeCollider->GetInter().x, planeCollider->GetInter().y), 0.5f);
	}
}



D3DXVECTOR2 PaintObj::CalcInkCollisionUv(const Segment& segment, const Triangle& triangle, const D3DXVECTOR3 & inter)
{
//	float distance = 1.0e5;
	//D3DXVECTOR3 l_inter;

	// 線分の始点と衝突点の距離を計算（めりこみ距離）
	/*D3DXVECTOR3 vec = segment.start - inter;
	float temp_distance = D3DXVec3Length(&vec);*/
	// めりこみ具合がここまでで最小なら
	
		// 衝突点の座標、めりこみ距離を記録
	//	l_inter = inter;
	//	distance = temp_distance;


		D3DXVECTOR3 p1 = triangle.p0;
		D3DXVECTOR3 p2 = triangle.p1;
		D3DXVECTOR3 p3 = triangle.p2;
		D3DXVECTOR3 p = inter;

		D3DXVECTOR2 uv1 = triangle.uv0;
		D3DXVECTOR2 uv2 = triangle.uv1;
		D3DXVECTOR2 uv3 = triangle.uv2;

		//塗られるオブジェクトのワールド座標をかける
		//PerspectiveCollect(透視投影を考慮したUV補間)
		D3DXMATRIX mvp = GetWVP();
		//各点をProjectionSpaceへの変換
		D3DXVECTOR4 p1_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p1.x, p1.y, p1.z, 0));
		D3DXVECTOR4 p2_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p2.x, p2.y, p1.z, 0));
		D3DXVECTOR4 p3_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p3.x, p3.y, p1.z, 0));
		D3DXVECTOR4 p_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p.x, p.y, p.z, 0));


		//通常座標への変換(ProjectionSpace)
		D3DXVECTOR2 p1_n = D3DXVECTOR2(p1_p.x, p1_p.y) / p1_p.w;
		D3DXVECTOR2 p2_n = D3DXVECTOR2(p2_p.x, p2_p.y) / p2_p.w;
		D3DXVECTOR2 p3_n = D3DXVECTOR2(p3_p.x, p3_p.y) / p3_p.w;
		D3DXVECTOR2 p_n = D3DXVECTOR2(p_p.x, p_p.y) / p_p.w;


		//頂点のなす三角形を点pにより3分割し、必要になる面積を計算
		float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
		float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
		float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
		//面積比からuvを補間
		float u = s1 / s;
		float v = s2 / s;
		float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
		D3DXVECTOR2 uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);
		return uv;
}





