#pragma once
#include<d3dx9.h>
#include <vector>

#include "FollowCamera.h"
#include "ShaderManager.h"
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

	//�I���W�i���@�}�e���A���\����
	struct MY_MATERIAL
	{
		CHAR szName[100];
		D3DXVECTOR4 Ambient;//�A���r�G���g
		D3DXVECTOR4 Diffuse;//�f�B�q���[�Y
		D3DXVECTOR4 Specular;//�X�y�L�����[
		CHAR szTextureName[256];//�e�N�X�`���t�@�C����
		ID3D11ShaderResourceView* pTexture;
		DWORD dwNumFace;//���̃}�e���A���ł���|���S����

	};

	//���b�V���R���e�i�[�\���� �t���[�����Ŏg�p
	/// <summary>
	/// Name
	/// ���b�V�����B
	/// MeshData
	/// ���b�V�����̃f�[�^�̃^�C�v�B
	/// pMaterials
	/// ���b�V���ޗ��̔z��B
	/// pEffects
	/// �f�t�H���g�G�t�F�N�g�p�����[�^�̃Z�b�g�ւ̃|�C���^�B
	/// NumMaterials
	/// ���b�V�����̃}�e���A���̐��B
	/// pAdjacency
	/// �אڏ����܂ރ��b�V���̎O�p�`������3��DWORD�̔z��ւ̃|�C���^�B
	/// pSkinInfo
	/// �X�L�����C���^�[�t�F�C�X�ւ̃|�C���^�B
	/// pNextMeshContainer
	/// ���̃��b�V���R���e�i�ւ̃|�C���^�B
	/// </summary>
	struct MYHMESHCONTAINER : public D3DXMESHCONTAINER
	{
		//�A�v���P�[�V��������e�N�X�`���[ ���\�[�X�𑀍�
		LPDIRECT3DTEXTURE9*  ppTextures;
	};

	//�p�[�c���b�V���@�A�v����`���b�V��
	//(�X�^�e�B�b�N���b�V���̃T���v���ɂ�����CD3DXMESH�N���X�Ɠ���)
	struct PARTS_MESH
	{
		DWORD dwNumMaterial;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer** ppIndexBuffer;
		char TextureFileName[8][256];//�e�N�X�`���t�@�C����(8���܂�)
		MY_MATERIAL* pMaterial;
		bool Tex;
	};

	//�h���t���[���\����
	struct MYHFRAME : public D3DXFRAME
	{
		D3DXMATRIXA16 CombinedTransformationMatrix;
		PARTS_MESH* pPartsMesh;
		MYHFRAME()
		{
			ZeroMemory(this, sizeof(MYHFRAME));
		}
	};

//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����N���X�B
/// <summary>
/// ID3DXAllocateHierarchy
/// CreateFrame	�t���[�� �I�u�W�F�N�g�̊��蓖�Ă�v������B
/// CreateMeshContainer	���b�V�� �R���e�i �I�u�W�F�N�g�̊��蓖�Ă�v������B
///	DestroyFrame	�t���[�� �I�u�W�F�N�g�̊��蓖�ĉ�����v������B
///DestroyMeshContainer	���b�V�� �R���e�i �I�u�W�F�N�g�̊��蓖�ĉ�����v������B
/// </summary>
class MY_H_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_H_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
	CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};


class CD3DXMESH
{
public:

	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLECONSTANT_BUFFER0
	{
		D3DXMATRIX mW;//���[���h�s��
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
		D3DXVECTOR4 vLightDir;//���C�g����
		D3DXVECTOR4 vEyes;//�J�����ʒu

	};
	struct SIMPLECONSTANT_BUFFER1
	{
		D3DXVECTOR4 vAmbient;//�A���r�G���g��
		D3DXVECTOR4 vDiffuse;//�f�B�q���[�Y�F
		D3DXVECTOR4 vSpecular;//���ʔ���
	};


	//���_�̍\����
	struct MY_VERTEX_TEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNorm;
		D3DXVECTOR2 vTex;
	};

	//���_�̍\����
	struct MY_VERTEX_NOTEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNorm;
	};



	//�I���W�i�����b�V��
	struct MY_MESH
	{
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};



public:
	CD3DXMESH();
	~CD3DXMESH();
	HRESULT Init(LPSTR);
	HRESULT InitDx9();
	HRESULT InitShader();//�V�F�[�_������

	void Render(unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
		//�`��
	HRESULT LoadXAnimMesh(LPSTR FileName);
	void BuildAllMesh(D3DXFRAME* p);
	HRESULT CreateAppMeshFromD3DXMesh(LPD3DXFRAME p);
	void DrawFrame(LPD3DXFRAME p, unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
	void DrawPartsMesh(PARTS_MESH* p, D3DXMATRIX World, unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
	void UpdateHierarchyMatrices(D3DXFRAME* p, LPD3DXMATRIX pParentMatrix);
	inline LPD3DXANIMATIONCONTROLLER GetAnimController() {
		return m_pAnimController;
	}
	float&  GetfYaw() { return m_fYaw; }


	void ChangeAnimSet(int index);

private:
	//Date
	//DWORD m_dwNumMaterial;
	LPD3DXMESH m_pMesh;
	ShaderManager shadermanager;
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll;
	float m_fScale;
	//�A�j���֘A
	MY_H_HIERARCHY* m_pHierarchy;
	/// <summary>
	/// �t���[���̖��O�B
		/*TransformationMatrix
		�g�����X�t�H�[���s��B
		pMeshContainer
		���b�V�� �R���e�i�ւ̃|�C���^�B
		pFrameSibling
		�Z��t���[���ւ̃|�C���^�B
		pFrameFirstChild
		�q�t���[���ւ̃|�C���^�B*/
	/// </summary>
	LPD3DXFRAME m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	//Dx9
	LPDIRECT3D9 m_pD3d9;
	LPDIRECT3DDEVICE9 m_pDevice9;
	D3DMATERIAL9* pMeshMaterials;
	//DWORD m_NumAttr;
	DWORD m_AttrID[300];//300�����܂�
	bool m_Tex;
	//LPDIRECT3DTEXTURE9* pMeshTextures;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11InputLayout* m_pVertexLayout_NoTex;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11VertexShader* m_pVertexShaderNoTex;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11PixelShader* m_pPixelShaderNoTex;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pVertexBuffer0;
	ID3D11Buffer** m_ppIndexBuffer;
	//�����f������
	//char m_TextureFileName[8][256];//�e�N�X�`���[�t�@�C�����i�W���܂Łj
	ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	//ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[
	//MY_MATERIAL* m_pMaterial;
	//bool m_Texture;//�e�N�X�`���f�[�^�����݂��邩
	int birthcnt;//��������Ă���̃t���[����

	vector<LPD3DXANIMATIONSET> m_pAnimSet;//�A�j���[�V�����Z�b�g�ɑΉ�


};

