#pragma once
#include <memory>
#include "Device.h"
#include "ShaderManager.h"
#include "FollowCamera.h"
#include "AStar/Math.h"
#include "MatrixObject.h"
#include "ShaderManager.h"


using namespace Microsoft::WRL;

#define MAX_BONES 255

//�}�N��
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }


//�V�F�[�_�[�ɓn���l
struct LightAndEyeGlobal
{
	D3DXVECTOR4 vLightDir;//���C�g����
	D3DXVECTOR4 vEye;//�J�����ʒu
};

struct SkinMeshGlobal
{
	D3DXMATRIX mW;//���[���h�s��
	D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXVECTOR4 vAmbient;//�A���r�G���g��
	D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
	D3DXVECTOR4 vSpecular;//���ʔ���
};

struct BoneGlobal
{
	D3DXMATRIX boneMatrix[MAX_BONES];
	BoneGlobal()
	{
		for (int i = 0; i<MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&boneMatrix[i]);
		}
	}
};

//�I���W�i���@�}�e���A���\����
struct SkinMaterial
{
	CHAR szName[110];
	D3DXVECTOR4 Ka;//����
	D3DXVECTOR4 Kd;//�g�U���ˌ�
	D3DXVECTOR4 Ks;//���ʔ��ˌ�
	CHAR textureName[512];//�e�N�X�`���[�t�@�C����
	ComPtr<ID3D11ShaderResourceView> texture;
	DWORD numFace;//���̃}�e���A���ł���|���S����
	SkinMaterial()
	{
		ZeroMemory(this, sizeof(SkinMaterial));
	}
	~SkinMaterial()
	{
		texture.Reset();
	}
};

//���_�\����
struct SkinVertex
{
	D3DXVECTOR3 pos;//���_�ʒu
	D3DXVECTOR3 norm;//���_�@��
	D3DXVECTOR2 uv;//UV���W
	UINT boneIndex[4];//�{�[���@�ԍ�
	float boneWeight[4];//�{�[���@�d��
	SkinVertex()
	{
		ZeroMemory(this, sizeof(SkinVertex));
	}
};
//�{�[���\����
struct Bone
{
	D3DXMATRIX bindPose;//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX newPose;//���݂̃|�[�Y�i���̓s�x�ς��j
	DWORD numChild;//�q�̐�
	int childIndex[50];//�����̎q�́h�C���f�b�N�X" 50�܂�
	CHAR name[256];

	Bone()
	{
		ZeroMemory(this, sizeof(Bone));
		D3DXMatrixIdentity(&bindPose);
		D3DXMatrixIdentity(&newPose);
	}
};


//�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct DerivedFrame : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};


//�h�����b�V���R���e�i�[�\����(
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD weight;//�d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD boneNum;//�{�[���̐�
	LPD3DXBUFFER boneBuffer;//�{�[���E�e�[�u��
	D3DXMATRIX** ppBoneMatrix;//�S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
	D3DXMATRIX* pBoneOffsetMatrices;//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[	
};


//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
	CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

//
//
//
class D3DXPARSER
{
public:

	MY_HIERARCHY cHierarchy;
	MY_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;
	MYMESHCONTAINER* m_pContainer;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	HRESULT LoadMeshFromX(LPDIRECT3DDEVICE9, LPSTR);
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER);
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME);
	void UpdateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);
	//
	int GetNumVertices();
	int GetNumFaces();
	int GetNumMaterials();
	int GetNumUVs();
	int GetNumBones();
	int GetNumBoneVertices(int iBoneIndex);
	DWORD GetBoneVerticesIndices(int iBoneIndex, int iIndexInGroup);
	double GetBoneVerticesWeights(int iBoneIndex, int iIndexInGroup);
	D3DXVECTOR3 GetVertexCoord(DWORD iIndex);
	D3DXVECTOR3 GetNormal(DWORD iIndex);
	D3DXVECTOR2 GetUV(DWORD iIndex);
	int GetIndex(DWORD iIndex);
	D3DXVECTOR4 GetAmbient(int iIndex);
	D3DXVECTOR4 GetDiffuse(int iIndex);
	D3DXVECTOR4 GetSpecular(int iIndex);
	CHAR* GetTexturePath(int index);
	float GetSpecularPower(int iIndex);
	int GetFaceMaterialIndex(int iFaceIndex);
	int GetFaceVertexIndex(int iFaceIndex, int iIndexInFace);
	D3DXMATRIX GetBindPose(int iBoneIndex);
	D3DXMATRIX GetNewPose(int iBoneIndex);
	CHAR* GetBoneName(int iBoneIndex);
};

//
//class SkinMesh
//SkinMesh �I���W�i�����b�V���N���X
class SkinMesh
{
public:
	//Dx11
	ComPtr<ID3D11SamplerState> sampleLinear;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> vertexLayout;
	ComPtr<ID3D11Buffer> lightAndEyeBuffer;
	ComPtr<ID3D11Buffer> skinMeshBuffer;
	ComPtr<ID3D11Buffer> boneBuffer;

	//���b�V��
	D3DXPARSER* d3dxMesh;
	DWORD numVert;
	DWORD numFace;
	DWORD numUV;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer>* pIndexBuffer;
	SkinMaterial* material;
	std::vector<Triangle> triangles;

	DWORD numMaterial;

	std::unique_ptr<MatrixObject> matrixObject;

	//�{�[��
	int numBone;
	Bone* boneArray;

	//���\�b�h
	SkinMesh();
	~SkinMesh();
	virtual HRESULT Initialize();
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);
	virtual void Render();
	HRESULT CreateFromX(CHAR* szFileName);
	HRESULT ReadSkinInfo(SkinVertex*);
	void SetNewPoseMatrices(int frame);
	D3DXMATRIX GetBindPoseMatrix(int index);
	D3DXMATRIX GetCurrentPoseMatrix(int index);
	CHAR* GetBoneNames(int iBoneIndex);
	void UpDateTriangles();

};