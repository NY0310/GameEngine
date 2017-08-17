/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"



class OBJ
{
public:
	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
		D3DXVECTOR3 Normal;//�@��
		D3DXVECTOR2 Tex; //�e�N�X�`���[���W
	};

	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		//D3DXMATRIX mW;//���[���h�s��
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
		//D3DXVECTOR4 vLightDir;//���C�g����
		//D3DXVECTOR4 vColor;//�f�B�q���[�Y�F
		//D3DXVECTOR4 vSpecular;//���ʔ��ˌ�
		//D3DXVECTOR4 vEyes;//�J�����ʒu
	};


	//�I���W�i�����b�V��
	struct MY_MESH
	{
		DWORD dwNumVert;
		DWORD dwNumFace;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};
	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
private:
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11Buffer* m_pConstantBuffer;
	MY_MESH m_Mesh;
	ID3D11InputLayout* m_pVertexLayout;

	ShaderManager shadermanager;//�V�F�[�_�[�֘A


	D3DXVECTOR3 m_vLight;

	ID3D11SamplerState* m_pSampleLimear;//�e�N�X�`���T���v��
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`��
	int birthcnt;

};

