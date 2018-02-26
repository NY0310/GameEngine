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

	////���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//�g�p����V�F�[�_�[�̓o�^	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		world = matrixObject->GetWorldMatrix();
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = world  *View * Proj;
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





