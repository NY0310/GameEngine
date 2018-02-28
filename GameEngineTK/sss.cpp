#include "sss.h"

using namespace DirectX::SimpleMath;

sss::sss()
{
	modelAngele = 0.0f;
	angle = 0.0f;
	m_vLightPos = D3DXVECTOR3(0.0f, -5.0f, 0.0f);

}


sss::~sss()
{
}

HRESULT sss::InitD3D()
{
	//�[�x�e�N�X�`�� �i�����_�[�^�[�Q�b�g�j
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	tdesc.Width = DEPTHTEX_WIDTH;
	tdesc.Height = DEPTHTEX_HEIGHT;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.MiscFlags = 0;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	pDevice->CreateTexture2D(&tdesc, nullptr, &m_pDepthTexture);

	//�[�x�e�N�X�`���p�@�����_�[�^�[�Q�b�g�r���[�쐬�i�����_�����O���p�j
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	if (FAILED(pDevice->CreateRenderTargetView(m_pDepthTexture, &DescRT, &m_pDepthRTV)))
	{
		MessageBox(0, L"RTV fail", L"", MB_OK);
		return E_FAIL;
	}
	//�[�x�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[�쐬�i�e�N�X�`���[�m�F���p�j	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = tdesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(pDevice->CreateShaderResourceView(m_pDepthTexture, &SRVDesc, &m_pDepthTextureView)))
	{
		MessageBox(0, L"SRV fail", L"", MB_OK);
		return E_FAIL;
	}
	//�[�x�e�N�X�`���p�@�[�x�X�e���V���r���[�e�N�X�`���[�쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = DEPTHTEX_WIDTH;
	descDepth.Height = DEPTHTEX_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthDepthStencil);

	//����DSV�쐬
	pDevice->CreateDepthStencilView(m_pDepthDepthStencil, NULL, &m_pDepthDSV);

	//�V�F�[�_�[������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬
	m_pMesh = new MY_MESH;
	m_pMesh->dwNumFace = 0;
	if (FAILED(InitStaticMesh("dog.obj", m_pMesh)))
	{
		return E_FAIL;
	}

	return S_OK;
}



HRESULT sss::InitShader()
{
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//SSS�V�F�[�_�[ ������
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader[0],&pCompiledShader)))return E_FAIL;
	//���_�C���v�b�g���C�A�E�g��`
	D3D11_INPUT_ELEMENT_DESC layout[]=
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g�쐬
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout[0])))return FALSE;
	//�s�N�Z���V�F�[�_�쐬
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "PS", "ps_5_0",(void**)&m_pPixelShader[0], &pCompiledShader)))return E_FAIL;


	//�[�x�e�N�X�`���쐬
	//�o�[�e�b�N�X�V�F�[�_�[
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "VS_Depth", "vs_5_0", (void**)&m_pVertexShader[1], &pCompiledShader)))return E_FAIL;
	//�s�N�Z���V�F�[�_�[
	if (FAILED(shadermanager.MakeShader("sss.hlsl", "PS_Depth", "ps_5_0", (void**)&m_pPixelShader[1], &pCompiledShader)))return E_FAIL;
	//�[�x�e�N�X�`���p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pSamLinear);
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_SSS);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBuffer[0])))return E_FAIL;

	return S_OK;
}


void sss::ZTexRender(std::unique_ptr<FollowCamera>& camera)
{


	////////�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);
	pDeviceContext->OMSetRenderTargets(1, &m_pDepthRTV, m_pDepthDSV);

	float ClearColor[4] = { 0,0,1,1 };
	pDeviceContext->ClearRenderTargetView(m_pDepthRTV, ClearColor);
	pDeviceContext->ClearDepthStencilView(m_pDepthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	m_mView = camera->GetView();
	m_mProj = camera->GetProjection();


	//���f�����[���h�s�� 
	D3DXMATRIX mR;
	D3DXMatrixIdentity(&mR);
	modelAngele += 0.01f;
	D3DXMatrixRotationY(&mR, modelAngele);
	//D3DXMatrixTranslation(&mR, 0.0f, 2.0f, 0.0f);

	m_pMesh->mWorld = mR;
	// �r���[�g�����X�t�H�[�� �����ł̓��C�g����̎��E
	//���C�g����]������
	D3DXMATRIX mLight;
	D3DXVECTOR3 vLight = m_vLightPos;
	D3DXMatrixRotationY(&mR, angle);
	//angle += 0.01f;
	D3DXVec3TransformCoord(&vLight, &vLight, &mR);


	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_SSS cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�E���C�g�r���[�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
		D3DXMATRIX s;//�d�v�I�@�ۂߌ덷�ɂ��e�N�Z���̒[�ł̃A�[�e�B�t�@�N�g�h�~
		D3DXMatrixScaling(&s, 3.1, 3.1, 3.1);
		cb.mWLP = s*m_pMesh->mWorld*mLight*m_mProj;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer[0], 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexLayout[0]);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SIMPLE_VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�g�p����V�F�[�_�[�̓o�^	
	pDeviceContext->VSSetShader(m_pVertexShader[1], NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader[1], NULL, 0);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);

}

void sss::Render(std::unique_ptr<FollowCamera>& camera)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_SSS cb;

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s����V�F�[�_�[�ɓn��
		cb.mW = m_pMesh->mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		//���[���h�E�r���[�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
		cb.mWVP = m_pMesh->mWorld*m_mView*m_mProj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
		cb.mWLP = m_pMesh->mWorld*mLight*m_mProj;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);
		//�ˉe��Ԃ���e�N�X�`�����W��Ԃ֕ϊ�����s���n��
		D3DXMATRIX m;
		ZeroMemory(&m, sizeof(D3DXMATRIX));
		m._11 = 0.5;
		m._22 = -0.5;
		m._33 = 1;
		m._41 = 0.5;
		m._42 = 0.5;
		m._44 = 1;
		cb.mWLPT = m_pMesh->mWorld*mLight*m_mProj*m;
		D3DXMatrixTranspose(&cb.mWLPT, &cb.mWLPT);
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3DXVECTOR4 mat(0, 0, 0, 0);
		mat = D3DXVECTOR4(0.0, 0.0, 0.0, 1);
		cb.Ambient = mat;
		mat = D3DXVECTOR4(0.0, 1.0, 0.0, 1);
		cb.Diffuse = mat;
		mat = D3DXVECTOR4(0.5, 0.5, 0.5, 1);
		cb.Specular = mat;
		//���C�g�̈ʒu���V�F�[�_�[�ɓn��
		cb.vLightPos = vLight;//vLight�͏�ŁA��]���������Ƃ̃��C�g���݈ʒu

							  //�����x�N�g�����G�t�F�N�g�i�V�F�[�_�[�j�ɒʒm�E�K�p
		cb.vEye = camera->GetEyePos();
		//�ǂ��܂œ��߂��邩�̋���
		cb.g_Transparent = 2.4;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer[0], 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	//�P�p�X�ڂō�����[�x�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &m_pSamLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pDepthTextureView);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SIMPLE_VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexLayout[0]);
	//�g�p����V�F�[�_�[�̓o�^	
	pDeviceContext->VSSetShader(m_pVertexShader[0], NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader[0], NULL, 0);
	//�v���~�e�B�u�������_�����O	
	pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);
	

}



HRESULT sss::InitStaticMesh(LPSTR FileName, MY_MESH* pMesh)
{
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	char key[190] = { 0 };
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//���_
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	SIMPLE_VERTEX* pvVertexBuffer = new SIMPLE_VERTEX[pMesh->dwNumVert];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[pMesh->dwNumVert];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

													  //�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		//���_ �ǂݍ���
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = x;
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;
			dwVCount++;
		}

		//�@�� �ǂݍ���
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = x;
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}

		//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			dwFCount++;
			//���_�\���̂ɑ��
			pvVertexBuffer[v1 - 1].vPos = pvCoord[v1 - 1];
			pvVertexBuffer[v1 - 1].vNorm = pvNormal[vn1 - 1];
			pvVertexBuffer[v2 - 1].vPos = pvCoord[v2 - 1];
			pvVertexBuffer[v2 - 1].vNorm = pvNormal[vn2 - 1];
			pvVertexBuffer[v3 - 1].vPos = pvCoord[v3 - 1];
			pvVertexBuffer[v3 - 1].vNorm = pvNormal[vn3 - 1];
		}
	}

	fclose(fp);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SIMPLE_VERTEX) *pMesh->dwNumVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//�C���f�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete pvCoord;
	delete pvNormal;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}