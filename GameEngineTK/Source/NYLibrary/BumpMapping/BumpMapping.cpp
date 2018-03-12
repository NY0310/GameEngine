#include "BumpMapping.h"


//
//D3DXVECTOR4 CalcTangent
//���_3��UV���W�R�Ɩ@�������Ƃɐڐ����v�Z����B�Ԃ�4�����x�N�g����w�͍����E��n���̔��f�p�t���O�B
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
	//�V�F�[�_�[������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}
	return S_OK;

}


HRESULT BumpMapping::InitShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("BumpMapping.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader)))return E_FAIL;
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))return FALSE;
	//�u���u����s�N�Z���V�F�[�_�쐬
	if (FAILED(shadermanager.MakeShader("BumpMapping.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader)))return E_FAIL;
	//�R���X�^���g�o�b�t�@�[�쐬
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
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[] =
	{
		// ���W�A�@���AUV�A�^���W�F���g�̏�
		D3DXVECTOR3(-0.5,0,-0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,1),D3DXVECTOR4(0,0,0,0),//���_1, �^���W�F���g�͍��͂��ׂă[���i���ƂŌv�Z�j
		D3DXVECTOR3(-0.5,0,0.5), D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),D3DXVECTOR4(0,0,0,0),//���_2
		D3DXVECTOR3(0.5,0,-0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1),D3DXVECTOR4(0,0,0,0), //���_3
		D3DXVECTOR3(-0.5,0,0.5),D3DXVECTOR3(0,0,-1), D3DXVECTOR2(0,0),D3DXVECTOR4(0,0,0,0), //���_4
		D3DXVECTOR3(0.5,0,0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,0),D3DXVECTOR4(0,0,0,0),//���_5
		D3DXVECTOR3(0.5,0,-0.5),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1),D3DXVECTOR4(0,0,0,0), //���_6
	};
	//�ڐ����v�Z
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

		//�ڐ��v�Z�̓g���C�A���O���P�ʁ@�P�ڂ̃g���C�A���O��
		vTangent = CalcTangent(v1, v2, v3, t1, t2, t3, vN);
		vertices[0].vTangent = vTangent;
		vertices[1].vTangent = vTangent;
		vertices[2].vTangent = vTangent;
		//�ڐ��v�Z�̓g���C�A���O���P�ʁ@�Q�ڂ̃g���C�A���O��
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
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	//�m�[�}���}�b�v�̃e�N�X�`���[�쐬
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

	//�m�[�}���}�b�v�̃e�N�X�`���[�̃V�F�[�_�[���\�[�X�r���[�쐬
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
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	//D3DXMatrixRotationY(&mWorld, timeGetTime() / 1000.0f);//�P����yaw��]������

	//�g�p����V�F�[�_�̓o�^
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE SubRes;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubRes)))
	{
		//���[���h�s���n��
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		//���[���h�̋t�s��̓]�u�s����V�F�[�_�[�ɓn��
		cb.mWIT = mWorld;
		D3DXMatrixInverse(&cb.mWIT, NULL, &cb.mWIT);
		D3DXMatrixTranspose(&cb.mWIT, &cb.mWIT);
		//���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = mWorld*mView*mProj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//���C�g������n��
		cb.vLightDir = (D3DXVECTOR4)m_vLight;
		//Ambient��n��
		cb.Ambient = D3DXVECTOR4(0.0, 0.0, 0.4, 1);
		//Diffuse��n��
		cb.Diffuse = D3DXVECTOR4(0.0, 0.0, 0.7, 1);
		//Specular��n��
		cb.Specular = D3DXVECTOR4(1.5, 1.5, 1.5, 1);
		//�J�����̈ʒu(���_)���V�F�[�_�[�ɓn��
		D3DXVECTOR3 eyepos = (camera->GetEyePos());
		cb.Specular = D3DXVECTOR4(eyepos.x, eyepos.y, eyepos.z, 0);

		//�g�@�֘A
		static D3DXVECTOR4 WaveMove(0, 0, 0, 0);
		WaveMove.x += 0.0001f;
		WaveMove.y += 0.0005f;
		//�g�̈ʒu�ω��ʂ�n��
		cb.WaveMove = WaveMove;


		memcpy_s(SubRes.pData, SubRes.RowPitch, (void*)&cb, sizeof(SHADER_CONSTANT_BUFFER));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);

	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pNormalTextureSRV);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(6, 0);



}

