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

	////���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//ID3D11VertexShader* VSShader;
	//deviceContext->VSGetShader(&VSShader,nullptr,nullptr);
	//if (VSShader != vertexShader.Get())
	//{
		//�g�p����V�F�[�_�[�̓o�^	
		deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//}
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		//world = matrixObject->GetWorldMatrix();
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = GetWVP();
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//���[���h�A���C�g�r���[�A�ˉe�s���n��
		cb.mWLP = world * mLight * Proj;

		//���[���h�A���C�g�r���[�A�ˉe�s��A�e�N�X�`���s��
		cb.mWLPT = world * mLight * Proj * m_mClipToUV;

		//���C�g�̕�����n��
		D3DXVECTOR3 vLightDir(-1, 0, -1);
		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);

		//���_�ʒu��n��
		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	deviceContext->PSSetShaderResources(1, 1, campus->GetInkTexSRV());//�S�C���N�������_�����O�����e�N�X�`��
	deviceContext->PSSetShaderResources(2, 1, depthMapTexSRV.GetAddressOf());//���C�g�r���[�ł̐[�x�e�N�X�`���쐬
															   //���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(vertexLayout.Get());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mesh.pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	deviceContext->IASetIndexBuffer(mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�������_�����O
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

	// �����̎n�_�ƏՓ˓_�̋������v�Z�i�߂肱�݋����j
	/*D3DXVECTOR3 vec = segment.start - inter;
	float temp_distance = D3DXVec3Length(&vec);*/
	// �߂肱�݋�������܂łōŏ��Ȃ�
	
		// �Փ˓_�̍��W�A�߂肱�݋������L�^
	//	l_inter = inter;
	//	distance = temp_distance;


		D3DXVECTOR3 p1 = triangle.p0;
		D3DXVECTOR3 p2 = triangle.p1;
		D3DXVECTOR3 p3 = triangle.p2;
		D3DXVECTOR3 p = inter;

		D3DXVECTOR2 uv1 = triangle.uv0;
		D3DXVECTOR2 uv2 = triangle.uv1;
		D3DXVECTOR2 uv3 = triangle.uv2;

		//�h����I�u�W�F�N�g�̃��[���h���W��������
		//PerspectiveCollect(�������e���l������UV���)
		D3DXMATRIX mvp = GetWVP();
		//�e�_��ProjectionSpace�ւ̕ϊ�
		D3DXVECTOR4 p1_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p1.x, p1.y, p1.z, 0));
		D3DXVECTOR4 p2_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p2.x, p2.y, p1.z, 0));
		D3DXVECTOR4 p3_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p3.x, p3.y, p1.z, 0));
		D3DXVECTOR4 p_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p.x, p.y, p.z, 0));


		//�ʏ���W�ւ̕ϊ�(ProjectionSpace)
		D3DXVECTOR2 p1_n = D3DXVECTOR2(p1_p.x, p1_p.y) / p1_p.w;
		D3DXVECTOR2 p2_n = D3DXVECTOR2(p2_p.x, p2_p.y) / p2_p.w;
		D3DXVECTOR2 p3_n = D3DXVECTOR2(p3_p.x, p3_p.y) / p3_p.w;
		D3DXVECTOR2 p_n = D3DXVECTOR2(p_p.x, p_p.y) / p_p.w;


		//���_�̂Ȃ��O�p�`��_p�ɂ��3�������A�K�v�ɂȂ�ʐς��v�Z
		float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
		float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
		float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
		//�ʐϔ䂩��uv����
		float u = s1 / s;
		float v = s2 / s;
		float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
		D3DXVECTOR2 uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);
		return uv;
}





