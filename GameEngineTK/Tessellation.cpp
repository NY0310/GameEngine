#include "Tessellation.h"


Tessellation::Tessellation()
{
}


Tessellation::~Tessellation()
{
}

HRESULT Tessellation::InitD3D()
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

//�V�F�[�_�[���쐬�@���_���C�A�E�g���`
HRESULT Tessellation::InitShader()
{
	ID3DBlob *pCompiledShader = nullptr;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader)))return E_FAIL;
	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬 
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))return FALSE;
	//�h���C���V�F�[�_�[
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "DS", "ds_5_0", (void**)&m_pDomainShader, &pCompiledShader)))return E_FAIL;
	//�n���V�F�[�_�[�쐬 
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "HS", "hs_5_0", (void**)&m_pHullShader, &pCompiledShader)))return E_FAIL;
	//�s�N�Z���V�F�[�_�쐬
	if (FAILED(shadermanager.MakeShader("Tessellation.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader)))return E_FAIL;
	if (pCompiledShader)
	{
		pCompiledShader->Release();
		pCompiledShader = nullptr;
	}
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBuffer)))return E_FAIL;




	return S_OK;
}

//�o�[�e�b�N�X�o�b�t�@�[�쐬
HRESULT Tessellation::InitPolygon()
{
	//�R���g���[���|�C���g���`
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-1, 1,0),
		D3DXVECTOR3(1, 1,0),
		D3DXVECTOR3(-1, -1, 0),
	};
	//��̃R���g���[���|�C���g�Ńo�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}


void Tessellation::Render(std::unique_ptr<FollowCamera>& camera)
{
	D3DXMATRIX mWorld;
	D3DXMATRIX mView = camera->GetView();
	D3DXMATRIX mProj = camera->GetProjection();
	//D3DXMatrixIdentity(&mWorld);
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	D3DXMatrixRotationY(&mWorld, timeGetTime() / 1000.0f);//�P����yaw��]������

	//�V�F�[�_�[�̓o�^
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(m_pHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(m_pDomainShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = mWorld*mView*mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���A�ǂ̃V�F�[�_�[�Ŏg�������w��
	pDeviceContext->DSSetConstantBuffers(0, 1, &m_pConstantBuffer);//�h���C���V�F�[�_�[�Ŏg��

																	 //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(3, 0);

}