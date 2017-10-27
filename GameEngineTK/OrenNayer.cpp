#include "pch.h"
#include "OrenNayer.h"


OrenNayer::OrenNayer()
{
	m_fScale = 1.0f;
}


OrenNayer::~OrenNayer()
{

}

HRESULT OrenNayer::Init(LPSTR FileName)
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

HRESULT OrenNayer::InitDx9()
{
	auto& device = Devices::Get();


	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);

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

HRESULT OrenNayer::LoadXMesh(LPSTR FileName)
{ 
	auto& device = Devices::Get();
	LPD3DXBUFFER D3DXMtrlBuffer;
	D3DXLoadMeshFromXA(FileName, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		m_pDevice9, NULL, &D3DXMtrlBuffer, NULL,
		&m_dwNumMaterial, &m_pMesh);

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;


	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)D3DXMtrlBuffer->GetBufferPointer();
	m_pMaterial = new MY_MATERIAL[m_dwNumMaterial];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];


	for (DWORD i = 0; i < m_dwNumMaterial; i++)
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
			strcpy(m_pMaterial[i].szTextureName, d3dxMaterials[i].pTextureFilename);
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device.Device.Get(),
				m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))
			{
				return E_FAIL;
			}
		}

	}
}

HRESULT OrenNayer::InitShader()
{
	return E_NOTIMPL;
}

void OrenNayer::Render(D3DXMATRIX & mView, D3DXMATRIX & mProj, D3DXVECTOR3 & vLight, D3DXVECTOR3 & vEye)
{
}
