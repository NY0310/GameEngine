#include "PaintObj.h"
#include "../InkSegmentCollider/InkSegmentCollider.h"

using namespace std;
using namespace NYLibrary;


PaintObj::PaintObj( LPSTR fileName, bool isPlane)
	 :Obj(fileName) ,isPlane(isPlane)
{
}




void PaintObj::CreateAddChild()
{
	campus = make_shared<Paint>(isPlane);
	AddChild(campus);

}

void PaintObj::Initialize()
{
	Obj::Initialize();
}



void PaintObj::Render()
{

	ShadowMap* shadowMap = ShadowMap::GetInstance();
	auto light = Light::GetInstance();


	D3DXMATRIX Proj = camera->GetProjection();




	//�g�p����V�F�[�_�[�̓o�^	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s���n��
		cb.w = GetWorldMatrix();
		D3DXMatrixTranspose(&cb.w, &cb.w);
		//���[���h�A�J�����A�ˉe�s���n��
		cb.wvp = GetWVP();
		D3DXMatrixTranspose(&cb.wvp, &cb.wvp);

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

		D3DXMATRIX a_light = light->GetLightView();

		//	�r���[�s��̐���
		D3DXVECTOR3 eyepos(31, 57, 7.03f);
		D3DXVECTOR3 refpos(28, 55, 7.06f);
		D3DXVECTOR3 upvec(0, 1, 0);
		D3DXMatrixLookAtLH(&a_light, &eyepos, &refpos, &upvec);

		//���[���h�A���C�g�r���[�A�ˉe�s���n��
		//cb.wlp = GetWorldMatrix() * light->GetLightView() * Proj;
		cb.wlp = GetWorldMatrix() *  light->GetLightView() * Proj;
		D3DXMatrixTranspose(&cb.wlp, &cb.wlp);
		//���[���h�A���C�g�r���[�A�ˉe�s��A�e�N�X�`���s��
		//cb.wlpt = GetWorldMatrix() * light->GetLightView() * Proj * clipToUV;
		cb.wlpt = GetWorldMatrix() *  light->GetLightView() * Proj * clipToUV;
		D3DXMatrixTranspose(&cb.wlpt, &cb.wlpt);
		//���C�g�̕�����n��
		cb.lightDir = D3DXVECTOR4(light->GetPosition().x, light->GetPosition().y, light->GetPosition().z, 1.0f);

		//���_�ʒu��n��
		cb.eyesPos = D3DXVECTOR4(camera->GetEyePos().x, camera->GetEyePos().y, camera->GetEyePos().z, 1.0f);
		//�t���[����n��
		cb.frame = Math::Lerp(0, MAX_BREAK_CNT, breakTime);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}
	//�T���v���[���V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());
	//���f���̃e�N�X�`�����V�F�[�_�[�ɓn��
	deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	//�S�C���N�������_�����O�����e�N�X�`��
	deviceContext->PSSetShaderResources(1, 1, campus->GetInkTexSRV());
	//���C�g�r���[�ł̐[�x�e�N�X�`����n��
	deviceContext->PSSetShaderResources(2, 1, shadowMap->GetShaderResourceView().GetAddressOf());
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
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

	deviceContext->GSSetShader(nullptr, nullptr, 0);

}


void PaintObj::OnCollisiton(Collider * collider)
{
	InkSegmentCollider* ink = dynamic_cast<InkSegmentCollider*>(collider);
	TrianglePolygonListCollider* triangleList = GetComponent<TrianglePolygonListCollider>();
	if (ink && triangleList)
	{
		auto a = GetLocalSize();
		campus->CreateInk(ink->GetColor(), CalcInkCollisionUv(triangleList->GetCollisionTriangle(), triangleList->GetInter()),0.01f);
	}
	PlaneCollider* plane = GetComponent<PlaneCollider>();
	if (ink && plane)
	{

		D3DXVECTOR2 uv = CalcInkCollisionUv(triangles[0], plane->GetInter());
		if (!uv)
		{
			uv = CalcInkCollisionUv(triangles[1], plane->GetInter());

		}
		campus->CreateInk(ink->GetColor(), uv , 10.0f / Math::ComparisonBig(Math::D3DXVec3Multiplication(GetLocalSize(), GetScale())) );
	}
}


D3DXVECTOR2 PaintObj::CalcInkCollisionUv(const Triangle& triangle, const D3DXVECTOR3 & inter)
{

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
		D3DXVECTOR4 p2_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p2.x, p2.y, p2.z, 0));
		D3DXVECTOR4 p3_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p3.x, p3.y, p3.z, 0));
		D3DXVECTOR4 p_p = Math::MatrixTimes(mvp, D3DXVECTOR4(p.x, p.y, p.z, 0));

		//�ʏ���W�ւ̕ϊ�(ProjectionSpace)
		D3DXVECTOR2 p1_n = D3DXVECTOR2(p1_p.x, p1_p.y) / p1_p.w;
		D3DXVECTOR2 p2_n = D3DXVECTOR2(p2_p.x, p2_p.y) / p2_p.w;
		D3DXVECTOR2 p3_n = D3DXVECTOR2(p3_p.x, p3_p.y) / p3_p.w;
		D3DXVECTOR2 p_n = D3DXVECTOR2(p_p.x, p_p.y) / p_p.w;


		//�񐔂��`�F�b�N
		Math::ValidateNan(p1_n);
		Math::ValidateNan(p2_n);
		Math::ValidateNan(p3_n);
		Math::ValidateNan(p_n);


		//���_�̂Ȃ��O�p�`��_p�ɂ��3�������A�K�v�ɂȂ�ʐς��v�Z
		float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
		float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
		float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
		//�ʐϔ���v�Z
		float tri1 = s1 / s;
		float tri2 = s2 / s;
		float tri3 = ((1 - tri1 - tri2));

		float w = 1 / (tri3  / p1_p.w + tri1 / p2_p.w + tri2  / p3_p.w);
		D3DXVECTOR2 uv =  tri3 * uv1 + tri1 * uv2 + tri2 * uv3;
		return uv;
}


