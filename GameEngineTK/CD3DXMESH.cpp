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

	//�t�@�C�����𐶐�
	//lstrlenA�͕�����̒�����Ԃ�
	pFrame->Name = new CHAR[lstrlenA(Name) + 1];
	//�t�@�C������NULL�Ȃ玸�s
	if (!pFrame->Name)
	{
		return E_FAIL;
	}

	//�����Ŏ󂯎�����t�@�C�������R�s�[
	strcpy(pFrame->Name, Name);

	//�P�ʍs��쐬
	//���W
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	pFrame->pMeshContainer = nullptr;
	pFrame->pFrameSibling = nullptr;
	pFrame->pFrameFirstChild = nullptr;
	*ppNewFrame = pFrame;

	return S_OK;
}


//HRESULT MY_HIERARCHY::CreateMeshContainer
//���b�V���R���e�i�[���쐬����
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

	// ���Y���b�V�����@���������Ȃ��ꍇ�͖@����ǉ�����
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
/// �t���[����j������
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
///���b�V�� �R���e�i �I�u�W�F�N�g�̊��蓖�ĉ���������B
/// </summary>
HRESULT MY_H_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYHMESHCONTAINER *pMeshContainer = (MYHMESHCONTAINER*)pMeshContainerBase;

	//�X�L�����̃C���^�[�t�F�C�X������������
	if (pMeshContainer->pSkinInfo)
	{
		pMeshContainer->pSkinInfo->Release();
		pMeshContainer->pSkinInfo = 0;
	}

	//���b�V����������������
	if (pMeshContainer->Name)
	{
		delete[] pMeshContainer->pSkinInfo;
		pMeshContainer->pSkinInfo = nullptr;
	}

	// �אڏ����܂ރ��b�V���̎O�p�`������3��DWORD�̔z��ւ̃|�C���^�B
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
/// �R���X�g���N�^
/// </summary>
CD3DXMESH::CD3DXMESH()
{
	ZeroMemory(this, sizeof(CD3DXMESH));
	//birthcnt = 0;
	m_fScale = 10.0f;
	//m_Texture = false;
}

/// <summary>
/// �f�X�g���N�^
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

HRESULT CD3DXMESH::LoadXAnimMesh(LPSTR FileName)
{
	auto& devices = Devices::Get();

	m_pHierarchy = new MY_H_HIERARCHY;
	if (FAILED(D3DXLoadMeshHierarchyFromXA(FileName, D3DXMESH_MANAGED, m_pDevice9,
		m_pHierarchy, nullptr, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(nullptr, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", nullptr, MB_OK);
		return E_FAIL;
	}


	//���̎��_�ŁA�t�@�C��������o�����S�t���[����m_pFrameRoot�ɓ����Ă���A
	//�܂��A�j���[�V�������R���g���[������ɂ�m_pAnimController������������Ă���͂��Ȃ̂ŁA������g���B

	//���Ƃ́A��������K�v�ȏ����Ƃ肾���A�����t���[�����ƂɃA�v���Ǝ��̃��b�V�����\�z���Ă���
	BuildAllMesh(m_pFrameRoot);


	//�A�j���[�V�����g���b�N�𓾂� �@�{�T���v���Y�t��X�t�@�C���̏ꍇ��2�Z�b�g��������100�܂łɑΉ��ł���
	for (DWORD i = 0; i<m_pAnimController->GetNumAnimationSets(); i++)
	{
		LPD3DXANIMATIONSET animSet;
		m_pAnimController->GetAnimationSet(i, &animSet);
		m_pAnimSet.emplace_back(animSet);
	}

	return S_OK;


}

/// <summary>
/// D3DXMESH����A�v�����b�V�����쐬����
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

	//���̎��_�ŁA�t�@�C��������o�������b�V���f�[�^���ADx9��D3DX���b�V���ɓ����Ă���A
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//�h���t���[���\����
	MYHFRAME* pFrame = (MYHFRAME*)p;

	//D3DX���b�V���i��������E�E�E�j
	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;
	//�A�v�����b�V���i�E�E�E�����Ƀ��b�V���f�[�^���R�s�[����j
	PARTS_MESH* pAppMesh = new PARTS_MESH;
	pAppMesh->Tex = false;

	////���Ƃ́A��������D���ȏ������o����Dx11�̎��O���b�V���ɗ��p���邾���B
	pAppMesh->dwNumMaterial = pFrame->pMeshContainer->NumMaterials;
	pAppMesh->pMaterial = new MY_MATERIAL[pAppMesh->dwNumMaterial];
	pAppMesh->ppIndexBuffer = new ID3D11Buffer*[pAppMesh->dwNumMaterial];

	for (DWORD i = 0; i<pAppMesh->dwNumMaterial; i++)
	{
		//�A���r�G���g
		pAppMesh->pMaterial[i].Ambient.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.r;
		pAppMesh->pMaterial[i].Ambient.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.g;
		pAppMesh->pMaterial[i].Ambient.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.b;
		pAppMesh->pMaterial[i].Ambient.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.a;
		//�f�B�t���[�Y
		pAppMesh->pMaterial[i].Diffuse.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.r;
		pAppMesh->pMaterial[i].Diffuse.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.g;
		pAppMesh->pMaterial[i].Diffuse.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.b;
		pAppMesh->pMaterial[i].Diffuse.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.a;
		//�X�y�L�����[
		pAppMesh->pMaterial[i].Specular.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.r;
		pAppMesh->pMaterial[i].Specular.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.g;
		pAppMesh->pMaterial[i].Specular.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.b;
		pAppMesh->pMaterial[i].Specular.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.a;
		//�e�N�X�`���[�������
		if (pFrame->pMeshContainer->pMaterials != nullptr &&
			lstrlenA(pFrame->pMeshContainer->pMaterials[i].pTextureFilename) > 0)
		{
			pAppMesh->Tex = true;
			strcpy(pAppMesh->pMaterial[i].szTextureName, pFrame->pMeshContainer->pMaterials[i].pTextureFilename);
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device.Device().Get(),
				pAppMesh->pMaterial[i].szTextureName, nullptr, nullptr, &pAppMesh->pMaterial[i].pTexture, nullptr)))
			{
				return E_FAIL;
			}
		}

	}

	//�C���f�b�N�X�o�b�t�@�[���쐬
	//����ɐ旧���A���b�V���̑������𓾂�B�������ŃC���f�b�N�X�o�b�t�@�[����ׂ����}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�𕪗��ł���

	D3DXATTRIBUTERANGE* pAttrTable = nullptr;
	DWORD NumAttr = 0;
	//pD3DXMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	pD3DXMesh->GetAttributeTable(pAttrTable, &NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[NumAttr];
	if (pD3DXMesh->GetAttributeTable(pAttrTable, &NumAttr))
	{
		MessageBoxA(0, "�����e�[�u���擾���s", "", MB_OK);
		return E_FAIL;
	}
	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�C���f�b�N�X�o�b�t�@�[������o���Ȃ��̂Ń��b�N����B

	//2�o�C�g�̃C���f�b�N�X�̏ꍇ�����肦��
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

	//�������Ƃ̃C���f�b�N�X�o�b�t�@���쐬
	for (DWORD i = 0; i<NumAttr; i++)
	{
		//m_AttrID[i] = pAttrTable[i].AttribId;
		//Dx9��D3DMESH�̃C���f�b�N�X�o�b�t�@�[����̏��ŁADx11�̃C���f�b�N�X�o�b�t�@���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DWORD) * pAttrTable[i].FaceCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];//�傫���C���f�b�N�X�o�b�t�@���̃I�t�Z�b�g(*3��Y�ꂸ�Ɂj
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


	//�o�[�e�b�N�X�o�b�t�@�[���쐬

	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�o�[�e�b�N�X�o�b�t�@�[������o���Ȃ��̂Ń��b�N����B
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	pD3DXMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = pD3DXMesh->GetNumBytesPerVertex();
	BYTE *pVertices = nullptr;
	MY_VERTEX_NOTEX* pvVertex = nullptr;
	MY_VERTEX_TEX* pvVertexTex = nullptr;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		//Dx9��D3DMESH�̃o�[�e�b�N�X�o�b�t�@�[����̏��ŁADx11�̃o�[�e�b�N�X�o�b�t�@���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//�e�N�X�`���[���W���}�C�i�X�΍�
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

	////�e�N�X�`���[�p�T���v���[�쐬
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
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�e�N�X�`���[�Ȃ��V�F�[�_�[
	SAFE_RELEASE(pErrors);
	SAFE_RELEASE(pCompiledShader);
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS_NoTex", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShaderNoTex)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//�e�N�X�`���[�Ȃ�
	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(layout2) / sizeof(layout2[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout2, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout_NoTex)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�e�N�X�`���[�Ȃ�
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS_NoTex", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShaderNoTex)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
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
	//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
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

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
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
	////���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
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



	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
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
		sg.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
		devices.Context().Get()->Unmap(m_pConstantBuffer0, 0);
	}
	//m_pSampleLinear = NULL;
	////�e�N�X�`���[���V�F�[�_�[�ɓn��
	//devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
	//devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	//UINT stride = m_pMesh->GetNumBytesPerVertex();


	UINT stride = 0;
	if (pPartsMesh->Tex)
	{
		stride = sizeof(MY_VERTEX_TEX);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
		//�g�p����V�F�[�_�[�̓o�^
		devices.Context().Get()->VSSetShader(m_pVertexShader, nullptr, 0);
		devices.Context().Get()->PSSetShader(m_pPixelShader, nullptr, 0);
	}
	else
	{
		stride = sizeof(MY_VERTEX_NOTEX);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		devices.Context()->IASetInputLayout(m_pVertexLayout_NoTex);
		//�g�p����V�F�[�_�[�̓o�^
		devices.Context()->VSSetShader(m_pVertexShaderNoTex, nullptr, 0);
		devices.Context()->PSSetShader(m_pPixelShaderNoTex, nullptr, 0);
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &pPartsMesh->pVertexBuffer, &stride, &offset);


	//�����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@�|��`��
	for (DWORD i = 0; i < pPartsMesh->dwNumMaterial; i++)
	{
		//�g�p����Ă��Ȃ��΍�
		if (pPartsMesh->pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		devices.Context().Get()->IASetIndexBuffer(pPartsMesh->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient = pPartsMesh->pMaterial[i].Ambient;//�A���r�G���g�����V�F�[�_�[�ɓn��
			sg.vDiffuse = pPartsMesh->pMaterial[i].Diffuse;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			sg.vSpecular = pPartsMesh->pMaterial[i].Specular;//�X�y�L�����[���V�F�[�_�[�ɓn��
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER1));
			devices.Context().Get()->Unmap(m_pConstantBuffer1, 0);
		}
		devices.Context().Get()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		devices.Context().Get()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);

		//�e�N�X�`���[���V�F�[�_�[�ɓn��
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
		//�v���~�e�B�u�������_�����O
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