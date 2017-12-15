#include "DisplacementMapping.h"




DisplacementMapping::DisplacementMapping()
{
	//WaveMove = D3DXVECTOR4(0, 0, 0, 0);
	//WaveHeight = D3DXVECTOR4(0, 0, 0, 0);

}


DisplacementMapping::~DisplacementMapping()
{
}

HRESULT DisplacementMapping::InitD3D()
{
	//�V�F�[�_�[������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//���b�V���쐬
	if (FAILED(InitStaticMesh("Quad_Lo.obj", &m_Mesh)))
	{
		return E_FAIL;
	}
	//�n�C�g�}�b�v
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, "WaterBump.bmp", nullptr, nullptr, &m_pNormalTexture, nullptr)))return E_FAIL;
	////�[�x�}�b�v�e�N�X�`��
	//if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, "china_DensityMap.tif", nullptr, nullptr, &m_pDensityTexture, nullptr)))return E_FAIL;
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, "SnowHeight.bmp", nullptr, nullptr, &m_pSnowHeightTexture, nullptr)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamDesc.MaxAnisotropy = 16;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pDevice->CreateSamplerState(&SamDesc, &m_pSamPoint);

	return S_OK;
}

HRESULT DisplacementMapping::InitShader()
{
	ID3DBlob *pCompiledShader = nullptr;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader)))return E_FAIL;
	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(layout,numElements,pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),&m_pVertexLayout)))return E_FAIL;
	//�n���V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "HS", "hs_5_0", (void**)&m_pHullShader, &pCompiledShader)))return E_FAIL;
	//�h���C���V�F�[�_�[�쐬 
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "DS", "ds_5_0", (void**)&m_pDomainShader, &pCompiledShader)))return E_FAIL;
	//�s�N�Z���V�F�[�_ 
	if (FAILED(shadermanager.MakeShader("DisplacementMapping.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader)))return E_FAIL;
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
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

HRESULT DisplacementMapping::InitPolygon()
{
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-1, 0,1),D3DXVECTOR2(0,0),
		D3DXVECTOR3(1, 0,1),D3DXVECTOR2(0,1),
		D3DXVECTOR3(1, 0,-1),D3DXVECTOR2(1,1),
		D3DXVECTOR3(-1, 0, -1),D3DXVECTOR2(1,0),
	};

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))return E_FAIL;

	return S_OK;
}

HRESULT DisplacementMapping::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVTCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[
	pMesh->dwNumFace = 0;
	pMesh->dwNumVert = 0;
	char key[200] = { 0 };
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A���ǂݍ���
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(key, &m_Material);
		}
		//���_
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		//�@��
		if (strcmp(key, "vn") == 0)
		{
			dwVNCount++;
		}
		//�e�N�X�`���[���W
		if (strcmp(key, "vt") == 0)
		{
			dwVTCount++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	SimpleVertex* pvVertexBuffer = new SimpleVertex[pMesh->dwNumVert];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

													  //�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	dwVCount = 0;
	dwVNCount = 0;
	dwVTCount = 0;
	dwFCount = 0;

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

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[dwVTCount].x = 1 - x;//OBJ�t�@�C����X�������t�Ȃ̂ō��킹��
			pvTexture[dwVTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
			dwVTCount++;
		}

		//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			dwFCount++;
			//���_�\���̂ɑ��
			pvVertexBuffer[v1 - 1].vPos = pvCoord[v1 - 1];
			pvVertexBuffer[v1 - 1].vNorm = pvNormal[vn1 - 1];
			pvVertexBuffer[v1 - 1].vTex = pvTexture[vt1 - 1];
			pvVertexBuffer[v2 - 1].vPos = pvCoord[v2 - 1];
			pvVertexBuffer[v2 - 1].vNorm = pvNormal[vn2 - 1];
			pvVertexBuffer[v2 - 1].vTex = pvTexture[vt2 - 1];
			pvVertexBuffer[v3 - 1].vPos = pvCoord[v3 - 1];
			pvVertexBuffer[v3 - 1].vNorm = pvNormal[vn3 - 1];
			pvVertexBuffer[v3 - 1].vTex = pvTexture[vt3 - 1];
		}
	}

	fclose(fp);

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) *pMesh->dwNumVert;
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
	delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}

HRESULT DisplacementMapping::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMaterial)
{
	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	char key[110] = { 0 };
	D3DXVECTOR4 v(0, 0, 0, 0);
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(m_Material.szName, key);
		}
		//Ka�@�A���r�G���g
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ka = v;
		}
		//Kd�@�f�B�t���[�Y
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Kd = v;
		}
		//Ks�@�X�y�L�����[
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ks = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &m_Material.szTextureName, sizeof(m_Material.szTextureName));
			//�e�N�X�`���[�쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, m_Material.szTextureName, nullptr, nullptr, &m_pTexture, nullptr)))
			{
				return E_FAIL;
			}
		}
	}
	fclose(fp);

	return S_OK;
}

void DisplacementMapping::Render(std::unique_ptr<FollowCamera>& camera)
{

	/*WaveMove.x += 0.0004f;
	WaveMove.y += 0.0002f;*/


	D3DXMATRIX mWorld;
	D3DXMATRIX mView = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX mProj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	D3DXMatrixIdentity(&mWorld);
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	//D3DXMatrixTranslation(&mWorld, 0, -10000, 0);
	
	D3DXMatrixRotationX(&mWorld, 2.9f);
//	D3DXMatrixScaling(&mWorld, 15, 15, 15);

	//���[���h�g�����X�t�H�[���͌X�ňقȂ�
	D3DXMATRIX Scale, Tran, Rot;
	D3DXMatrixScaling(&Scale, 7, 5, 5);
	D3DXMatrixTranslation(&Tran,0, -1.3, 0);
	mWorld *= Scale * Tran;
	//�V�F�[�_�[�̓o�^�@	
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(m_pHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(m_pDomainShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	//�V�F�[�_�[�ɃR���X�^���g�o�b�t�@�[��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX m = mWorld*mView*mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//���[���h�s���n��
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		//���f�����猩�����_�i�܂�A���f���̋t���[���h�����������_�j
		cb.vEyePos = shadermanager.VectorToD3DXVECTOR3(DirectX::SimpleMath::Vector3(camera->GetEyePos().x, camera->GetEyePos().y + 10, camera->GetEyePos().z));
		D3DXMATRIX Inv;
		D3DXMatrixInverse(&Inv, nullptr, &(mWorld*mView));
		//�w�肳�ꂽ�s��ɂ�� 3D �x�N�g����ϊ����A���̌��ʂ� w = 1 �Ɏˉe����B
		D3DXVec3TransformCoord(&cb.vEyePos, &cb.vEyePos, &Inv);
		//�ŏ������A�ő勗��
		cb.fMinDistance = 0.5f;
		cb.iMaxDevide = 4.5f;
		//�ő啪����
		cb.iMaxDevide = 64;
		//���C�g�̕���
		cb.LightDir = D3DXVECTOR4(1, 1, 1, 0);
		//��̐����W����n��
		static float Glow = 0;
		Glow -= 0.0004;
		if (Glow>1.0f) Glow = 0;
		cb.Glow.x = Glow;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//�e�N�X�`���[���h���C���V�F�[�_�[�ɓn��
	pDeviceContext->DSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->DSSetShaderResources(0, 1, &m_pNormalTexture);
	//�e�N�X�`���[���s�N�Z���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pNormalTexture);
	//�e�N�X�`���[���h���C���V�F�[�_�[�ɓn��
	pDeviceContext->DSSetSamplers(0, 1, &m_pSamPoint);
	pDeviceContext->DSSetShaderResources(1, 1, &m_pSnowHeightTexture);

	//�R���X�^���g�o�b�t�@�[���V�F�[�_�ɓn��
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->DSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->HSSetConstantBuffers(0, 1, &m_pConstantBuffer);//�n���V�F�[�_�[�Ŏg��
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);//�s�N�Z���V�F�[�_�[�Ŏg��

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(SimpleVertex);
	offset = 0;
	pDeviceContext->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);




}
