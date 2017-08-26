#include "CD3DXMESH.h"

using namespace std;

/// <summary>
/// �R���X�g���N�^
/// </summary>
CD3DXMESH::CD3DXMESH()
{
	ZeroMemory(this, sizeof(CD3DXMESH));
	birthcnt = 0;
	m_fScale = 1.0f;
	m_Texture = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CD3DXMESH::~CD3DXMESH()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pD3d9);

}

HRESULT CD3DXMESH::Init(LPSTR FileName)
{

	if (FAILED(InitDx9()))
	{
		return E_FAIL;
	}
	if (FAILED(LoadXMesh(FileName)))
	{
		return E_FAIL;
	}

	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	return S_OK;
}

/// <summary>
/// Dx9�̏���������
/// D3DX�̃p�[�T�[���g�����߂ɂ́ADx9�̃f�o�C�X���K�v�Ȃ̂ō쐬����B
/// </summary>
/// <returns>�����A���s����</returns>
HRESULT CD3DXMESH::InitDx9()
{

	auto& device = Devices::Get();


	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	if (NULL == (m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBoxA(0, "Direct3D9�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}

	//�uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;



	if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBoxA(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
			if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBoxA(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}


	return S_OK;
}

HRESULT CD3DXMESH::LoadXMesh(LPSTR FileName)
{
	auto& devices = Devices::Get();


	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromXA(FileName, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		m_pDevice9, NULL, &pD3DXMtrlBuffer, NULL,
		&m_dwNumMaterial, &m_pMesh)))
	{
		MessageBoxA(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return E_FAIL;
	}

	//���̎��_�ŁA�t�@�C��������o�������b�V���f�[�^���ADx9��D3DX���b�V���ɓ����Ă���A
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//���Ƃ́A��������D���ȏ������o����Dx11�̎��O���b�V���ɗ��p���邾���B
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterial = new MY_MATERIAL[m_dwNumMaterial];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];

	for (DWORD i = 0; i<m_dwNumMaterial; i++)
	{
		//�A���r�G���g
		m_pMaterial[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterial[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterial[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterial[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//�f�B�t���[�Y
		m_pMaterial[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterial[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterial[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterial[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//�X�y�L�����[
		m_pMaterial[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterial[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterial[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterial[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;
		//�e�N�X�`���[�������
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			m_Texture = true;
			strcpy(m_pMaterial[i].szTextureName, d3dxMaterials[i].pTextureFilename);
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(),
				m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))
			{
				return E_FAIL;
			}
		}
		
	}

	//�C���f�b�N�X�o�b�t�@�[���쐬

	//���b�V���̑������𓾂�B�������ŃC���f�b�N�X�o�b�t�@�[����ׂ����}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�𕪗��ł���
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr)))
	{
		MessageBoxA(0, "�����e�[�u���擾���s", "", MB_OK);
		return E_FAIL;
	}
	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�C���f�b�N�X�o�b�t�@�[������o���Ȃ��̂Ń��b�N����B
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);

	//�������Ƃ̃C���f�b�N�X�o�b�t�@���쐬
	for (DWORD i = 0; i<m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;
		//Dx9��D3DMESH�̃C���f�b�N�X�o�b�t�@�[����̏��ŁADx11�̃C���f�b�N�X�o�b�t�@���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pAttrTable[i].FaceCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];//�傫���C���f�b�N�X�o�b�t�@���̃I�t�Z�b�g(*3��Y�ꂸ�Ɂj

		if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			return FALSE;
		}
		m_pMaterial[m_AttrID[i]].dwNumFace = pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;
	m_pMesh->UnlockIndexBuffer();

	pD3DXMtrlBuffer->Release();

	//�o�[�e�b�N�X�o�b�t�@�[���쐬

	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�o�[�e�b�N�X�o�b�t�@�[������o���Ȃ��̂Ń��b�N����B
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MY_VERTEX_TEX* pvVertex = NULL;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pvVertex = (MY_VERTEX_TEX*)pVertices;
		//Dx9��D3DMESH�̃o�[�e�b�N�X�o�b�t�@�[����̏��ŁADx11�̃o�[�e�b�N�X�o�b�t�@���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pvVertex;

		//�e�N�X�`���[���W���}�C�i�X�΍�
		if (m_Texture)
		{
			for (int i = 0; i<m_pMesh->GetNumVertices(); i++)
			{
				if (pvVertex[i].vTex.x<0)
				{
					pvVertex[i].vTex.x += 1;
				}
				if (pvVertex[i].vTex.y<0)
				{
					pvVertex[i].vTex.y += 1;
				}
			}
		}

		if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
			return FALSE;

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return S_OK;
}




HRESULT CD3DXMESH::InitShader()
{
	auto& devices = Devices::Get();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (m_Texture)
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			if (pErrors != NULL)
			{
				string error_str((char*)pErrors->GetBufferPointer());
				OutputDebugStringA((char*)pErrors->GetBufferPointer());
			}
			if (pErrors) pErrors->Release();
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS_NoTeX", "vs_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return E_FAIL;
		}
	}
		
	SAFE_RELEASE(pErrors);

	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	

	//���_�C���v�b�g���C�A�E�g���`
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[3];
	if (m_Texture)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = 3;
		memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = 2;
		memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (m_Texture)
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS_NoTex", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬 �ϊ��s��n��
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer2)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CD3DXMESH::Render(unique_ptr<FollowCamera>& camera , D3DXVECTOR3& vLight)
{
	auto& devices = Devices::Get();


	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	D3DXMatrixScaling(&mScale, m_fScale, m_fScale, m_fScale);
	D3DXMatrixRotationY(&mYaw, m_fYaw);
	D3DXMatrixRotationX(&mPitch, m_fPitch);
	D3DXMatrixRotationZ(&mRoll, m_fRoll);
	D3DXMatrixTranslation(&mTran, m_vPos.x, m_vPos.y, m_vPos.z);

	mWorld = mScale*mYaw*mPitch*mRoll*mTran;
	D3DXMATRIX View;
	D3DXMATRIX Proj;




	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->m_eyepos);
	D3DXVECTOR3 vLookatPt = shadermanager.VectorToD3DXVECTOR3(camera->m_refpos);
	D3DXVECTOR3 vUpVec = shadermanager.VectorToD3DXVECTOR3(camera->m_upvec);

	D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovRH(&Proj, camera->m_fovY, camera->m_aspect, camera->m_NearClip, camera->m_FarClip);



	//�g�p����V�F�[�_�[�̓o�^
	devices.Context().Get()->VSSetShader(m_pVertexShader, NULL, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SIMPLECONSTANT_BUFFER0 sg;
		//���[���h�s���n��
		sg.mW = mWorld;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);
		//���[���h�A�J�����A�ˉe�s���n��
		sg.mWVP = mWorld*View*Proj;
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
		//���C�g�̕�����n��
		sg.vLightDir = D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);
		//���_�ʒu��n��
		sg.vEyes = D3DXVECTOR4(camera->GetEyepos().x, camera->GetEyepos().y, camera->GetEyepos().z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
	}
	m_pSampleLinear = NULL;
	////�e�N�X�`���[���V�F�[�_�[�ɓn��
	//devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
	//devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	//�����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@�|��`��
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		//�g�p����Ă��Ȃ��΍�
		if (m_pMaterial[m_AttrID[i]].dwNumFace == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		devices.Context().Get()->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient = m_pMaterial[m_AttrID[i]].Ambient;//�A���r�G���g�����V�F�[�_�[�ɓn��
			sg.vDiffuse = m_pMaterial[m_AttrID[i]].Diffuse;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			sg.vSpecular = m_pMaterial[m_AttrID[i]].Specular;//�X�y�L�����[���V�F�[�_�[�ɓn��
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER1));
			devices.Context().Get()->Unmap(m_pConstantBuffer2, 0);
		}
		devices.Context().Get()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer2);
		devices.Context().Get()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer2);
		
		m_pMaterial[m_AttrID[i]].pTexture = nullptr;
		if (m_pMaterial[m_AttrID[i]].pTexture)
		{
			devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
			devices.Context().Get()->PSSetShaderResources(0, 1, &m_pMaterial[m_AttrID[i]].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			devices.Context().Get()->PSSetShaderResources(0, 1, Nothing);
		}
		//�v���~�e�B�u�������_�����O
		devices.Context().Get()->DrawIndexed(m_pMaterial[m_AttrID[i]].dwNumFace * 3, 0, 0);

	}



}
