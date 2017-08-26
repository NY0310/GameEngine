#pragma once
#include "ShaderManager.h"
#include "FollowCamera.h"

#include<d3dx9.h>
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

////X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����N���X�B
//class MY_H_HIERARCHY : public ID3DXAllocateHierarchy
//{
//public:
//	MY_H_HIERARCHY() {}
//	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
//	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
//	CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
//	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
//	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
//};
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
	//���_�̍\����
	struct MY_VERTEX_TEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNorm;
		D3DXVECTOR2 vTex;
	};

	////���_�̍\����
	//struct MY_VERTEX_NOTEX
	//{
	//	D3DXVECTOR3 vPos;
	//	D3DXVECTOR3 vNorm;
	//};



	////�I���W�i�����b�V��
	//struct MY_MESH
	//{
	//	ID3D11Buffer* pVertexBuffer;
	//	ID3D11Buffer* pIndexBuffer;
	//};



	////�p�[�c���b�V���@�A�v����`���b�V��
	////(�X�^�e�B�b�N���b�V���̃T���v���ɂ�����CD3DXMESH�N���X�Ɠ���)
	//struct PARTS_MESH
	//{
	//	DWORD dwNumMaterial;
	//	ID3D11Buffer* pVertexBuffer;
	//	ID3D11Buffer* ppIndexBuffer;
	//	char TextureFileName[8][256];//�e�N�X�`���t�@�C����(8���܂�)
	//	MY_MATERIAL* pMaterial;
	//	bool Tex;
	//};
public:
	CD3DXMESH();
	~CD3DXMESH();
	HRESULT Init(LPSTR);
	HRESULT InitDx9();
	HRESULT LoadXMesh(LPSTR FileName);
	HRESULT InitShader();//�V�F�[�_������

	void CD3DXMESH::Render(unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
		//�`��
private:
	//Date
	DWORD m_dwNumMaterial;
	LPD3DXMESH m_pMesh;
	ShaderManager shadermanager;
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll;
	float m_fScale;
	//Dx9
	LPDIRECT3D9 m_pD3d9;
	LPDIRECT3DDEVICE9 m_pDevice9;
	D3DMATERIAL9* pMeshMaterials;
	DWORD m_NumAttr;
	DWORD m_AttrID[300];//300�����܂�
	bool m_Tex;
	//LPDIRECT3DTEXTURE9* pMeshTextures;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pConstantBuffer2;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	//�����f������
	char m_TextureFileName[8][256];//�e�N�X�`���[�t�@�C�����i�W���܂Łj
	ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[
	MY_MATERIAL* m_pMaterial;
	bool m_Texture;//�e�N�X�`���f�[�^�����݂��邩
	int birthcnt;//��������Ă���̃t���[����



};

