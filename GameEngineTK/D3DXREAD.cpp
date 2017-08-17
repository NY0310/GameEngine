#include "D3DXREAD.h"



D3DXREAD::D3DXREAD()
{
	ZeroMemory(this, sizeof(D3DXREAD));
}


D3DXREAD::~D3DXREAD()
{
}

/// <summary>
/// D3DXのパーサーにDx9のデバイスを使うから作成する
/// </summary>
/// <returns>成功か失敗(原因)</returns>
HRESULT D3DXREAD::InitDx9()
{
	auto& device = Devices::Get();
	//「Direct3D」オブジェクトの作成
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBoxA(0, "Direct3D9の作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	//「DIRECT3Dデバイス」オブジェクトの作成
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
			MessageBoxA(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &pDevice)))
			{
				if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &pDevice)))
				{
					MessageBoxA(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}


/// <summary>
/// Xファイルからメッシュをロードする
/// </summary>
/// <param name="FileName">ファイル名</param>
/// <returns>成功か失敗(原因)</returns>
HRESULT D3DXREAD::LoadXMesh(LPSTR FileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromXA(FileName, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBoxA(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
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
