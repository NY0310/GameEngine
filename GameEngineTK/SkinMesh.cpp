#include <stdlib.h>
#include "SkinMesh.h"

//
//HRESULT MY_HIERARCHY::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
//フレームを作成する
HRESULT MY_HIERARCHY::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	MYFRAME *pFrame;

	*ppNewFrame = NULL;

	pFrame = new MYFRAME;
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	pFrame->Name = new CHAR[strlen(Name) + 1];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	strcpy(pFrame->Name, Name);

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	*ppNewFrame = pFrame;

	return S_OK;
}

//
//HRESULT MY_HIERARCHY::CreateMeshContainer
//メッシュコンテナーを作成する
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	*ppMeshContainer = NULL;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER *pMeshContainer = new MYMESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	pMeshContainer->Name = new CHAR[strlen(Name) + 1];
	strcpy(pMeshContainer->Name, Name);

	memcpy(&pMeshContainer->MeshData, pMeshData, sizeof(pMeshContainer->MeshData));
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	int NumPoly = pMeshContainer->MeshData.pMesh->GetNumFaces();
	//メッシュを複製する。　なぜかスキンメッシュだと、この関数を抜けた直後にD3DX内部でメッシュポインターがおかしくなってしまうので。
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pMeshContainer->MeshData.pMesh->GetDevice(&pDevice);
	LPD3DXMESH pMesh = NULL;
	pMeshContainer->MeshData.pMesh->CloneMesh(NULL, NULL, pDevice, &pMesh);
	//SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	pMeshContainer->MeshData.pMesh = pMesh;

	//メッシュのマテリアル設定
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[pMeshContainer->MeshData.pMesh->GetNumFaces() * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_FAIL;
	}
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumPoly * 3);

	if (NumMaterials > 0)
	{
		for (int i = 0; i<NumMaterials; i++)
		{
			memcpy(&pMeshContainer->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));
			if (pMaterials[i].pTextureFilename != NULL)
			{
				pMeshContainer->pMaterials[i].pTextureFilename = new CHAR[512];
				strcpy(pMeshContainer->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename);
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	//当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for (DWORD i = 0; i < dwBoneNum; i++)
		{
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		}
	}
	//ローカルに生成したメッシュコンテナーを呼び出し側にコピーする
	*ppMeshContainer = pMeshContainer;

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
//フレームを破棄する
HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
//メッシュコンテナーを破棄する
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);

	return S_OK;
}

//
//
//
HRESULT D3DXPARSER::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

	for (DWORD i = 0; i<dwBoneNum; i++)
	{
		pFrame = (MYFRAME*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;

	}
	return S_OK;
}
//
//
//
HRESULT D3DXPARSER::AllocateAllBoneMatrices(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//
//
//
HRESULT D3DXPARSER::LoadMeshFromX(LPDIRECT3DDEVICE9 pDevice9, LPSTR FileName)
{
	// Xファイルからアニメーションメッシュを読み込み作成する
	m_pHierarchy = new MY_HIERARCHY;
	if (FAILED(
		D3DXLoadMeshHierarchyFromXA(FileName, D3DXMESH_MANAGED, pDevice9, m_pHierarchy,
			NULL, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(NULL, "Xファイルの読み込みに失敗しました", FileName, MB_OK);
		return E_FAIL;
	}
	//ボーンメモリ割りあて
	AllocateAllBoneMatrices(m_pFrameRoot);
	//メッシュコンテナーを得る
	if (m_pFrameRoot->pMeshContainer)
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pMeshContainer;
	}
	else if (m_pFrameRoot->pFrameFirstChild->pMeshContainer)
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pFrameFirstChild->pMeshContainer;
	}
	else//もしもこれ以上深いノードのXファイルの場合は、適宜この部分を追加すること
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pFrameFirstChild->pFrameFirstChild->pMeshContainer;
	}

	return S_OK;
}
//
//VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
//フレーム内のメッシュ毎にワールド変換行列を更新する
VOID D3DXPARSER::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}
//
//
//
int D3DXPARSER::GetNumVertices()
{
	return m_pContainer->MeshData.pMesh->GetNumVertices();
}
//
//
//
int D3DXPARSER::GetNumFaces()
{
	return m_pContainer->MeshData.pMesh->GetNumFaces();
}
//
//
//
int D3DXPARSER::GetNumMaterials()
{
	return m_pContainer->NumMaterials;
}
//
//
//
int D3DXPARSER::GetNumUVs()
{
	return m_pContainer->MeshData.pMesh->GetNumVertices();
}
//
//
//指定されたボーンが影響を及ぼす頂点数を返す
int D3DXPARSER::GetNumBoneVertices(int iBoneIndex)
{
	return m_pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
}
//
//
//指定されたボーンが影響を及ぼす頂点のインデックスを返す 第2引数は、影響を受ける頂点のインデックスグループ内のインデックス（インデックスが若い順）
//例えばボーンに影響をうける頂点が４つだとして、そのボーンに影響をうける４つの頂点のうち2番目のインデックスを知りたい場合は、iIndexInGroupに1を指定する（0スタートなので）
DWORD D3DXPARSER::GetBoneVerticesIndices(int iBoneIndex, int iIndexInGroup)
{
	int Num = m_pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
	DWORD* pVerts = new DWORD[Num];
	float* pWights = new float[Num];

	if (FAILED(m_pContainer->pSkinInfo->GetBoneInfluence(iBoneIndex, pVerts, pWights)))
	{
		return E_FAIL;
	}
	return pVerts[iIndexInGroup];
}
//
//
//指定されたボーンが影響を及ぼす頂点のボーンウェイトを返す 第2引数は、影響を受ける頂点のインデックスグループ内のインデックス（インデックスが若い順）
//例えばボーンに影響をうける頂点が４つだとして、そのボーンに影響をうける４つの頂点のうち2番目の頂点のボーンウェイトを知りたい場合は、iIndexInGroupに1を指定する（0スタートなので）
double D3DXPARSER::GetBoneVerticesWeights(int iBoneIndex, int iIndexInGroup)
{
	int Num = m_pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
	DWORD* pVerts = new DWORD[Num];
	float* pWights = new float[Num];

	if (FAILED(m_pContainer->pSkinInfo->GetBoneInfluence(iBoneIndex, pVerts, pWights)))
	{
		return E_FAIL;
	}
	return pWights[iIndexInGroup];
}
//
//
//座標
D3DXVECTOR3 D3DXPARSER::GetVertexCoord(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR3* pCoord = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index*Stride;
		pCoord = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pCoord;
}
//
//
//法線
D3DXVECTOR3 D3DXPARSER::GetNormal(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR3* pNormal = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index*Stride;
		pVertices += sizeof(D3DXVECTOR3);//座標分進める 
		pNormal = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pNormal;
}
//
//
//テクスチャー座標
D3DXVECTOR2 D3DXPARSER::GetUV(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR2* pUV = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index*Stride;
		pVertices += sizeof(D3DXVECTOR3);//座標分進める
		pVertices += sizeof(D3DXVECTOR3);//法線分進める
		pUV = (D3DXVECTOR2*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pUV;
}
//
//
//インデックスバッファ内の指定した位置（インデックスインデックス）にある頂点インデックスを返す
int D3DXPARSER::GetIndex(DWORD Index)
{
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	m_pContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndex);

	VertIndex = pIndex[Index];

	m_pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}
//
//
//
D3DXVECTOR4 D3DXPARSER::GetAmbient(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Ambient;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}
//
//
//
D3DXVECTOR4 D3DXPARSER::GetDiffuse(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Diffuse;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}
//
//
//
D3DXVECTOR4 D3DXPARSER::GetSpecular(int iIndex)
{
	D3DXCOLOR color;

	color = m_pContainer->pMaterials[iIndex].MatD3D.Specular;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}
//
//
//
CHAR* D3DXPARSER::GetTexturePath(int index)
{
	CHAR* p = NULL;
	p = m_pContainer->pMaterials[index].pTextureFilename;
	return p;
}
//
//
//
float D3DXPARSER::GetSpecularPower(int iIndex)
{
	float power;

	power = m_pContainer->pMaterials[iIndex].MatD3D.Power;
	return power;
}
//
//
//そのポリゴンが、どのマテリアルであるかを返す 
int D3DXPARSER::GeFaceMaterialIndex(int iFaceIndex)
{
	int MaterialIndex = 0;
	DWORD* pBuf = NULL;
	if (SUCCEEDED(m_pContainer->MeshData.pMesh->LockAttributeBuffer(D3DLOCK_READONLY, &pBuf)))
	{
		MaterialIndex = pBuf[iFaceIndex];
	}
	else
	{
		MessageBoxA(0, "属性バッファのロック失敗", "", MB_OK);
	}
	return MaterialIndex;
}
//
//
//iFaceIndex番目のポリゴンを形成する3頂点の中で、iIndexInFace番目[0,2]の頂点のインデックスを返す
int D3DXPARSER::GetFaceVertexIndex(int iFaceIndex, int iIndexInFace)
{
	//インデックスバッファーを調べれば分かる
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	m_pContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndex);

	VertIndex = pIndex[iFaceIndex * 3 + iIndexInFace];

	m_pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}
//
//
//
D3DXMATRIX D3DXPARSER::GetBindPose(int iBoneIndex)
{
	return *m_pContainer->pSkinInfo->GetBoneOffsetMatrix(iBoneIndex);
}
//
//
//この関数をよぶごとにUpdateすると無駄なので、Render関数実行時にUpdateするものとしてそのまま渡す。
//親クラスがこのクラスのUpdateMatricesを実行しない場合は、UpdateMatricesしないと行列が最新にならないので留意。
D3DXMATRIX D3DXPARSER::GetNewPose(int iBoneIndex)
{
	MYMESHCONTAINER* pMyContaiber = (MYMESHCONTAINER*)m_pContainer;
	return *pMyContaiber->ppBoneMatrix[iBoneIndex];
}

//
//
//
CHAR* D3DXPARSER::GetBoneName(int iBoneIndex)
{
	return (CHAR*)m_pContainer->pSkinInfo->GetBoneName(iBoneIndex);
}
//
//
//
int D3DXPARSER::GetNumBones()
{
	return m_pContainer->pSkinInfo->GetNumBones();
}
//
//CD3DXSKINMESH::CD3DXSKINMESH()
//コンストラクター
CD3DXSKINMESH::CD3DXSKINMESH()
{
	///	ZeroMemory(this, sizeof(CD3DXSKINMESH));
	m_Scale = D3DXVECTOR3(1, 1, 1);
}
//
//CD3DXSKINMESH::~CD3DXSKINMESH()
//デストラクター
CD3DXSKINMESH::~CD3DXSKINMESH()
{
	delete[] m_BoneArray;
	delete[] m_pMaterial;
	SAFE_RELEASE(m_pVertexBuffer);
	for (int i = 0; i<m_dwNumMaterial; i++)
	{
		//SAFE_RELEASE(m_ppIndexBuffer[i]);
	}
	delete[] m_ppIndexBuffer;
}
//
//
//初期化
HRESULT CD3DXSKINMESH::Initialize()
{

	//D3d"9"のデバイスを作る、全てはD3DXMESHの引数に必要だから
	// Direct3D"9"オブジェクトの作成
	if (NULL == (m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBoxA(0, "Direct3D9の作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	// DIRECT3D"9"デバイスオブジェクトの作成
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_hWnd = Devices::Get().HWnd();
	if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBoxA(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				if (FAILED(m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBoxA(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	//D3D11関連の初期化
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"Resources/HLSL/Geometry_Material_Texture_Skin.hlsl", NULL, NULL, "VSSkin", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		int size = pErrors->GetBufferSize();
		char* ch = (char*)pErrors->GetBufferPointer();
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
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
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
		return FALSE;
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"Resources/HLSL/Geometry_Material_Texture_Skin.hlsl", NULL, NULL, "PSSkin", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	D3D11_BUFFER_DESC cb;
	//コンスタントバッファー0作成
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer0)))
	{
		return E_FAIL;
	}
	//コンスタントバッファー1作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer1)))
	{
		return E_FAIL;
	}
	//コンスタントバッファーボーン用　作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL_BONES);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBufferBone)))
	{
		return E_FAIL;
	}
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return S_OK;
}
//
//HRESULT CD3DXSKINMESH::ReadSkinInfo(KFbxMesh* pFbxMesh,MY_SKINVERTEX* pvVB)
//Xからスキン関連の情報を読み出す　
HRESULT CD3DXSKINMESH::ReadSkinInfo(MY_SKINVERTEX* pvVB)
{
	//Xから抽出すべき情報は、頂点ごとのボーンインデックス、頂点ごとのボーンウェイト、バインド行列、ポーズ行列　の4項目

	int i, k, m, n;
	int iNumVertex = m_pD3dxMesh->GetNumVertices();//頂点数
	int iNumBone = 0;//ボーン数


					 //ボーン	の個数を得る
	iNumBone = m_pD3dxMesh->GetNumBones();
	//それぞれのボーンに影響を受ける頂点を調べる　そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
	for (i = 0; i<iNumBone; i++)
	{
		int iNumIndex = m_pD3dxMesh->GetNumBoneVertices(i);//このボーンに影響を受ける頂点数
		int* piIndex = new int[iNumIndex];
		for (k = 0; k<iNumIndex; k++) piIndex[k] = m_pD3dxMesh->GetBoneVerticesIndices(i, k);
		double* pdWeight = new double[iNumIndex];
		for (k = 0; k<iNumIndex; k++) pdWeight[k] = m_pD3dxMesh->GetBoneVerticesWeights(i, k);
		//頂点側からインデックスをたどって、頂点サイドで整理する
		for (k = 0; k<iNumIndex; k++)
		{
			//XやCGソフトがボーン4本以内とは限らない。5本以上の場合は、重みの大きい順に4本に絞る 

			//ウェイトの大きさ順にソート（バブルソート）
			for (m = 4; m>1; m--)
			{
				for (n = 1; n<m; n++)
				{
					if (pvVB[piIndex[k]].bBoneWeight[n - 1] < pvVB[piIndex[k]].bBoneWeight[n])
					{
						float tmp = pvVB[piIndex[k]].bBoneWeight[n - 1];
						pvVB[piIndex[k]].bBoneWeight[n - 1] = pvVB[piIndex[k]].bBoneWeight[n];
						pvVB[piIndex[k]].bBoneWeight[n] = tmp;
						int itmp = pvVB[piIndex[k]].bBoneIndex[n - 1];
						pvVB[piIndex[k]].bBoneIndex[n - 1] = pvVB[piIndex[k]].bBoneIndex[n];
						pvVB[piIndex[k]].bBoneIndex[n] = itmp;
					}
				}
			}
			//ソート後は、最後の要素に一番小さいウェイトが入っているはず。
			bool flag = false;
			for (m = 0; m<4; m++)
			{
				if (pvVB[piIndex[k]].bBoneWeight[m] == 0)
				{
					flag = true;
					pvVB[piIndex[k]].bBoneIndex[m] = i;
					pvVB[piIndex[k]].bBoneWeight[m] = pdWeight[k];
					break;
				}
			}
			if (flag == false)
			{
				pvVB[piIndex[k]].bBoneIndex[3] = i;
				pvVB[piIndex[k]].bBoneWeight[3] = pdWeight[k];
				break;
			}

		}
		delete piIndex;
		delete pdWeight;
	}
	//
	//ボーンを生成
	m_iNumBone = iNumBone;
	m_BoneArray = new BONE[iNumBone];
	//ポーズ行列を得る 初期ポーズ
	for (i = 0; i<m_iNumBone; i++)
	{
		m_BoneArray[i].mBindPose = m_pD3dxMesh->GetBindPose(i);
	}

	return S_OK;
}
//
//HRESULT CD3DXSKINMESH::CreateFromX(CHAR* szFileName)
//Xからスキンメッシュを作成する　　注意）素材（X)のほうは、三角ポリゴンにすること
HRESULT CD3DXSKINMESH::CreateFromX(CHAR* szFileName)
{
	//Xファイル読み込み
	m_pD3dxMesh = new D3DXPARSER;
	m_pD3dxMesh->LoadMeshFromX(m_pDevice9, szFileName);

	//事前に頂点数、ポリゴン数等を調べる
	m_dwNumVert = m_pD3dxMesh->GetNumVertices();
	m_dwNumFace = m_pD3dxMesh->GetNumFaces();
	m_dwNumUV = m_pD3dxMesh->GetNumUVs();
	if (m_dwNumVert<m_dwNumUV)//Direct3DではUVの数だけ頂点が必要
	{
		//共有頂点等で、頂点数が足りない時
		MessageBox(0, L"Direct3Dは、UVの数だけ頂点が必要です（UVを置く場所が必要です）テクスチャーは正しく貼られないと思われます", NULL, MB_OK);
		return E_FAIL;
	}
	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	MY_SKINVERTEX* pvVB = new MY_SKINVERTEX[m_dwNumVert];
	int* piFaceBuffer = new int[m_dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

												 //頂点読み込み
	for (int i = 0; i<m_dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pD3dxMesh->GetVertexCoord(i);
		pvVB[i].vPos.x = v.x;
		pvVB[i].vPos.y = v.y;
		pvVB[i].vPos.z = v.z;
	}
	//ポリゴン情報（頂点インデックス）読み込み
	for (int i = 0; i<m_dwNumFace * 3; i++)
	{
		piFaceBuffer[i] = m_pD3dxMesh->GetIndex(i);
	}
	//法線読み込み
	for (int i = 0; i<m_dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pD3dxMesh->GetNormal(i);
		pvVB[i].vNorm.x = v.x;
		pvVB[i].vNorm.y = v.y;
		pvVB[i].vNorm.z = v.z;
	}
	//テクスチャー座標読み込み
	for (int i = 0; i<m_dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pD3dxMesh->GetUV(i);
		pvVB[i].vTex.x = v.x;
		pvVB[i].vTex.y = v.y;
	}
	//マテリアル読み込み
	m_dwNumMaterial = m_pD3dxMesh->GetNumMaterials();
	m_pMaterial = new MY_SKINMATERIAL[m_dwNumMaterial];

	//マテリアルの数だけインデックスバッファーを作成
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];
	for (int i = 0; i<m_dwNumMaterial; i++)
	{
		//環境光	
		m_pMaterial[i].Ka.x = m_pD3dxMesh->GetAmbient(i).y;
		m_pMaterial[i].Ka.y = m_pD3dxMesh->GetAmbient(i).z;
		m_pMaterial[i].Ka.z = m_pD3dxMesh->GetAmbient(i).w;
		m_pMaterial[i].Ka.w = m_pD3dxMesh->GetAmbient(i).x;
		//拡散反射光	
		m_pMaterial[i].Kd.x = m_pD3dxMesh->GetDiffuse(i).y;
		m_pMaterial[i].Kd.y = m_pD3dxMesh->GetDiffuse(i).z;
		m_pMaterial[i].Kd.z = m_pD3dxMesh->GetDiffuse(i).w;
		m_pMaterial[i].Kd.w = m_pD3dxMesh->GetDiffuse(i).x;
		//鏡面反射光
		m_pMaterial[i].Ks.x = m_pD3dxMesh->GetSpecular(i).y;
		m_pMaterial[i].Ks.y = m_pD3dxMesh->GetSpecular(i).z;
		m_pMaterial[i].Ks.z = m_pD3dxMesh->GetSpecular(i).w;
		m_pMaterial[i].Ks.w = m_pD3dxMesh->GetSpecular(i).x;

		//テクスチャー（ディフューズテクスチャーのみ）
		char* name = m_pD3dxMesh->GetTexturePath(i);
		if (name)
		{
			strcpy(m_pMaterial[i].szTextureName, name);
		}
		//テクスチャーを作成
		if (m_pMaterial[i].szTextureName[0] != 0 && FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))//絶対パスファイル名は、まず失敗すると思うが、、、
		{
			MessageBox(0, L"テクスチャー読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}

		//そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる
		int iCount = 0;
		int* pIndex = new int[m_dwNumFace * 3];//とりあえず、メッシュ内のポリゴン数でメモリ確保（個々のポリゴングループはかならずこれ以下になるが）

		for (int k = 0; k<m_dwNumFace; k++)
		{
			if (i == m_pD3dxMesh->GeFaceMaterialIndex(k))//もし i == k番目のポリゴンのマテリアル番号 なら
			{
				pIndex[iCount] = m_pD3dxMesh->GetFaceVertexIndex(k, 0);//k番目のポリゴンを作る頂点のインデックス　1個目
				pIndex[iCount + 1] = m_pD3dxMesh->GetFaceVertexIndex(k, 1);//2個目
				pIndex[iCount + 2] = m_pD3dxMesh->GetFaceVertexIndex(k, 2);//3個目
				iCount += 3;
			}
		}
		if (iCount>0) CreateIndexBuffer(iCount * sizeof(int), pIndex, &m_ppIndexBuffer[i]);
		delete pIndex;
		m_pMaterial[i].dwNumFace = iCount / 3;//そのマテリアル内のポリゴン数

	}
	//スキン情報（ジョイント、ウェイト　）読み込み
	ReadSkinInfo(pvVB);
	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_SKINVERTEX) *m_dwNumVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return FALSE;

	//一時的な入れ物は、もはや不要
	delete piFaceBuffer;
	if (pvVB) delete pvVB;

	return S_OK;
}
//
//HRESULT CD3DXSKINMESH::CreateIndexBuffer(DWORD dwSize,int* pIndex,ID3D11Buffer** ppIndexBuffer)
//Direct3Dのインデックスバッファー作成
HRESULT CD3DXSKINMESH::CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dwSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, ppIndexBuffer)))
	{
		return FALSE;
	}

	return S_OK;
}

//
//void CD3DXSKINMESH::SetNewPoseMatrices(int frame)
//ボーンを次のポーズ位置にセットする
void CD3DXSKINMESH::SetNewPoseMatrices(int iFrame)
{
	//望むフレームでUpdateすること。しないと行列が更新されない
	//m_pD3dxMesh->UpdateFrameMatrices(m_pD3dxMesh->m_pFrameRoot)をレンダリング時に実行すること

	//また、アニメーション時間に見合った行列を更新するのはD3DXMESHでは
	//アニメーションコントローラーが（裏で）やってくれるものなので、アニメーションコントローラー
	//を使ってアニメを進行させることも必要
	//	m_pD3dxMesh->m_pAnimController->AdvanceTime(....)をレンダリング時に実行すること

	for (int i = 0; i<m_iNumBone; i++)
	{
		m_BoneArray[i].mNewPose = m_pD3dxMesh->GetNewPose(i);
	}
}
//
//D3DXMATRIX CD3DXSKINMESH::GetCurrentPoseMatrix(int index)
//次の（現在の）ポーズ行列を返す
D3DXMATRIX CD3DXSKINMESH::GetCurrentPoseMatrix(int index)
{
	D3DXMATRIX ret = m_BoneArray[index].mBindPose*m_BoneArray[index].mNewPose;
	return ret;
}
//
//D3DXMATRIX CD3DXSKINMESH::GetBindPoseMatrix(int index)
//バインドポーズ行列を返す
D3DXMATRIX CD3DXSKINMESH::GetBindPoseMatrix(int index)
{
	return m_BoneArray[index].mBindPose;
}
//
//
//
CHAR* CD3DXSKINMESH::GetBoneNames(int iBoneIndex)
{
	return m_pD3dxMesh->GetBoneName(iBoneIndex);
}
//
//void CD3DXSKINMESH::Render()
//レンダリング
void CD3DXSKINMESH::Render()
{
	D3D11_MAPPED_SUBRESOURCE pData;

	//使用するシェーダーのセット
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	//ワールド行列
	D3DXMATRIX Scale, Yaw, Pitch, Roll, Tran;
	D3DXMatrixScaling(&Scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationY(&Yaw, m_Yaw);
	D3DXMatrixRotationX(&Pitch, m_Pitch);
	D3DXMatrixRotationZ(&Roll, m_Roll);
	m_Rotation = Yaw*Pitch*Roll;
	D3DXMatrixTranslation(&Tran, 1, 1, 1);
	m_World = Scale*m_Rotation*Tran;

	//アニメーションフレームを進める　スキンを更新
	static int iFrame = 0;
	iFrame += 1000;
	if (iFrame >= 3600)
	{
		iFrame = 0;
	}
	SetNewPoseMatrices(iFrame);
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBufferBone, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_GLOBAL_BONES sg;
		for (int i = 0; i<m_iNumBone; i++)
		{
			D3DXMATRIX mat = GetCurrentPoseMatrix(i);
			D3DXMatrixTranspose(&mat, &mat);
			sg.mBone[i] = mat;
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_GLOBAL_BONES));
		m_pDeviceContext->Unmap(m_pConstantBufferBone, 0);
	}
	m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pConstantBufferBone);
	m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pConstantBufferBone);

	//バーテックスバッファーをセット（バーテックスバッファーは一つでいい）
	UINT stride = sizeof(MY_SKINVERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//カメラ位置をシェーダーに渡す
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_SKIN_GLOBAL0 sg;
		sg.vLightDir = D3DXVECTOR4(1, 1, -1, 0.0f);
		sg.vEye = D3DXVECTOR4(m_Eye.x, m_Eye.y, m_Eye.z, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_SKIN_GLOBAL0));
		m_pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i<m_dwNumMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (m_pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		m_pDeviceContext->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//マテリアルの各要素と変換行列をシェーダーに渡す			
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_SKIN_GLOBAL1 sg;
			sg.mW = m_World;
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			sg.mWVP = m_World*m_View*m_Proj;
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			sg.vAmbient = m_pMaterial[i].Ka;
			sg.vDiffuse = m_pMaterial[i].Kd;
			sg.vSpecular = m_pMaterial[i].Ks;
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_SKIN_GLOBAL1));
			m_pDeviceContext->Unmap(m_pConstantBuffer1, 0);
		}
		m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		//テクスチャーをシェーダーに渡す
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pDeviceContext->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);
		}
		//Draw
		m_pDeviceContext->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
	//アニメ進行
	if (m_pD3dxMesh->m_pAnimController)
	{
		m_pD3dxMesh->m_pAnimController->AdvanceTime(0.001666, NULL);
	}
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	m_pD3dxMesh->UpdateFrameMatrices(m_pD3dxMesh->m_pFrameRoot, &m);
}