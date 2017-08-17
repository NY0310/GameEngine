/// <summary>
/// メッシュデータをファイルから取り出す
/// </summary>
#pragma once
#include<d3dx9.h>
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

class D3DXREAD
{
private:
	LPDIRECT3D9 pD3d;
	LPDIRECT3DDEVICE9 pDevice;
	D3DMATERIAL9* pMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;
	DWORD dwNumMaterials;
	LPD3DXMESH pMesh;

public:
	D3DXREAD();
	~D3DXREAD();
	HRESULT InitDx9();
	HRESULT LoadXMesh(LPSTR FileName);
	//getter
	const LPD3DXMESH& GetpMesh() {return pMesh;}
	char m_TextureFileName[8][256];//テクスチャーファイル名（８枚まで）

};

