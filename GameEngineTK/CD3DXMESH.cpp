#include "CD3DXMESH.h"

using namespace std;


HRESULT MY_H_HIERARCHY::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	*ppNewFrame = NULL;

	MYHFRAME* pFrame = new MYHFRAME;
	if (pFrame == nullptr)
	{
		return E_OUTOFMEMORY;
	}

	//ファイル名を生成
	//lstrlenAは文字列の長さを返す
	pFrame->Name = new CHAR[lstrlenA(Name) + 1];
	//ファイル名がNULLなら失敗
	if (!pFrame->Name)
	{
		return E_FAIL;
	}

	//引数で受け取ったファイル名をコピー
	strcpy(pFrame->Name, Name);

	//単位行列作成
	//座標
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	pFrame->pMeshContainer = nullptr;
	pFrame->pFrameSibling = nullptr;
	pFrame->pFrameFirstChild = nullptr;
	*ppNewFrame = pFrame;

	return S_OK;
}


//HRESULT MY_HIERARCHY::CreateMeshContainer
//メッシュコンテナーを作成する
HRESULT MY_H_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	auto& device = Devices::Get();

	HRESULT hr;
	MYHMESHCONTAINER *pMeshContainer = nullptr;
	int iFacesAmount;
	int iMaterial;
	LPDIRECT3DDEVICE9 pDvece = nullptr;
	LPD3DXMESH pMesh = nullptr;
	*ppMeshContainer = nullptr;

	pMesh = pMeshData->pMesh;
	pMeshContainer = new MYHMESHCONTAINER;

	if (pMeshContainer == nullptr)
	{
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer, sizeof(MYHMESHCONTAINER));

	pMeshContainer->Name = new CHAR[strlen(Name) + 1];
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}
	strcpy(pMeshContainer->Name, Name);
	pMesh->GetDevice(&pDvece);
	iFacesAmount = pMesh->GetNumFaces();

	// 当該メッシュが法線を持たない場合は法線を追加する
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pDvece, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
		{
			return E_FAIL;
		}

		pMesh = pMeshContainer->MeshData.pMesh;
		D3DXComputeNormals(pMesh, NULL);
	}
	else
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		pMesh->AddRef();
	}
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_OUTOFMEMORY;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMaterials[iMaterial].pTextureFilename != nullptr)
			{
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = new char[strlen(pMaterials[iMaterial].pTextureFilename) + 1];
				strcpy(pMeshContainer->pMaterials[iMaterial].pTextureFilename, pMaterials[iMaterial].pTextureFilename);
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = nullptr;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	*ppMeshContainer = pMeshContainer;
	(*ppMeshContainer)->pMaterials[0].pTextureFilename;
	pMeshContainer = nullptr;

	return S_OK;
}

/// <summary>
/// フレームを破棄する
/// </summary>
/// <param name="pFrameToFree"></param>
/// <returns></returns>
HRESULT MY_H_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	if (pFrameToFree->Name)
	{
		delete[] pFrameToFree->Name;
		pFrameToFree->Name = nullptr;
	}
	if (pFrameToFree)
	{
		delete pFrameToFree;
		pFrameToFree = nullptr;
	}
	
	return S_OK;
}

/// <summary>
///メッシュ コンテナ オブジェクトの割り当て解除をする。
/// </summary>
HRESULT MY_H_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYHMESHCONTAINER *pMeshContainer = (MYHMESHCONTAINER*)pMeshContainerBase;

	//スキン情報のインターフェイスを初期化する
	if (pMeshContainer->pSkinInfo)
	{
		pMeshContainer->pSkinInfo->Release();
		pMeshContainer->pSkinInfo = 0;
	}

	//メッシュ名を初期化する
	if (pMeshContainer->Name)
	{
		delete[] pMeshContainer->pSkinInfo;
		pMeshContainer->pSkinInfo = nullptr;
	}

	// 隣接情報を含むメッシュの三角形あたり3つのDWORDの配列へのポインタ。
	if (pMeshContainer->pAdjacency)
	{
		delete[] pMeshContainer->pAdjacency;
		pMeshContainer->pSkinInfo = nullptr;
	}

	//
	if (pMeshContainer->pMaterials)
	{
		delete[] pMeshContainer->pMaterials;
		pMeshContainer->pMaterials = nullptr;
	}


	if (pMeshContainer->ppTextures != nullptr)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			delete pMeshContainer->ppTextures[iMaterial];
			pMeshContainer->ppTextures[iMaterial] = 0;
		}
		if (pMeshContainer->MeshData.pMesh)
		{
			delete pMeshContainer->MeshData.pMesh;
			pMeshContainer->MeshData.pMesh = nullptr;
		}
	}
	if (pMeshContainer->MeshData.pMesh)
	{
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = 0;
	}

	if (pMeshContainer)
	{
		delete pMeshContainer;
		pMeshContainer = nullptr;
	}

	return S_OK;
}


/// <summary>
/// コンストラクタ
/// </summary>
CD3DXMESH::CD3DXMESH()
{
	ZeroMemory(this, sizeof(CD3DXMESH));
	//birthcnt = 0;
	m_fScale = 10.0f;
	//m_Texture = false;
}

/// <summary>
/// デストラクタ
/// </summary>
CD3DXMESH::~CD3DXMESH()
{
	//SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pDevice9);

	SAFE_RELEASE(m_pD3d9);

}

HRESULT CD3DXMESH::Init(LPSTR FileName)
{

	if (FAILED(InitDx9()))
	{
		return E_FAIL;
	}
	if (FAILED(LoadXAnimMesh(FileName)))
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
/// Dx9の初期化処理
/// D3DXのパーサーを使うためには、Dx9のデバイスが必要なので作成する。
/// </summary>
/// <returns>成功、失敗原因</returns>
HRESULT CD3DXMESH::InitDx9()
{

	auto& device = Devices::Get();


	// 「Direct3D」オブジェクトの作成
	if (NULL == (m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION)))
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



	if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, device.HWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBoxA(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, device.HWnd(),
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBoxA(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}


	return S_OK;
}

HRESULT CD3DXMESH::LoadXAnimMesh(LPSTR FileName)
{
	auto& devices = Devices::Get();

	m_pHierarchy = new MY_H_HIERARCHY;
	if (FAILED(D3DXLoadMeshHierarchyFromXA(FileName, D3DXMESH_MANAGED, m_pDevice9,
		m_pHierarchy, nullptr, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(nullptr, "Xファイルの読み込みに失敗しました", nullptr, MB_OK);
		return E_FAIL;
	}


	//この時点で、ファイルから取り出した全フレームがm_pFrameRootに入っている、
	//またアニメーションをコントロールするにはm_pAnimControllerが初期化されているはずなので、それを使う。

	//あとは、そこから必要な情報をとりだしつつ、かくフレームごとにアプリ独自のメッシュを構築していく
	BuildAllMesh(m_pFrameRoot);


	//アニメーショントラックを得る 　本サンプル添付のXファイルの場合は2セットだけだが100個までに対応できる
	for (DWORD i = 0; i<m_pAnimController->GetNumAnimationSets(); i++)
	{
		LPD3DXANIMATIONSET animSet;
		m_pAnimController->GetAnimationSet(i, &animSet);
		m_pAnimSet.emplace_back(animSet);
	}

	return S_OK;


}

/// <summary>
/// D3DXMESHからアプリメッシュを作成する
/// </summary>
/// <param name="pFrame"></param>
void CD3DXMESH::BuildAllMesh(D3DXFRAME* pFrame)
{		
	if (pFrame && pFrame->pMeshContainer)
	{
		CreateAppMeshFromD3DXMesh(pFrame);
	}

	if (pFrame->pFrameSibling != nullptr)
	{
		BuildAllMesh(pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild!=nullptr)
	{
		BuildAllMesh(pFrame->pFrameFirstChild);
	}

}


HRESULT CD3DXMESH::CreateAppMeshFromD3DXMesh(LPD3DXFRAME p)
{
	auto& device = Devices::Get();

	//この時点で、ファイルから取り出したメッシュデータが、Dx9のD3DXメッシュに入っている、
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//派生フレーム構造体
	MYHFRAME* pFrame = (MYHFRAME*)p;

	//D3DXメッシュ（ここから・・・）
	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;
	//アプリメッシュ（・・・ここにメッシュデータをコピーする）
	PARTS_MESH* pAppMesh = new PARTS_MESH;
	pAppMesh->Tex = false;

	////あとは、そこから好きな情報を取り出してDx11の自前メッシュに利用するだけ。
	pAppMesh->dwNumMaterial = pFrame->pMeshContainer->NumMaterials;
	pAppMesh->pMaterial = new MY_MATERIAL[pAppMesh->dwNumMaterial];
	pAppMesh->ppIndexBuffer = new ID3D11Buffer*[pAppMesh->dwNumMaterial];

	for (DWORD i = 0; i<pAppMesh->dwNumMaterial; i++)
	{
		//アンビエント
		pAppMesh->pMaterial[i].Ambient.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.r;
		pAppMesh->pMaterial[i].Ambient.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.g;
		pAppMesh->pMaterial[i].Ambient.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.b;
		pAppMesh->pMaterial[i].Ambient.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.a;
		//ディフューズ
		pAppMesh->pMaterial[i].Diffuse.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.r;
		pAppMesh->pMaterial[i].Diffuse.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.g;
		pAppMesh->pMaterial[i].Diffuse.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.b;
		pAppMesh->pMaterial[i].Diffuse.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.a;
		//スペキュラー
		pAppMesh->pMaterial[i].Specular.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.r;
		pAppMesh->pMaterial[i].Specular.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.g;
		pAppMesh->pMaterial[i].Specular.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.b;
		pAppMesh->pMaterial[i].Specular.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.a;
		//テクスチャーがあれば
		if (pFrame->pMeshContainer->pMaterials != nullptr &&
			lstrlenA(pFrame->pMeshContainer->pMaterials[i].pTextureFilename) > 0)
		{
			pAppMesh->Tex = true;
			strcpy(pAppMesh->pMaterial[i].szTextureName, pFrame->pMeshContainer->pMaterials[i].pTextureFilename);
			//テクスチャーを作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device.Device().Get(),
				pAppMesh->pMaterial[i].szTextureName, nullptr, nullptr, &pAppMesh->pMaterial[i].pTexture, nullptr)))
			{
				return E_FAIL;
			}
		}

	}

	//インデックスバッファーを作成
	//それに先立ち、メッシュの属性情報を得る。属性情報でインデックスバッファーから細かいマテリアルごとのインデックスバッファを分離できる

	D3DXATTRIBUTERANGE* pAttrTable = nullptr;
	DWORD NumAttr = 0;
	//pD3DXMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	pD3DXMesh->GetAttributeTable(pAttrTable, &NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[NumAttr];
	if (pD3DXMesh->GetAttributeTable(pAttrTable, &NumAttr))
	{
		MessageBoxA(0, "属性テーブル取得失敗", "", MB_OK);
		return E_FAIL;
	}
	//D3DXMESHの場合、ロックしないとD3DXインデックスバッファーから取り出せないのでロックする。

	//2バイトのインデックスの場合もありえる
	DWORD* pIndex = (DWORD*)new DWORD[pD3DXMesh->GetNumFaces() * 3];

	LPDIRECT3DINDEXBUFFER9 pIB;
	pD3DXMesh->GetIndexBuffer(&pIB);
	D3DINDEXBUFFER_DESC desc;
	pIB->GetDesc(&desc);

	if (desc.Format==D3DFMT_INDEX16)
	{
		WORD *wordIndex = nullptr;
		pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&wordIndex);
		for (int i = 0; i<pD3DXMesh->GetNumFaces() * 3; i++)
		{
			pIndex[i] = wordIndex[i];
		}

	}
	else if (desc.Format == D3DFMT_INDEX32)
	{
		DWORD *dwordIndex = nullptr;
		pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&dwordIndex);
		memcpy(pIndex, dwordIndex, pD3DXMesh->GetNumFaces() * 3);
	}

	//属性ごとのインデックスバッファを作成
	for (DWORD i = 0; i<NumAttr; i++)
	{
		//m_AttrID[i] = pAttrTable[i].AttribId;
		//Dx9のD3DMESHのインデックスバッファーからの情報で、Dx11のインデックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DWORD) * pAttrTable[i].FaceCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];//大きいインデックスバッファ内のオフセット(*3を忘れずに）
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		if (device.Device().Get()->CreateBuffer(&bd, &InitData, &pAppMesh->ppIndexBuffer[i])) 
		{
			return FALSE;
		}
		pAppMesh->pMaterial[i].dwNumFace = pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;
	SAFE_RELEASE(pIB);
	pD3DXMesh->UnlockIndexBuffer();

	delete pIndex;


	//バーテックスバッファーを作成

	//D3DXMESHの場合、ロックしないとD3DXバーテックスバッファーから取り出せないのでロックする。
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	pD3DXMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = pD3DXMesh->GetNumBytesPerVertex();
	BYTE *pVertices = nullptr;
	MY_VERTEX_NOTEX* pvVertex = nullptr;
	MY_VERTEX_TEX* pvVertexTex = nullptr;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		//Dx9のD3DMESHのバーテックスバッファーからの情報で、Dx11のバーテックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//テクスチャー座標がマイナス対策
		if (!pAppMesh->Tex)
		{
			pvVertex = (MY_VERTEX_NOTEX*)pVertices;
			bd.ByteWidth = sizeof(MY_VERTEX_NOTEX)*pD3DXMesh->GetNumVertices();
			InitData.pSysMem = pvVertex;
		}
		else
		{
			pvVertexTex = (MY_VERTEX_TEX*)pVertices;
			bd.ByteWidth = sizeof(MY_VERTEX_TEX) *pD3DXMesh->GetNumVertices();
			InitData.pSysMem = pvVertexTex;
		}

		if (device.Device().Get()->CreateBuffer(&bd, &InitData, &pAppMesh->pVertexBuffer))
			return FALSE;

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	pFrame->pPartsMesh = pAppMesh;

	////テクスチャー用サンプラー作成
	//D3D11_SAMPLER_DESC SamDesc;
	//ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//devices.Device().Get()->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return S_OK;

}





HRESULT CD3DXMESH::InitShader()
{
	auto& devices = Devices::Get();
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//テクスチャーなしシェーダー
	SAFE_RELEASE(pErrors);
	SAFE_RELEASE(pCompiledShader);
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS_NoTex", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShaderNoTex)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//テクスチャーなし
	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(layout2) / sizeof(layout2[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout2, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout_NoTex)))
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//テクスチャーなし
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS_NoTex", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShaderNoTex)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer0)))
	{
		return E_FAIL;
	}
	//コンスタントバッファー作成  マテリアル渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer1)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CD3DXMESH::UpdateHierarchyMatrices(D3DXFRAME * p, LPD3DXMATRIX pParentMatrix)
{
	MYHFRAME *pFrame = (MYHFRAME*)p;

	if (pParentMatrix != nullptr)
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix**pParentMatrix;
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if (pFrame->pFrameSibling != nullptr)
	{
		UpdateHierarchyMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != nullptr)
	{
		UpdateHierarchyMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}

}


void CD3DXMESH::Render(unique_ptr<FollowCamera>& camera , D3DXVECTOR3& vLight)
{
	auto& devices = Devices::Get();

	//プリミティブ・トポロジーをセット
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXMATRIX World;
	D3DXMatrixRotationY(&World, m_fYaw);
	UpdateHierarchyMatrices(m_pFrameRoot, &World);
	DrawFrame(m_pFrameRoot,camera, vLight);

}

void CD3DXMESH::DrawFrame(LPD3DXFRAME p, unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight)
{
	MYHFRAME* pFrame = (MYHFRAME*)p;
	PARTS_MESH* pPartsMesh = pFrame->pPartsMesh;
	if (pPartsMesh != nullptr)
	{
		DrawPartsMesh(pPartsMesh, pFrame->CombinedTransformationMatrix, camera, vLight);
	}

	if (pFrame->pFrameSibling != nullptr)
	{
		DrawFrame(pFrame->pFrameSibling,camera, vLight);
	}

	if (pFrame->pFrameFirstChild != nullptr)
	{
		DrawFrame(pFrame->pFrameFirstChild,camera, vLight);
	}

}


void CD3DXMESH::DrawPartsMesh(PARTS_MESH * pPartsMesh, D3DXMATRIX World , unique_ptr<FollowCamera>& camera , D3DXVECTOR3& vLight)
{


	//D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;
	////ワールドトランスフォーム（絶対座標変換）
	//D3DXMatrixScaling(&mScale, m_fScale, m_fScale, m_fScale);
	//D3DXMatrixRotationY(&mYaw, m_fYaw);
	//D3DXMatrixRotationX(&mPitch, m_fPitch);
	//D3DXMatrixRotationZ(&mRoll, m_fRoll);
	//D3DXMatrixTranslation(&mTran, m_vPos.x, m_vPos.y, m_vPos.z);

	//mWorld = mScale*mYaw*mPitch*mRoll*mTran;
	auto& devices = Devices::Get();
	D3DXMATRIX mWorld = World;
	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());




	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->GetEyePos());
	//D3DXVECTOR3 vLookatPt = shadermanager.VectorToD3DXVECTOR3(camera->GetRefPos());
	//D3DXVECTOR3 vUpVec = shadermanager.VectorToD3DXVECTOR3(camera->GetUpVec());


	//D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	//D3DXMatrixPerspectiveFovRH(&Proj, camera->m_fovY, camera->m_aspect, camera->m_NearClip, camera->m_FarClip);



	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SIMPLECONSTANT_BUFFER0 sg;
		//ワールド行列を渡す
		sg.mW = mWorld;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);
		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = mWorld*View*Proj;
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
		//ライトの方向を渡す
		sg.vLightDir = D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);
		//視点位置を渡す
		sg.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
		devices.Context().Get()->Unmap(m_pConstantBuffer0, 0);
	}
	//m_pSampleLinear = NULL;
	////テクスチャーをシェーダーに渡す
	//devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
	//devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);

	//このコンスタントバッファーを使うシェーダーの登録
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	//UINT stride = m_pMesh->GetNumBytesPerVertex();


	UINT stride = 0;
	if (pPartsMesh->Tex)
	{
		stride = sizeof(MY_VERTEX_TEX);
		//頂点インプットレイアウトをセット
		devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
		//使用するシェーダーの登録
		devices.Context().Get()->VSSetShader(m_pVertexShader, nullptr, 0);
		devices.Context().Get()->PSSetShader(m_pPixelShader, nullptr, 0);
	}
	else
	{
		stride = sizeof(MY_VERTEX_NOTEX);
		//頂点インプットレイアウトをセット
		devices.Context()->IASetInputLayout(m_pVertexLayout_NoTex);
		//使用するシェーダーの登録
		devices.Context()->VSSetShader(m_pVertexShaderNoTex, nullptr, 0);
		devices.Context()->PSSetShader(m_pPixelShaderNoTex, nullptr, 0);
	}

	//バーテックスバッファーをセット
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &pPartsMesh->pVertexBuffer, &stride, &offset);


	//属性の数だけ、それぞれの属性のインデックスバッファ－を描画
	for (DWORD i = 0; i < pPartsMesh->dwNumMaterial; i++)
	{
		//使用されていない対策
		if (pPartsMesh->pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		devices.Context().Get()->IASetIndexBuffer(pPartsMesh->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient = pPartsMesh->pMaterial[i].Ambient;//アンビエントををシェーダーに渡す
			sg.vDiffuse = pPartsMesh->pMaterial[i].Diffuse;//ディフューズカラーをシェーダーに渡す
			sg.vSpecular = pPartsMesh->pMaterial[i].Specular;//スペキュラーをシェーダーに渡す
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER1));
			devices.Context().Get()->Unmap(m_pConstantBuffer1, 0);
		}
		devices.Context().Get()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		devices.Context().Get()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);

		//テクスチャーをシェーダーに渡す
		if (pPartsMesh->Tex == true)
		{
			devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
			devices.Context().Get()->PSSetShaderResources(0, 1, &pPartsMesh->pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			devices.Context().Get()->PSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブをレンダリング
		devices.Context().Get()->DrawIndexed(pPartsMesh->pMaterial[i].dwNumFace * 3, 0, 0);

	}

}


void CD3DXMESH::ChangeAnimSet(int index)
{
	D3DXTRACK_DESC TrackDesc;
	ZeroMemory(&TrackDesc, sizeof(TrackDesc));

	TrackDesc.Weight = 1;
	TrackDesc.Speed = 1;
	TrackDesc.Enable = 1;
	m_pAnimController->SetTrackDesc(0, &TrackDesc);
	m_pAnimController->SetTrackAnimationSet(0, m_pAnimSet[index]);
	m_pAnimController->SetTrackEnable(index, true);
}