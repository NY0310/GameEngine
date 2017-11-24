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
		D3DXMATRIX mW;//���[���h�s��
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
		D3DXMATRIX mWLP;//���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����̍���
		D3DXMATRIX mWLPT;//���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����E�e�N�X�`�����W�s��̍���
		D3DXVECTOR4 vLightDir;//���C�g����
		D3DXVECTOR4 vEyes;//�J�����ʒu
		D3DXVECTOR4 inkColor;//�C���N�̐F
		ALIGN16 D3DXVECTOR2 inkUv;//�C���N�e�N�X�`����UV���W
		ALIGN16 float inkScale;//�C���N��h��L���͈�
		//ALIGN16 bool ObjTexFlag;
		//D3DXVECTOR4 vDiffuse;//�f�B�q���[�Y�F
		//D3DXVECTOR4 vSpecular;//���ʔ��ˌ�
	};



	struct ZTEXTURE_CONSTANT_BUFFER
	{
		D3DXMATRIX mWLP;//���[���h���C�g�s��
	};

	//�I���W�i���@�}�e���A���\����
	struct MY_MATERIAL
	{
		CHAR szName[111];
		D3DXVECTOR4 Ka;//�A���r�G���g
		D3DXVECTOR4 Kd;//�f�B�q���[�Y
		D3DXVECTOR4 Ks;//�X�y�L�����[
		CHAR szTextureName[110];//�e�N�X�`���[�t�@�C����
	};


	//�I���W�i�����b�V��
	struct MY_MESH
	{
		DWORD dwNumVert;
		DWORD dwNumFace;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};


	struct InkData
	{
		D3DXVECTOR4 Color;
		D3DXVECTOR2 Uv;
		 float Scale;
	};

	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
	void ZTextureRender(unique_ptr<FollowCamera>& camera);
	void Render(std::unique_ptr<FollowCamera>& camera, D3DXVECTOR3 && worldPosition);

	bool IntersectSegment(const Segment & segment, unique_ptr<FollowCamera>& camera);

	DirectX::SimpleMath::Vector4 MatrixTimes(const DirectX::SimpleMath::Matrix & matrix, const DirectX::SimpleMath::Vector4 & vector);


//	const D3DXMATRIX& World() { return mW; }
private:
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;

	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pZTexConstantBuffer;



	MY_MESH m_Mesh;
	ID3D11InputLayout* m_pVertexLayout;

	ShaderManager shadermanager;//�V�F�[�_�[�֘A

	D3DXVECTOR3 m_vLight;

	ID3D11SamplerState* m_pSampleLimear;//�e�N�X�`���T���v��
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`��
	int birthcnt;

	MY_MATERIAL m_Material;//�}�e���A���\����

	ID3D11RasterizerState* m_pRasterizerState;


	ID3D11ShaderResourceView* inkTexture;//�C���N�e�N�X�`��



	D3DXMATRIX World;
	DirectX::SimpleMath::Vector3 localPosition;
	std::vector<Triangle> triangles;



	InkData inkData;


	float DEPTHTEX_WIDTH = 800 * 2;
	float DEPTHTEX_HEIGHT = 600 * 2;


	float WINDOW_WIDTH = 800;
	float WINDOW_HEIGHT = 600;

	ID3D11Device* device = Devices::Get().Device().Get();
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();



	ID3D11Texture2D* m_pDepthMap_Tex;//�[�x�}�b�v�e�N�X�`���[
	ID3D11RenderTargetView* m_pDepthMap_TexRTV;//�[�x�}�b�v�e�N�X�`���[��RTV
	ID3D11ShaderResourceView* m_pDepthMap_TexSRV;//�[�x�}�b�v�e�N�X�`���[��SRV
	ID3D11Texture2D* m_pDepthMap_DSTex;//�[�x�}�b�v�e�N�X�`���[�pDS
	ID3D11DepthStencilView* m_pDepthMap_DSTexDSV;//�[�x�}�b�v�e�N�X�`���[�pDS��DSV	


	ID3D11VertexShader* m_pDepthVertexShader;//�[�x�e�N�X�`���[�����_�����O�p
	ID3D11PixelShader* m_pDepthPixelShader;//�[�x�e�N�X�`���[�����_�����O�p
	D3DXMATRIX m_mClipToUV;

	D3DXVECTOR3 m_vLightPos;
	D3DXMATRIX mLight;
	;
};

