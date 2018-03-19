#include <stdlib.h>
#include "SkinMesh.h"

using namespace std;
using namespace NYLibrary;

/// <summary>
/// �t���[���쐬
/// </summary>
/// <param name="Name">�t���[����</param>
/// <param name="ppNewFrame">�t���[��</param>
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
	//�t���[�����̃������m��(�i�������ׂ̈�+1����)
	pFrame->Name = new CHAR[strlen(Name) + 1];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	//�t���[�������R�s�[
	strcpy(pFrame->Name, Name);

	//�g�����X�t�H�[���s���P�ʍs��Ƃ��ď���������
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	//���[���h�s���P�ʍs��Ƃ��ď���������
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	*ppNewFrame = pFrame;

	return S_OK;
}


/// <summary>
/// ���b�V���R���e�i�쐬
/// </summary>
/// <param name="Name">���b�V����</param>
/// <param name="pMeshData">���b�V���f�[�^</param>
/// <param name="pMaterials">�}�e���A���f�[�^</param>
/// <param name="pEffectInstances">�G�t�F�N�g�f�[�^</param>
/// <param name="NumMaterials">�}�e���A����</param>
/// <param name="pAdjacency">�אڃ|���S���C���f�b�N�X</param>
/// <param name="pSkinInfo">�X�L�����</param>
/// <param name="ppMeshContainer">���b�V���R���e�i</param>
/// <returns>���s����</returns>
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	*ppMeshContainer = nullptr;
	DWORD dwBoneNum = 0;

	//���b�V���R���e�i�̏������邽�߂̃|�C���^(�ŏI�I�Ɉ����ɃA�h���X��n��)
	MYMESHCONTAINER *pMeshContainer = new MYMESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	//���b�V����+�i������(1)���̃��������m�ۂ���
	pMeshContainer->Name = new CHAR[strlen(Name) + 1];
	//���b�V�������R�s�[
	strcpy(pMeshContainer->Name, Name);

	//���b�V���f�[�^�̃��������R�s�[
	memcpy(&pMeshContainer->MeshData, pMeshData, sizeof(pMeshContainer->MeshData));
	//�f�[�^�̎�ނ����b�V���Ǝw�肷��
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//���b�V���̃|���S�������擾
	int NumPoly = pMeshContainer->MeshData.pMesh->GetNumFaces();
	//���b�V���𕡐�����B�@�Ȃ����X�L�����b�V�����ƁA���̊֐��𔲂��������D3DX�����Ń��b�V���|�C���^�[�����������Ȃ��Ă��܂��̂ŁB
	{
		LPDIRECT3DDEVICE9 pDevice = nullptr;
		//���b�V���Ɋ֘A�����Ă���f�o�C�X���擾
		pMeshContainer->MeshData.pMesh->GetDevice(&pDevice);
		LPD3DXMESH pMesh = nullptr;
		//���_�錾���g���āA���b�V���̃R�s�[���쐬����
		pMeshContainer->MeshData.pMesh->CloneMesh(NULL, nullptr, pDevice, &pMesh);
		//SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		pMeshContainer->MeshData.pMesh = pMesh;
	}

	//���b�V���̃}�e���A���ݒ�
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	//�S�Ẵ}�e���A�����̃��������m��
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	
	
	//pAdjacency�Ƃ�
	//�|���S���ɗאڂ���|���S���̃C���f�b�N�X
	//DWORD��3�̉��1�Ƃ��ă|���S�����������݂���z��
	//���R�s�[����
	{
		//�אڃ|���S���C���f�b�N�X(���b�V�����̃|���S���̐� *�@3)��
		pMeshContainer->pAdjacency = new DWORD[NumPoly * 3];
		//�אڂ���|���S�������݂��Ȃ��܂��̓}�e���A����񂪂Ȃ�
		if ((pMeshContainer->pAdjacency == nullptr) || (pMeshContainer->pMaterials == nullptr))
		{
			return E_FAIL;
		}
		//���b�V���R���e�i�ɗאڃ|���S���C���f�b�N�X���R�s�[����
		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumPoly * 3);
	}

	//�}�e���A�������R�s�[����
	{
		if (NumMaterials > 0)
		{
			for (int i = 0; i < NumMaterials; i++)
			{
				//���b�V���R���e�i�Ƀ}�e���A���̃��������R�s�[����
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
			//�}�e���A����񂪂Ȃ��ꍇ�͌��ߑł�����
			pMeshContainer->pMaterials[0].pTextureFilename = nullptr;
			memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}
	}
	//���Y���b�V�����X�L�����������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
	if (pSkinInfo != nullptr)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		//D3DXLoadHierarchyFromXA�֐����Ăяo�����Ƃ��ɎQ�ƃJ�E���^���f�N�������g�����̂ŃC���N�������g
		pSkinInfo->AddRef();
		//�{�[���̐����擾
		dwBoneNum = pSkinInfo->GetNumBones();
		//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[	
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		//�S�Ẵt���[���̃{�[���̃��[���h�s��̃��������R�s�[
		for (DWORD i = 0; i < dwBoneNum; i++)
		{
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		}
	}
	//���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[����
	*ppMeshContainer = pMeshContainer;

	return S_OK;
}

/// <summary>
/// �t���[�����폜
/// </summary>
/// <param name="pFrameToFree">�t���[��</param>
/// <returns>���s����</returns>
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
/// ���b�V���R���e�i�폜
/// </summary>
/// <param name="pMeshContainerBase">���b�V���R���e�i</param>
/// <returns>���s����</returns>
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
/// ���b�V���R���e�i�̑S�{�[���̍s����i�[	
/// </summary>
/// <param name="pMeshContainerBase"></param>
/// <returns></returns>
HRESULT D3DXPARSER::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DerivedFrame *pFrame = nullptr;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	//�X�L�����C���^�[�t�F�C�X�ւ̃|�C���^��null
	if (pMeshContainer->pSkinInfo == nullptr)
	{
		return S_OK;
	}

	//�{�[���̐����擾
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	//�}�g���b�N�X�̃A�h���X�̃A�h���X���i�[
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

	for (DWORD i = 0; i<dwBoneNum; i++)
	{
		//���[�g�t���[���̎q�t���[������������
		//D3DXFrameFind(���[�g�t���[���̃|�C���^�A��������q�t���[���̖��O)
		pFrame = (DerivedFrame*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));

		//�q�t���[����������Ȃ�
		if (pFrame == nullptr)
		{
			return E_FAIL;
		}
		//�{�[���̃}�g���b�N�X�Ɏq�t���[���̃}�g���b�N�X���i�[����
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;

	}
	return S_OK;
}

/// <summary>
/// ���[�g�t���[������ċA�ŌZ��A�q�̃{�[����������
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
	//�Z�킪���邩
	if (pFrame->pFrameSibling != nullptr)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	//�q�����邩
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
	// X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
	m_pHierarchy = new MY_HIERARCHY;
	if (FAILED(
		D3DXLoadMeshHierarchyFromXA(FileName, D3DXMESH_MANAGED, pDevice9, m_pHierarchy,
			nullptr, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(nullptr, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", FileName, MB_OK);
		return E_FAIL;
	}
	//�{�[�����������肠��
	AllocateAllBoneMatrices(m_pFrameRoot);
	//���b�V���R���e�i�[�𓾂�
	if (m_pFrameRoot->pMeshContainer)
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pMeshContainer;
	}
	else if (m_pFrameRoot->pFrameFirstChild->pMeshContainer)
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pFrameFirstChild->pMeshContainer;
	}
	else//����������ȏ�[���m�[�h��X�t�@�C���̏ꍇ�́A�K�X���̕�����ǉ����邱��
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pFrameFirstChild->pFrameFirstChild->pMeshContainer;
	}

	return S_OK;
}
//
//VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
//�t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V����
void D3DXPARSER::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	DerivedFrame *pFrame = (DerivedFrame*)pFrameBase;

	if (pParentMatrix != nullptr)
	{
		//���[���h�s��Ƀg�����X�t�H�[���s����|����
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	//�Z��t���[���̍s������[���h�s���
	if (pFrame->pFrameSibling != nullptr)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	//�q�t���[���̍s������[���h�s���
	if (pFrame->pFrameFirstChild != nullptr)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


/// <summary>
/// ���_�̐����擾
/// </summary>
/// <returns>���_�̐�</returns>
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
//�w�肳�ꂽ�{�[�����e�����y�ڂ����_����Ԃ�
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
//�w�肳�ꂽ�{�[�����e�����y�ڂ����_�̃C���f�b�N�X��Ԃ� ��2�����́A�e�����󂯂钸�_�̃C���f�b�N�X�O���[�v���̃C���f�b�N�X�i�C���f�b�N�X���Ⴂ���j
//�Ⴆ�΃{�[���ɉe���������钸�_���S���Ƃ��āA���̃{�[���ɉe����������S�̒��_�̂���2�Ԗڂ̃C���f�b�N�X��m�肽���ꍇ�́AiIndexInGroup��1���w�肷��i0�X�^�[�g�Ȃ̂Łj
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
//�w�肳�ꂽ�{�[�����e�����y�ڂ����_�̃{�[���E�F�C�g��Ԃ� ��2�����́A�e�����󂯂钸�_�̃C���f�b�N�X�O���[�v���̃C���f�b�N�X�i�C���f�b�N�X���Ⴂ���j
//�Ⴆ�΃{�[���ɉe���������钸�_���S���Ƃ��āA���̃{�[���ɉe����������S�̒��_�̂���2�Ԗڂ̒��_�̃{�[���E�F�C�g��m�肽���ꍇ�́AiIndexInGroup��1���w�肷��i0�X�^�[�g�Ȃ̂Łj
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
//���W
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
//�@��
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
		pVertices += sizeof(D3DXVECTOR3);//���W���i�߂� 
		pNormal = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pNormal;
}

/// <summary>
/// UV���W���擾
/// </summary>
/// <param name="Index"></param>
/// <returns></returns>
D3DXVECTOR2 D3DXPARSER::GetUV(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	//�o�[�e�b�N�X�o�b�t�@�[�擾
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	//���_
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = nullptr;
	D3DXVECTOR2* pUV = nullptr;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index*Stride;
		pVertices += sizeof(D3DXVECTOR3);//���W���i�߂�
		pVertices += sizeof(D3DXVECTOR3);//�@�����i�߂�
		pUV = (D3DXVECTOR2*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pUV;
}
//
//
//�C���f�b�N�X�o�b�t�@���̎w�肵���ʒu�i�C���f�b�N�X�C���f�b�N�X�j�ɂ��钸�_�C���f�b�N�X��Ԃ�
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
/// �������擾����
/// </summary>
/// <param name="iIndex">�}�e���A���̃C���f�b�N�X</param>
/// <returns>����</returns>
D3DXVECTOR4 D3DXPARSER::GetAmbient(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Ambient;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}

/// <summary>
/// �g�U���ˌ�
/// </summary>
/// <param name="iIndex">�}�e���A���̃C���f�b�N�X</param>
/// <returns></returns>
D3DXVECTOR4 D3DXPARSER::GetDiffuse(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Diffuse;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}

/// <summary>
/// ���ʔ��ˌ�
/// </summary>
/// <param name="iIndex">�}�e���A���̃C���f�b�N�X</param>
/// <returns>���ʔ��ˌ�</returns>
D3DXVECTOR4 D3DXPARSER::GetSpecular(int iIndex)
{
	D3DXCOLOR color;

	color = m_pContainer->pMaterials[iIndex].MatD3D.Specular;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}


/// <summary>
/// �}�e���A���̃e�N�X�`�������擾
/// </summary>
/// <param name="index">�}�e���A���̃C���f�b�N�X</param>
/// <returns>�e�N�X�`���t�@�C����</returns>
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
//���̃|���S�����A�ǂ̃}�e���A���ł��邩��Ԃ� 
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
		MessageBoxA(0, "�����o�b�t�@�̃��b�N���s", "", MB_OK);
	}
	return MaterialIndex;
}
//
//
//iFaceIndex�Ԗڂ̃|���S�����`������3���_�̒��ŁAiIndexInFace�Ԗ�[0,2]�̒��_�̃C���f�b�N�X��Ԃ�
int D3DXPARSER::GetFaceVertexIndex(int iFaceIndex, int iIndexInFace)
{
	//�C���f�b�N�X�o�b�t�@�[�𒲂ׂ�Ε�����
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
//���̊֐�����Ԃ��Ƃ�Update����Ɩ��ʂȂ̂ŁARender�֐����s����Update������̂Ƃ��Ă��̂܂ܓn���B
//�e�N���X�����̃N���X��UpdateMatrices�����s���Ȃ��ꍇ�́AUpdateMatrices���Ȃ��ƍs�񂪍ŐV�ɂȂ�Ȃ��̂ŗ��ӁB
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
/// �R���X�g���N�^
/// </summary>
SkinMesh::SkinMesh()
{
	///	ZeroMemory(this, sizeof(SkinMesh));
}


/// <summary>
/// �f�X�g���N�^
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
/// //������
/// </summary>
/// <returns>���s����</returns>
void SkinMesh::Initialize()
{
	ID3D11Device* device = Devices::Get().Device().Get();
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();


	//D3D11�֘A�̏�����
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(MakeShader("Resources/HLSL/Geometry_Material_Texture_Skin.hlsl", "VSSkin", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)));
		//return E_FAIL;

	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(device->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf())))
		//return FALSE;
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(vertexLayout.Get());

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(MakeShader("Resources/HLSL/Geometry_Material_Texture_Skin.hlsl", "PSSkin", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)));
		//return E_FAIL;


	D3D11_BUFFER_DESC cb;
	//�R���X�^���g�o�b�t�@�[0�쐬
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
	//�R���X�^���g�o�b�t�@�[1�쐬  
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
	//�R���X�^���g�o�b�t�@�[�{�[���p�@�쐬  
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
	//�e�N�X�`���[�p�T���v���[�쐬
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
//X����X�L���֘A�̏���ǂݏo���@
HRESULT SkinMesh::ReadSkinInfo(SkinVertex* pvVB)
{
	//X���璊�o���ׂ����́A���_���Ƃ̃{�[���C���f�b�N�X�A���_���Ƃ̃{�[���E�F�C�g�A�o�C���h�s��A�|�[�Y�s��@��4����

	int i, k, m, n;
	int iNumVertex = d3dxMesh->GetNumVertices();//���_��
	int iNumBone = 0;//�{�[����


	//�{�[���̌��𓾂�
	iNumBone = d3dxMesh->GetNumBones();
	//���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�@��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���
	for (i = 0; i<iNumBone; i++)
	{
		//���̃{�[���ɉe�����󂯂钸�_��
		int iNumIndex = d3dxMesh->GetNumBoneVertices(i);
		int* piIndex = new int[iNumIndex];
		for (k = 0; k<iNumIndex; k++) piIndex[k] = d3dxMesh->GetBoneVerticesIndices(i, k);
		double* pdWeight = new double[iNumIndex];
		for (k = 0; k<iNumIndex; k++) pdWeight[k] = d3dxMesh->GetBoneVerticesWeights(i, k);
		//���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������
		for (k = 0; k<iNumIndex; k++)
		{
			//X��CG�\�t�g���{�[��4�{�ȓ��Ƃ͌���Ȃ��B5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi�� 

			//�E�F�C�g�̑傫�����Ƀ\�[�g�i�o�u���\�[�g�j
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
			//�\�[�g��́A�Ō�̗v�f�Ɉ�ԏ������E�F�C�g�������Ă���͂��B
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
	//�{�[���𐶐�
	numBone = iNumBone;
	boneArray = new Bone[iNumBone];
	//�|�[�Y�s��𓾂� �����|�[�Y
	for (i = 0; i<numBone; i++)
	{
		boneArray[i].bindPose = d3dxMesh->GetBindPose(i);
	}

	return S_OK;
}
//
//HRESULT SkinMesh::CreateFromX(CHAR* szFileName)
//X����X�L�����b�V�����쐬����@�@���Ӂj�f�ށiX)�̂ق��́A�O�p�|���S���ɂ��邱��
HRESULT SkinMesh::CreateFromX(CHAR* szFileName)
{
	ID3D11Device* device = Devices::Get().Device().Get();
	
	//X�t�@�C���ǂݍ���
	d3dxMesh = new D3DXPARSER;
	d3dxMesh->LoadMeshFromX(Devices::Get().Device9(), szFileName);

	//���O�ɒ��_���A�|���S�������𒲂ׂ�
	//���_��
	numVert = d3dxMesh->GetNumVertices();
	//�|���S����
	numFace = d3dxMesh->GetNumFaces();
	//Uv��
	numUV = d3dxMesh->GetNumUVs();

	
	//Direct3D�ł�UV�̐��������_���K�v
	if (numVert<numUV)
	{
		//���L���_���ŁA���_��������Ȃ���
		MessageBox(0, L"Direct3D�́AUV�̐��������_���K�v�ł��iUV��u���ꏊ���K�v�ł��j�e�N�X�`���[�͐������\���Ȃ��Ǝv���܂�", nullptr, MB_OK);
		return E_FAIL;
	}


	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	SkinVertex* pvVB = new SkinVertex[numVert];
	//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
	int* piFaceBuffer = new int[numFace * 3];


	D3DXVECTOR3 verPos[3];
	D3DXVECTOR2* verUv = new D3DXVECTOR2[3];

	Triangle tri;
	//���_�ǂݍ���											
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
	//�|���S�����i���_�C���f�b�N�X�j�ǂݍ���
	for (int i = 0; i<numFace * 3; i++)
	{
		piFaceBuffer[i] = d3dxMesh->GetIndex(i);
	}
	//�@���ǂݍ���
	for (int i = 0; i<numVert; i++)
	{
		D3DXVECTOR3 v = d3dxMesh->GetNormal(i);
		pvVB[i].norm.x = v.x;
		pvVB[i].norm.y = v.y;
		pvVB[i].norm.z = v.z;
	}

	//�}�e���A���ǂݍ���
	//�}�e���A����
	numMaterial = d3dxMesh->GetNumMaterials();
	material = new SkinMaterial[numMaterial];

	//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
	pIndexBuffer = new ComPtr<ID3D11Buffer>[numMaterial];
	for (int i = 0; i<numMaterial; i++)
	{
		//����	
		material[i].Ka.x = d3dxMesh->GetAmbient(i).y;
		material[i].Ka.y = d3dxMesh->GetAmbient(i).z;
		material[i].Ka.z = d3dxMesh->GetAmbient(i).w;
		material[i].Ka.w = d3dxMesh->GetAmbient(i).x;
		//�g�U���ˌ�	
		material[i].Kd.x = d3dxMesh->GetDiffuse(i).y;
		material[i].Kd.y = d3dxMesh->GetDiffuse(i).z;
		material[i].Kd.z = d3dxMesh->GetDiffuse(i).w;
		material[i].Kd.w = d3dxMesh->GetDiffuse(i).x;
		//���ʔ��ˌ�
		material[i].Ks.x = d3dxMesh->GetSpecular(i).y;
		material[i].Ks.y = d3dxMesh->GetSpecular(i).z;
		material[i].Ks.z = d3dxMesh->GetSpecular(i).w;
		material[i].Ks.w = d3dxMesh->GetSpecular(i).x;

		//�e�N�X�`���[�i�f�B�t���[�Y�e�N�X�`���[�̂݁j
		char* name = d3dxMesh->GetTexturePath(i);
		if (name)
		{
			strcpy(material[i].textureName, name);
		}
		//�ǂݍ��񂾃e�N�X�`���[���쐬
		if (material[i].textureName[0] != 0 && FAILED(D3DX11CreateShaderResourceViewFromFileA(device, material[i].textureName, nullptr, nullptr, material[i].texture.ReleaseAndGetAddressOf(), nullptr)))//��΃p�X�t�@�C�����́A�܂����s����Ǝv�����A�A�A
		{
			MessageBox(0, L"�e�N�X�`���[�ǂݍ��ݎ��s", nullptr, MB_OK);
			return E_FAIL;
		}

		//���̃}�e���A���ł���C���f�b�N�X�z����̊J�n�C���f�b�N�X�𒲂ׂ�@����ɃC���f�b�N�X�̌������ׂ�
		int iCount = 0;
		//���b�V�����̃|���S�����Ń������m��
		int* pIndex = new int[numFace * 3];

		for (int k = 0; k<numFace; k++)
		{
			if (i == d3dxMesh->GetFaceMaterialIndex(k))//���� i == k�Ԗڂ̃|���S���̃}�e���A���ԍ� �Ȃ�
			{
				pIndex[iCount] = d3dxMesh->GetFaceVertexIndex(k, 0);//k�Ԗڂ̃|���S������钸�_�̃C���f�b�N�X�@1��
				pIndex[iCount + 1] = d3dxMesh->GetFaceVertexIndex(k, 1);//2��
				pIndex[iCount + 2] = d3dxMesh->GetFaceVertexIndex(k, 2);//3��
				iCount += 3;

			}
		}

		//�C���f�b�N�X�o�b�t�@�쐬
		if (iCount>0) CreateIndexBuffer(iCount * sizeof(int), pIndex, pIndexBuffer[i].ReleaseAndGetAddressOf());
		delete pIndex;
		material[i].numFace = iCount / 3;//���̃}�e���A�����̃|���S����

	}
	//�X�L�����i�W���C���g�A�E�F�C�g�@�j�ǂݍ���
	ReadSkinInfo(pvVB);
	//�o�[�e�b�N�X�o�b�t�@�[���쐬
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

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete piFaceBuffer;
	if (pvVB) delete pvVB;
	/*if (verPos) delete verPos;
	if (verUv) delete verUv;*/

	return S_OK;
}
//
//HRESULT SkinMesh::CreateIndexBuffer(DWORD dwSize,int* pIndex,ID3D11Buffer** ppIndexBuffer)
//Direct3D�̃C���f�b�N�X�o�b�t�@�[�쐬
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
//�{�[�������̃|�[�Y�ʒu�ɃZ�b�g����
void SkinMesh::SetNewPoseMatrices(int iFrame)
{
	//�]�ރt���[����Update���邱�ƁB���Ȃ��ƍs�񂪍X�V����Ȃ�
	//d3dxMesh->UpdateFrameMatrices(d3dxMesh->m_pFrameRoot)�������_�����O���Ɏ��s���邱��

	//�܂��A�A�j���[�V�������ԂɌ��������s����X�V����̂�D3DXMESH�ł�
	//�A�j���[�V�����R���g���[���[���i���Łj����Ă������̂Ȃ̂ŁA�A�j���[�V�����R���g���[���[
	//���g���ăA�j����i�s�����邱�Ƃ��K�v
	//	d3dxMesh->m_pAnimController->AdvanceTime(....)�������_�����O���Ɏ��s���邱��

	for (int i = 0; i<numBone; i++)
	{
		boneArray[i].newPose = d3dxMesh->GetNewPose(i);
	}
}
//
//D3DXMATRIX SkinMesh::GetCurrentPoseMatrix(int index)
//���́i���݂́j�|�[�Y�s���Ԃ�
D3DXMATRIX SkinMesh::GetCurrentPoseMatrix(int index)
{
	D3DXMATRIX ret = boneArray[index].bindPose*boneArray[index].newPose;
	return ret;
}
//
//D3DXMATRIX SkinMesh::GetBindPoseMatrix(int index)
//�o�C���h�|�[�Y�s���Ԃ�
D3DXMATRIX SkinMesh::GetBindPoseMatrix(int index)
{
	return boneArray[index].bindPose;
}

/// <summary>
/// �{�[���̐����擾
/// </summary>
/// <param name="iBoneIndex">�{�[���C���f�b�N�X</param>
/// <returns>�����̃C���f�b�N�X�̖��O���擾</returns>
CHAR* SkinMesh::GetBoneNames(int iBoneIndex)
{
	return d3dxMesh->GetBoneName(iBoneIndex);
}

void SkinMesh::UpDateTriangles()
{
	//���_��
	numVert = d3dxMesh->GetNumVertices();

	//�|���S����
	numFace = d3dxMesh->GetNumFaces();
	//Uv��
	numUV = d3dxMesh->GetNumUVs();


	//Direct3D�ł�UV�̐��������_���K�v
	if (numVert<numUV)
	{
		//���L���_���ŁA���_��������Ȃ���
		MessageBox(0, L"Direct3D�́AUV�̐��������_���K�v�ł��iUV��u���ꏊ���K�v�ł��j�e�N�X�`���[�͐������\���Ȃ��Ǝv���܂�", nullptr, MB_OK);
	//	return E_FAIL;
	}


	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	SkinVertex* pvVB = new SkinVertex[numVert];
	//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
	int* piFaceBuffer = new int[numFace * 3];


	D3DXVECTOR3 verPos[3];
	D3DXVECTOR2* verUv = new D3DXVECTOR2[3];

	Triangle tri;
	//���_�ǂݍ���											
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
/// �`��
/// </summary>
void SkinMesh::Render()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	D3D11_MAPPED_SUBRESOURCE pData;
	FollowCamera* camera = FollowCamera::GetInstance();
	D3DXVECTOR3 eye =camera->GetEyePos();

	//�g�p����V�F�[�_�[�̃Z�b�g
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);



	//�A�j���[�V�����t���[����i�߂�@�X�L�����X�V
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

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g�i�o�[�e�b�N�X�o�b�t�@�[�͈�ł����j
	UINT stride = sizeof(SkinVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1,vertexBuffer.GetAddressOf(), &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(vertexLayout.Get());

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�J�����ʒu���V�F�[�_�[�ɓn��
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

	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i<numMaterial; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (material[i].numFace == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		deviceContext->IASetIndexBuffer(pIndexBuffer[i].Get(), DXGI_FORMAT_R32_UINT, 0);

		//�}�e���A���̊e�v�f�ƕϊ��s����V�F�[�_�[�ɓn��			
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
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
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
	//�A�j���i�s
	if (d3dxMesh->m_pAnimController)
	{
		d3dxMesh->m_pAnimController->AdvanceTime(0.001666, nullptr);
	}
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	d3dxMesh->UpdateFrameMatrices(d3dxMesh->m_pFrameRoot, &m);
}