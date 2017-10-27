#pragma once
#include "ShaderManager.h"
#include "FollowCamera.h"

#include<d3dx9.h>
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

class OrenNayer
{
	//�\���̒�`
	struct SIMPLECONSTANT_BUFFER0
	{
		D3DXMATRIX mW;//���[���h�s��
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
		D3DXVECTOR4 vLightDir;//���C�g����
		D3DXVECTOR4 vEye;//�J�����ʒu
	};

	struct SIMPLECONSTANT_BUFFER1
	{
		D3DXVECTOR4 vAmbient;//�A���r�G���g��
		D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
		D3DXVECTOR4 vSpecular;//���ʔ���
	};

	//�I���W�i���@�}�e���A���\����
	struct MY_MATERIAL
	{
		CHAR szName[110];
		D3DXVECTOR4 Ambient;//�A���r�G���g
		D3DXVECTOR4 Diffuse;//�f�B�t���[�Y
		D3DXVECTOR4 Specular;//�X�y�L�����[
		CHAR szTextureName[110];//�e�N�X�`���[�t�@�C����
		ID3D11ShaderResourceView* pTexture;
		DWORD dwNumFace;//���̃}�e���A���ł���|���S����
		MY_MATERIAL()
		{
			ZeroMemory(this, sizeof(MY_MATERIAL));
		}
		~MY_MATERIAL()
		{
			SAFE_RELEASE(pTexture);
		}
	};
	//���_�̍\����
	struct MY_VERTEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNorm;
		D3DXVECTOR2 vTex;
	};

public:
	//Data
	HWND m_hWnd;
	DWORD m_dwNumMaterial;
	LPD3DXMESH m_pMesh;
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
	//Dx11
	//ID3D11Device* m_pDevice11;
	//ID3D11DeviceContext *m_pDeviceContext11;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	//resource
	char m_TextureFileName[8][256];//�e�N�X�`���[�t�@�C�����i�W���܂Łj
	ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	MY_MATERIAL* m_pMaterial;
	//Method
	OrenNayer();
	~OrenNayer();
	HRESULT Init(LPSTR);
	HRESULT InitDx9();
	HRESULT LoadXMesh(LPSTR FileName);
	HRESULT InitShader();
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);
};

