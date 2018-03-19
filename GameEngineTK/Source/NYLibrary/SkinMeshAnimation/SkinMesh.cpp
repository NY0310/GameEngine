#include <stdlib.h>
#include "SkinMesh.h"

using namespace std;
using namespace NYLibrary;

/// <summary>
/// フレーム作成
/// </summary>
/// <param name="Name">フレーム名</param>
/// <param name="ppNewFrame">フレーム</param>
/// <returns></returns>
HRESULT MY_HIERARCHY::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	DerivedFrame *pFrame;

	*ppNewFrame = nullptr;

	pFrame = new DerivedFrame;
	if (pFrame == nullptr)
	{
		return E_OUTOFMEMORY;
	}
	//フレーム名のメモリ確保(ナル文字の為に+1する)
	pFrame->Name = new CHAR[strlen(Name) + 1];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	//フレーム名をコピー
	strcpy(pFrame->Name, Name);

	//トランスフォーム行列を単位行列として初期化する
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	//ワールド行列を単位行列として初期化する
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	*ppNewFrame = pFrame;

	return S_OK;
}


/// <summary>
/// メッシュコンテナ作成
/// </summary>
/// <param name="Name">メッシュ名</param>
/// <param name="pMeshData">メッシュデータ</param>
/// <param name="pMaterials">マテリアルデータ</param>
/// <param name="pEffectInstances">エフェクトデータ</param>
/// <param name="NumMaterials">マテリアル数</param>
/// <param name="pAdjacency">隣接ポリゴンインデックス</param>
/// <param name="pSkinInfo">スキン情報</param>
/// <param name="ppMeshContainer">メッシュコンテナ</param>
/// <returns>実行結果</returns>
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	*ppMeshContainer = nullptr;
	DWORD dwBoneNum = 0;

	//メッシュコンテナの情報を入れるためのポインタ(最終的に引数にアドレスを渡す)
	MYMESHCONTAINER *pMeshContainer = new MYMESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	//メッシュ名+ナル文字(1)分のメモリを確保する
	pMeshContainer->Name = new CHAR[strlen(Name) + 1];
	//メッシュ名をコピー
	strcpy(pMeshContainer->Name, Name);

	//メッシュデータのメモリをコピー
	memcpy(&pMeshContainer->MeshData, pMeshData, sizeof(pMeshContainer->MeshData));
	//データの種類をメッシュと指定する
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//メッシュのポリゴン数を取得
	int NumPoly = pMeshContainer->MeshData.pMesh->GetNumFaces();
	//メッシュを複製する。　なぜかスキンメッシュだと、この関数を抜けた直後にD3DX内部でメッシュポインターがおかしくなってしまうので。
	{
		LPDIRECT3DDEVICE9 pDevice = nullptr;
		//メッシュに関連つけられているデバイスを取得
		pMeshContainer->MeshData.pMesh->GetDevice(&pDevice);
		LPD3DXMESH pMesh = nullptr;
		//頂点宣言を使って、メッシュのコピーを作成する
		pMeshContainer->MeshData.pMesh->CloneMesh(NULL, nullptr, pDevice, &pMesh);
		//SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		pMeshContainer->MeshData.pMesh = pMesh;
	}

	//メッシュのマテリアル設定
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	//全てのマテリアル分のメモリを確保
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	
	
	//pAdjacencyとは
	//ポリゴンに隣接するポリゴンのインデックス
	//DWORDの3つの塊を1つとしてポリゴン数だけ存在する配列
	//をコピーする
	{
		//隣接ポリゴンインデックス(メッシュ内のポリゴンの数 *　3)の
		pMeshContainer->pAdjacency = new DWORD[NumPoly * 3];
		//隣接するポリゴンが存在しないまたはマテリアル情報がない
		if ((pMeshContainer->pAdjacency == nullptr) || (pMeshContainer->pMaterials == nullptr))
		{
			return E_FAIL;
		}
		//メッシュコンテナに隣接ポリゴンインデックスをコピーする
		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumPoly * 3);
	}

	//マテリアル情報をコピーする
	{
		if (NumMaterials > 0)
		{
			for (int i = 0; i < NumMaterials; i++)
			{
				//メッシュコンテナにマテリアルのメモリをコピーする
				memcpy(&pMeshContainer->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));
				if (pMaterials[i].pTextureFilename != nullptr)
				{
					pMeshContainer->pMaterials[i].pTextureFilename = new CHAR[512];
					strcpy(pMeshContainer->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename);
				}
			}
		}
		else
		{
			//マテリアル情報がない場合は決め打ちする
			pMeshContainer->pMaterials[0].pTextureFilename = nullptr;
			memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}
	}
	//当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != nullptr)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		//D3DXLoadHierarchyFromXA関数を呼び出したときに参照カウンタがデクリメントされるのでインクリメント
		pSkinInfo->AddRef();
		//ボーンの数を取得
		dwBoneNum = pSkinInfo->GetNumBones();
		//フレームとしてのボーンのワールド行列のポインター	
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		//全てのフレームのボーンのワールド行列のメモリをコピー
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

/// <summary>
/// フレームを削除
/// </summary>
/// <param name="pFrameToFree">フレーム</param>
/// <returns>実行結果</returns>
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


/// <summary>
/// メッシュコンテナ削除
/// </summary>
/// <param name="pMeshContainerBase">メッシュコンテナ</param>
/// <returns>実行結果</returns>
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	if (pMeshContainer->ppTextures != nullptr)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if (pMeshContainer->boneBuffer != nullptr)
	{
		SAFE_RELEASE(pMeshContainer->boneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);

	return S_OK;
}


/// <summary>
/// メッシュコンテナの全ボーンの行列を格納	
/// </summary>
/// <param name="pMeshContainerBase"></param>
/// <returns></returns>
HRESULT D3DXPARSER::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DerivedFrame *pFrame = nullptr;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	//スキン情報インターフェイスへのポインタがnull
	if (pMeshContainer->pSkinInfo == nullptr)
	{
		return S_OK;
	}

	//ボーンの数を取得
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	//マトリックスのアドレスのアドレスを格納
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

	for (DWORD i = 0; i<dwBoneNum; i++)
	{
		//ルートフレームの子フレームを検索する
		//D3DXFrameFind(ルートフレームのポインタ、検索する子フレームの名前)
		pFrame = (DerivedFrame*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));

		//子フレームが見つからない
		if (pFrame == nullptr)
		{
			return E_FAIL;
		}
		//ボーンのマトリックスに子フレームのマトリックスを格納する
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;

	}
	return S_OK;
}

/// <summary>
/// ルートフレームから再帰で兄弟、子のボーン情報を検索
/// </summary>
/// <param name="pFrame"></param>
/// <returns></returns>
HRESULT D3DXPARSER::AllocateAllBoneMatrices(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != nullptr)
	{
		if (FAILED(AllocateBoneMatrix(pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	//兄弟がいるか
	if (pFrame->pFrameSibling != nullptr)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	//子がいるか
	if (pFrame->pFrameFirstChild != nullptr)
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
			nullptr, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(nullptr, "Xファイルの読み込みに失敗しました", FileName, MB_OK);
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
void D3DXPARSER::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	DerivedFrame *pFrame = (DerivedFrame*)pFrameBase;

	if (pParentMatrix != nullptr)
	{
		//ワールド行列にトランスフォーム行列を掛ける
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	//兄弟フレームの行列をワールド行列に
	if (pFrame->pFrameSibling != nullptr)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	//子フレームの行列をワールド行列に
	if (pFrame->pFrameFirstChild != nullptr)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


/// <summary>
/// 頂点の数を取得
/// </summary>
/// <returns>頂点の数</returns>
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
/// <summary>
/// 
/// </summary>
/// <param name="iBoneIndex"></param>
/// <returns></returns>
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
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = nullptr;
	D3DXVECTOR3* pCoord = nullptr;

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
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = nullptr;
	D3DXVECTOR3* pNormal = nullptr;

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

/// <summary>
/// UV座標を取得
/// </summary>
/// <param name="Index"></param>
/// <returns></returns>
D3DXVECTOR2 D3DXPARSER::GetUV(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	//バーテックスバッファー取得
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	//頂点
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = nullptr;
	D3DXVECTOR2* pUV = nullptr;

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
	WORD* pIndex = nullptr;
	m_pContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndex);

	VertIndex = pIndex[Index];

	m_pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}

/// <summary>
/// 環境光を取得する
/// </summary>
/// <param name="iIndex">マテリアルのインデックス</param>
/// <returns>環境光</returns>
D3DXVECTOR4 D3DXPARSER::GetAmbient(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Ambient;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}

/// <summary>
/// 拡散反射光
/// </summary>
/// <param name="iIndex">マテリアルのインデックス</param>
/// <returns></returns>
D3DXVECTOR4 D3DXPARSER::GetDiffuse(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Diffuse;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}

/// <summary>
/// 鏡面反射光
/// </summary>
/// <param name="iIndex">マテリアルのインデックス</param>
/// <returns>鏡面反射光</returns>
D3DXVECTOR4 D3DXPARSER::GetSpecular(int iIndex)
{
	D3DXCOLOR color;

	color = m_pContainer->pMaterials[iIndex].MatD3D.Specular;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}


/// <summary>
/// マテリアルのテクスチャ名を取得
/// </summary>
/// <param name="index">マテリアルのインデックス</param>
/// <returns>テクスチャファイル名</returns>
CHAR* D3DXPARSER::GetTexturePath(int index)
{
	CHAR* p = nullptr;
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
int D3DXPARSER::GetFaceMaterialIndex(int iFaceIndex)
{
	int MaterialIndex = 0;
	DWORD* pBuf = nullptr;
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
	WORD* pIndex = nullptr;
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


/// <summary>
/// コンストラクタ
/// </summary>
SkinMesh::SkinMesh()
{
	///	ZeroMemory(this, sizeof(SkinMesh));
}


/// <summary>
/// デストラクタ
/// </summary>
SkinMesh::~SkinMesh()
{
	delete[] boneArray;
	delete[] material;
	vertexBuffer.Reset();
	for (int i = 0; i<numMaterial; i++)
	{
		//SAFE_RELEASE(m_ppIndexBuffer[i]);
	}
	delete[] pIndexBuffer;
}

/// <summary>
/// //初期化
/// </summary>
/// <returns>実行結果</returns>
void SkinMesh::Initialize()
{
	ID3D11Device* device = Devices::Get().Device().Get();
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();


	//D3D11関連の初期化
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(MakeShader("Resources/HLSL/Geometry_Material_Texture_Skin.hlsl", "VSSkin", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)));
		//return E_FAIL;

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
	if (FAILED(device->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf())))
		//return FALSE;
	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(vertexLayout.Get());

	//ブロブからピクセルシェーダー作成
	if (FAILED(MakeShader("Resources/HLSL/Geometry_Material_Texture_Skin.hlsl", "PSSkin", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)));
		//return E_FAIL;


	D3D11_BUFFER_DESC cb;
	//コンスタントバッファー0作成
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(LightAndEyeGlobal);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, nullptr, lightAndEyeBuffer.ReleaseAndGetAddressOf())))
	{
		//return E_FAIL;
	}
	//コンスタントバッファー1作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SkinMeshGlobal);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, nullptr, skinMeshBuffer.ReleaseAndGetAddressOf())))
	{
		//return E_FAIL;
	}
	//コンスタントバッファーボーン用　作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(BoneGlobal);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cb, nullptr, boneBuffer.ReleaseAndGetAddressOf())))
	{
		//return E_FAIL;
	}
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	device->CreateSamplerState(&SamDesc, sampleLinear.ReleaseAndGetAddressOf());

//	return S_OK;
}
//
//HRESULT SkinMesh::ReadSkinInfo(KFbxMesh* pFbxMesh,SkinVertex* pvVB)
//Xからスキン関連の情報を読み出す　
HRESULT SkinMesh::ReadSkinInfo(SkinVertex* pvVB)
{
	//Xから抽出すべき情報は、頂点ごとのボーンインデックス、頂点ごとのボーンウェイト、バインド行列、ポーズ行列　の4項目

	int i, k, m, n;
	int iNumVertex = d3dxMesh->GetNumVertices();//頂点数
	int iNumBone = 0;//ボーン数


	//ボーンの個数を得る
	iNumBone = d3dxMesh->GetNumBones();
	//それぞれのボーンに影響を受ける頂点を調べる　そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
	for (i = 0; i<iNumBone; i++)
	{
		//このボーンに影響を受ける頂点数
		int iNumIndex = d3dxMesh->GetNumBoneVertices(i);
		int* piIndex = new int[iNumIndex];
		for (k = 0; k<iNumIndex; k++) piIndex[k] = d3dxMesh->GetBoneVerticesIndices(i, k);
		double* pdWeight = new double[iNumIndex];
		for (k = 0; k<iNumIndex; k++) pdWeight[k] = d3dxMesh->GetBoneVerticesWeights(i, k);
		//頂点側からインデックスをたどって、頂点サイドで整理する
		for (k = 0; k<iNumIndex; k++)
		{
			//XやCGソフトがボーン4本以内とは限らない。5本以上の場合は、重みの大きい順に4本に絞る 

			//ウェイトの大きさ順にソート（バブルソート）
			for (m = 4; m>1; m--)
			{
				for (n = 1; n<m; n++)
				{
					if (pvVB[piIndex[k]].boneWeight[n - 1] < pvVB[piIndex[k]].boneWeight[n])
					{
						float tmp = pvVB[piIndex[k]].boneWeight[n - 1];
						pvVB[piIndex[k]].boneWeight[n - 1] = pvVB[piIndex[k]].boneWeight[n];
						pvVB[piIndex[k]].boneWeight[n] = tmp;
						int itmp = pvVB[piIndex[k]].boneIndex[n - 1];
						pvVB[piIndex[k]].boneIndex[n - 1] = pvVB[piIndex[k]].boneIndex[n];
						pvVB[piIndex[k]].boneIndex[n] = itmp;
					}
				}
			}
			//ソート後は、最後の要素に一番小さいウェイトが入っているはず。
			bool flag = false;
			for (m = 0; m<4; m++)
			{
				if (pvVB[piIndex[k]].boneWeight[m] == 0)
				{					 
					flag = true;	 
					pvVB[piIndex[k]].boneIndex[m] = i;
					pvVB[piIndex[k]].boneWeight[m] = pdWeight[k];
					break;
				}
			}
			if (flag == false)
			{
				pvVB[piIndex[k]].boneIndex[3] = i;
				pvVB[piIndex[k]].boneWeight[3] = pdWeight[k];
				break;
			}

		}
		delete piIndex;
		delete pdWeight;
	}
	//
	//ボーンを生成
	numBone = iNumBone;
	boneArray = new Bone[iNumBone];
	//ポーズ行列を得る 初期ポーズ
	for (i = 0; i<numBone; i++)
	{
		boneArray[i].bindPose = d3dxMesh->GetBindPose(i);
	}

	return S_OK;
}
//
//HRESULT SkinMesh::CreateFromX(CHAR* szFileName)
//Xからスキンメッシュを作成する　　注意）素材（X)のほうは、三角ポリゴンにすること
HRESULT SkinMesh::CreateFromX(CHAR* szFileName)
{
	ID3D11Device* device = Devices::Get().Device().Get();
	
	//Xファイル読み込み
	d3dxMesh = new D3DXPARSER;
	d3dxMesh->LoadMeshFromX(Devices::Get().Device9(), szFileName);

	//事前に頂点数、ポリゴン数等を調べる
	//頂点数
	numVert = d3dxMesh->GetNumVertices();
	//ポリゴン数
	numFace = d3dxMesh->GetNumFaces();
	//Uv数
	numUV = d3dxMesh->GetNumUVs();

	
	//Direct3DではUVの数だけ頂点が必要
	if (numVert<numUV)
	{
		//共有頂点等で、頂点数が足りない時
		MessageBox(0, L"Direct3Dは、UVの数だけ頂点が必要です（UVを置く場所が必要です）テクスチャーは正しく貼られないと思われます", nullptr, MB_OK);
		return E_FAIL;
	}


	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	SkinVertex* pvVB = new SkinVertex[numVert];
	//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
	int* piFaceBuffer = new int[numFace * 3];


	D3DXVECTOR3 verPos[3];
	D3DXVECTOR2* verUv = new D3DXVECTOR2[3];

	Triangle tri;
	//頂点読み込み											
	for (int i = 0; i<numVert; i++)
	{
		D3DXVECTOR3 v = d3dxMesh->GetVertexCoord(i);

		pvVB[i].pos.x = v.x;
		pvVB[i].pos.y = v.y;
		pvVB[i].pos.z = v.z;
		verPos[i % 3] = v;
	
			;


		D3DXVECTOR2 uv = d3dxMesh->GetUV(i);
		pvVB[i].uv.x = uv.x;
		pvVB[i].uv.y = uv.y;
		verUv[i % 3] = uv;


		if (i % 3 == 2)
		{
			ComputeTriangle(verPos[0], verPos[1],verPos[2], &tri);
			tri.uv0 = verUv[0];
			tri.uv1 = verUv[1];
			tri.uv2 = verUv[2];
			triangles.emplace_back(tri);
		}
	}
	//ポリゴン情報（頂点インデックス）読み込み
	for (int i = 0; i<numFace * 3; i++)
	{
		piFaceBuffer[i] = d3dxMesh->GetIndex(i);
	}
	//法線読み込み
	for (int i = 0; i<numVert; i++)
	{
		D3DXVECTOR3 v = d3dxMesh->GetNormal(i);
		pvVB[i].norm.x = v.x;
		pvVB[i].norm.y = v.y;
		pvVB[i].norm.z = v.z;
	}

	//マテリアル読み込み
	//マテリアル数
	numMaterial = d3dxMesh->GetNumMaterials();
	material = new SkinMaterial[numMaterial];

	//マテリアルの数だけインデックスバッファーを作成
	pIndexBuffer = new ComPtr<ID3D11Buffer>[numMaterial];
	for (int i = 0; i<numMaterial; i++)
	{
		//環境光	
		material[i].Ka.x = d3dxMesh->GetAmbient(i).y;
		material[i].Ka.y = d3dxMesh->GetAmbient(i).z;
		material[i].Ka.z = d3dxMesh->GetAmbient(i).w;
		material[i].Ka.w = d3dxMesh->GetAmbient(i).x;
		//拡散反射光	
		material[i].Kd.x = d3dxMesh->GetDiffuse(i).y;
		material[i].Kd.y = d3dxMesh->GetDiffuse(i).z;
		material[i].Kd.z = d3dxMesh->GetDiffuse(i).w;
		material[i].Kd.w = d3dxMesh->GetDiffuse(i).x;
		//鏡面反射光
		material[i].Ks.x = d3dxMesh->GetSpecular(i).y;
		material[i].Ks.y = d3dxMesh->GetSpecular(i).z;
		material[i].Ks.z = d3dxMesh->GetSpecular(i).w;
		material[i].Ks.w = d3dxMesh->GetSpecular(i).x;

		//テクスチャー（ディフューズテクスチャーのみ）
		char* name = d3dxMesh->GetTexturePath(i);
		if (name)
		{
			strcpy(material[i].textureName, name);
		}
		//読み込んだテクスチャーを作成
		if (material[i].textureName[0] != 0 && FAILED(D3DX11CreateShaderResourceViewFromFileA(device, material[i].textureName, nullptr, nullptr, material[i].texture.ReleaseAndGetAddressOf(), nullptr)))//絶対パスファイル名は、まず失敗すると思うが、、、
		{
			MessageBox(0, L"テクスチャー読み込み失敗", nullptr, MB_OK);
			return E_FAIL;
		}

		//そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる
		int iCount = 0;
		//メッシュ内のポリゴン数でメモリ確保
		int* pIndex = new int[numFace * 3];

		for (int k = 0; k<numFace; k++)
		{
			if (i == d3dxMesh->GetFaceMaterialIndex(k))//もし i == k番目のポリゴンのマテリアル番号 なら
			{
				pIndex[iCount] = d3dxMesh->GetFaceVertexIndex(k, 0);//k番目のポリゴンを作る頂点のインデックス　1個目
				pIndex[iCount + 1] = d3dxMesh->GetFaceVertexIndex(k, 1);//2個目
				pIndex[iCount + 2] = d3dxMesh->GetFaceVertexIndex(k, 2);//3個目
				iCount += 3;

			}
		}

		//インデックスバッファ作成
		if (iCount>0) CreateIndexBuffer(iCount * sizeof(int), pIndex, pIndexBuffer[i].ReleaseAndGetAddressOf());
		delete pIndex;
		material[i].numFace = iCount / 3;//そのマテリアル内のポリゴン数

	}
	//スキン情報（ジョイント、ウェイト　）読み込み
	ReadSkinInfo(pvVB);
	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SkinVertex) *numVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;
	if (FAILED(device->CreateBuffer(&bd, &InitData, vertexBuffer.ReleaseAndGetAddressOf())))
		return FALSE;

	//一時的な入れ物は、もはや不要
	delete piFaceBuffer;
	if (pvVB) delete pvVB;
	/*if (verPos) delete verPos;
	if (verUv) delete verUv;*/

	return S_OK;
}
//
//HRESULT SkinMesh::CreateIndexBuffer(DWORD dwSize,int* pIndex,ID3D11Buffer** ppIndexBuffer)
//Direct3Dのインデックスバッファー作成
HRESULT SkinMesh::CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer)
{
	ID3D11Device* device = Devices::Get().Device().Get();


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
	if (FAILED(device->CreateBuffer(&bd, &InitData, ppIndexBuffer)))
	{
		return FALSE;
	}

	return S_OK;
}

//
//void SkinMesh::SetNewPoseMatrices(int frame)
//ボーンを次のポーズ位置にセットする
void SkinMesh::SetNewPoseMatrices(int iFrame)
{
	//望むフレームでUpdateすること。しないと行列が更新されない
	//d3dxMesh->UpdateFrameMatrices(d3dxMesh->m_pFrameRoot)をレンダリング時に実行すること

	//また、アニメーション時間に見合った行列を更新するのはD3DXMESHでは
	//アニメーションコントローラーが（裏で）やってくれるものなので、アニメーションコントローラー
	//を使ってアニメを進行させることも必要
	//	d3dxMesh->m_pAnimController->AdvanceTime(....)をレンダリング時に実行すること

	for (int i = 0; i<numBone; i++)
	{
		boneArray[i].newPose = d3dxMesh->GetNewPose(i);
	}
}
//
//D3DXMATRIX SkinMesh::GetCurrentPoseMatrix(int index)
//次の（現在の）ポーズ行列を返す
D3DXMATRIX SkinMesh::GetCurrentPoseMatrix(int index)
{
	D3DXMATRIX ret = boneArray[index].bindPose*boneArray[index].newPose;
	return ret;
}
//
//D3DXMATRIX SkinMesh::GetBindPoseMatrix(int index)
//バインドポーズ行列を返す
D3DXMATRIX SkinMesh::GetBindPoseMatrix(int index)
{
	return boneArray[index].bindPose;
}

/// <summary>
/// ボーンの数を取得
/// </summary>
/// <param name="iBoneIndex">ボーンインデックス</param>
/// <returns>引数のインデックスの名前を取得</returns>
CHAR* SkinMesh::GetBoneNames(int iBoneIndex)
{
	return d3dxMesh->GetBoneName(iBoneIndex);
}

void SkinMesh::UpDateTriangles()
{
	//頂点数
	numVert = d3dxMesh->GetNumVertices();

	//ポリゴン数
	numFace = d3dxMesh->GetNumFaces();
	//Uv数
	numUV = d3dxMesh->GetNumUVs();


	//Direct3DではUVの数だけ頂点が必要
	if (numVert<numUV)
	{
		//共有頂点等で、頂点数が足りない時
		MessageBox(0, L"Direct3Dは、UVの数だけ頂点が必要です（UVを置く場所が必要です）テクスチャーは正しく貼られないと思われます", nullptr, MB_OK);
	//	return E_FAIL;
	}


	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	SkinVertex* pvVB = new SkinVertex[numVert];
	//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
	int* piFaceBuffer = new int[numFace * 3];


	D3DXVECTOR3 verPos[3];
	D3DXVECTOR2* verUv = new D3DXVECTOR2[3];

	Triangle tri;
	//頂点読み込み											
	for (int i = 0; i<numVert; i++)
	{
		D3DXVECTOR3 v = d3dxMesh->GetVertexCoord(i);

		pvVB[i].pos.x = v.x;
		pvVB[i].pos.y = v.y;
		pvVB[i].pos.z = v.z;
		verPos[i % 3] = v;

		;


		D3DXVECTOR2 uv = d3dxMesh->GetUV(i);
		pvVB[i].uv.x = uv.x;
		pvVB[i].uv.y = uv.y;
		verUv[i % 3] = uv;


		if (i % 3 == 2)
		{
			ComputeTriangle(verPos[0],verPos[1],verPos[2], &tri);
			tri.uv0 = verUv[0];
			tri.uv1 = verUv[1];
			tri.uv2 = verUv[2];
			triangles.emplace_back(tri);
		}
	}


}


/// <summary>
/// 描画
/// </summary>
void SkinMesh::Render()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	D3D11_MAPPED_SUBRESOURCE pData;
	FollowCamera* camera = FollowCamera::GetInstance();
	D3DXVECTOR3 eye =camera->GetEyePos();

	//使用するシェーダーのセット
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);



	//アニメーションフレームを進める　スキンを更新
	static int iFrame = 0;
	iFrame += 1000;
	if (iFrame >= 3600)
	{
		iFrame = 0;
	}
	SetNewPoseMatrices(iFrame);
	if (SUCCEEDED(deviceContext->Map(boneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		BoneGlobal sg;
		for (int i = 0; i<numBone; i++)
		{
			D3DXMATRIX mat = GetCurrentPoseMatrix(i);
			D3DXMatrixTranspose(&mat, &mat);
			sg.boneMatrix[i] = mat;
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(BoneGlobal));
		deviceContext->Unmap(boneBuffer.Get(), 0);
	}
	deviceContext->VSSetConstantBuffers(2, 1, boneBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(2, 1, boneBuffer.GetAddressOf());

	//バーテックスバッファーをセット（バーテックスバッファーは一つでいい）
	UINT stride = sizeof(SkinVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1,vertexBuffer.GetAddressOf(), &stride, &offset);

	//頂点インプットレイアウトをセット
	deviceContext->IASetInputLayout(vertexLayout.Get());

	//プリミティブ・トポロジーをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//カメラ位置をシェーダーに渡す
	if (SUCCEEDED(deviceContext->Map(lightAndEyeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		LightAndEyeGlobal sg;
		sg.vLightDir = D3DXVECTOR4(1, 1, -1, 0.0f);
		sg.vEye = D3DXVECTOR4(eye.x, eye.y, eye.z, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(LightAndEyeGlobal));
		deviceContext->Unmap(lightAndEyeBuffer.Get(), 0);
	}
	deviceContext->VSSetConstantBuffers(0, 1, lightAndEyeBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, lightAndEyeBuffer.GetAddressOf());

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i<numMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (material[i].numFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		deviceContext->IASetIndexBuffer(pIndexBuffer[i].Get(), DXGI_FORMAT_R32_UINT, 0);

		//マテリアルの各要素と変換行列をシェーダーに渡す			
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(deviceContext->Map(skinMeshBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SkinMeshGlobal sg;
			sg.mW = GetWorldMatrix();
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			sg.mWVP = GetWVP();
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			sg.vAmbient = material[i].Ka;
			sg.vDiffuse = material[i].Kd;
			sg.vSpecular = material[i].Ks;
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinMeshGlobal));
			deviceContext->Unmap(skinMeshBuffer.Get(), 0);
		}
		deviceContext->VSSetConstantBuffers(1, 1, skinMeshBuffer.GetAddressOf());
		deviceContext->PSSetConstantBuffers(1, 1, skinMeshBuffer.GetAddressOf());
		//テクスチャーをシェーダーに渡す
		if (material[i].textureName[0] != NULL)
		{
			deviceContext->PSSetSamplers(0, 1, sampleLinear.GetAddressOf());
			deviceContext->PSSetShaderResources(0, 1, material[i].texture.GetAddressOf());
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			deviceContext->PSSetShaderResources(0, 1, Nothing);
		}
		//Draw
		deviceContext->DrawIndexed(material[i].numFace * 3, 0, 0);
	}
	//アニメ進行
	if (d3dxMesh->m_pAnimController)
	{
		d3dxMesh->m_pAnimController->AdvanceTime(0.001666, nullptr);
	}
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	d3dxMesh->UpdateFrameMatrices(d3dxMesh->m_pFrameRoot, &m);
}