#include "D3DXREAD.h"



D3DXREAD::D3DXREAD()
{
	ZeroMemory(this, sizeof(D3DXREAD));
}


D3DXREAD::~D3DXREAD()
{
}

/// <summary>
/// D3DX�̃p�[�T�[��Dx9�̃f�o�C�X���g������쐬����
/// </summary>
/// <returns>���������s(����)</returns>
HRESULT D3DXREAD::InitDx9()
{
	auto& device = Devices::Get();
	//�uDirect3D�v�I�u�W�F�N�g�̍쐬
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
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

	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &pDevice)))
	{
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &pDevice)))
		{
			MessageBoxA(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
			if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &pDevice)))
			{
				if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &pDevice)))
				{
					MessageBoxA(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}


/// <summary>
/// X�t�@�C�����烁�b�V�������[�h����
/// </summary>
/// <param name="FileName">�t�@�C����</param>
/// <returns>���������s(����)</returns>
HRESULT D3DXREAD::LoadXMesh(LPSTR FileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromXA(FileName, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBoxA(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i<dwNumMaterials; i++)
	{
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;
		pMeshTextures[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			strcpy(m_TextureFileName[i], d3dxMaterials[i].pTextureFilename);
		}
	}
	pD3DXMtrlBuffer->Release();

	return S_OK;
}
