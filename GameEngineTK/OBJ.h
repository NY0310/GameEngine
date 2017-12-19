/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include <Windows.h>
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"
#include "Collision.h"
#include "Line.h"
#include "Input/MouseUtil.h"
#include "Geometry.h"


class Player;

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

	struct CampusVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
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
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 0.1f;
		ID3D11Buffer* vertexBuffer;
	};

	struct InkDataBuffer
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 0.1f;
	};


	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	void CreateInkVertexBuffer(InkData & inkdata);
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
	void InkRender(unique_ptr<FollowCamera>& camera);
	void InkRender(unique_ptr<FollowCamera>& camera, InkData & uv);
	void ZTextureRender(unique_ptr<FollowCamera>& camera);
	void Render(std::unique_ptr<FollowCamera>& camera, D3DXVECTOR3 && worldPosition);

	bool IntersectSegment(const Segment & segment, unique_ptr<FollowCamera>& camera);


	bool IntersectSphere(const Sphere & sphere, unique_ptr<FollowCamera>& camera);

	void MouseRay(unique_ptr<FollowCamera>& camera, unique_ptr<Player>&  player);

	DirectX::SimpleMath::Vector3 * CalcScreenToXZ(DirectX::SimpleMath::Vector3 * pout, int Sx, int Sy, int Screen_w, int Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);


	DirectX::SimpleMath::Vector4 MatrixTimes(const DirectX::SimpleMath::Matrix & matrix, const DirectX::SimpleMath::Vector4 & vector);

	DirectX::SimpleMath::Vector3 * CalcScreenToWorld(DirectX::SimpleMath::Vector3 * pout, float Sx, float Sy, float fZ, float Screen_w, float Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);

	void SetPosition(const D3DXVECTOR3& position);
//	const D3DXMATRIX& World() { return mW; }
private:
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;

	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pZTexConstantBuffer;//z�e�N�X�`���p�R���X�^���g�o�b�t�@



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



	D3DXMATRIX worldmat;
	DirectX::SimpleMath::Vector3 localPosition;
	std::vector<Triangle> triangles;


	std::vector<InkData> inkData;//�C���N�f�[�^�z��


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






	/// <summary>
	/// �S�ẴC���N�������_�����O
	/// </summary>
	ID3D11InputLayout* inkVertexLayout;//�C���N�e�N�X�`���p���_�C���v�b�g���C�A�E�g
	ID3D11Buffer* inkConstantBuffer;//�C���N�e�N�X�`���p�R���X�^���g�o�b�t�@
	ID3D11Texture2D* inkTex;				//�C���N��h��e�N�X�`��
	ID3D11Texture2D* inkTexDS;				//�C���N��h��e�N�X�`��
	ID3D11RenderTargetView* inkTexRTV;//�C���N��h��e�N�X�`��TRV
	ID3D11ShaderResourceView* inkTexSRV;   //�C���N��h��e�N�X�`����SRV
	ID3D11DepthStencilView* inkDSTexDSV;//�[�x�}�b�v�e�N�X�`���[�pDS��DSV	
	ID3D11VertexShader* inkVertexShader;//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
	ID3D11PixelShader* inkPixelShader;//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[


	D3DXMATRIX m_mClipToUV;//�e�N�X�`���s��

	D3DXVECTOR3 m_vLightPos;//���C�h���W
	D3DXMATRIX mLight;//���C�g�s��
	D3DXVECTOR3 ObjScale;//�I�u�W�F�N�g�̍��W


	std::unique_ptr<DirectX::Mouse> mouse;
	Mouse::ButtonStateTracker tracker;

	Line* line;


	D3DXMATRIX WVP;


	D3DXVECTOR3 world;


	const D3DXVECTOR3& ChangeRegularDevice(const D3DXVECTOR3& position) { return D3DXVECTOR3(position.x * 2 - 1,(position.y* 2 - 1) * -1, 0) ; }
};




