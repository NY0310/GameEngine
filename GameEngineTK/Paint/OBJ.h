/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include <Windows.h>
#include "../ShaderManager.h"
#include "../Device.h"
#include "../FollowCamera.h"
#include "../Collision.h"
#include "../Line.h"
#include "../Input/MouseUtil.h"
#include "../Geometry.h"
#include "Ink.h"


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





	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render();
	void InkRender();
	void ZTextureRender();
	void Render(D3DXVECTOR3 && worldPosition);

	bool IntersectSegment(const Segment & segment,D3DXVECTOR2& uv);
	bool IntersectSphere(const Sphere & sphere);

	void MouseRay(unique_ptr<Player>&  player);

	DirectX::SimpleMath::Vector3 * CalcScreenToXZ(DirectX::SimpleMath::Vector3 * pout, int Sx, int Sy, int Screen_w, int Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);


	DirectX::SimpleMath::Vector4 MatrixTimes(const DirectX::SimpleMath::Matrix & matrix, const DirectX::SimpleMath::Vector4 & vector);

	DirectX::SimpleMath::Vector3 * CalcScreenToWorld(DirectX::SimpleMath::Vector3 * pout, float Sx, float Sy, float fZ, float Screen_w, float Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);

	void SetPosition(const D3DXVECTOR3& position);
//	const D3DXMATRIX& World() { return mW; }
private:
	/// <summary>
	/// ���b�V��
	/// </summary>
	MY_MESH mesh;//���b�V�����
	MY_MATERIAL material;//�}�e���A���\����
	D3DXVECTOR3 world;//���[���h���W
	D3DXVECTOR3 ObjScale;//�T�C�Y
	ID3D11VertexShader* vertexShader;//�o�b�e�b�N�X�V�F�[�_�[
	ID3D11PixelShader* pixelShader;//�s�N�Z���V�F�[�_�[
	ID3D11Buffer* constantBuffer;//�R���X�^���g�o�b�t�@
	ID3D11InputLayout* vertexLayout;//���_�C���v�b�g���C�A�E�g
	ID3D11SamplerState* sampleLimear;//�e�N�X�`���T���v��
	ID3D11ShaderResourceView* texture;//�e�N�X�`��
	std::vector<Triangle> triangles;//�O�p�`�|���S��


	/// <summary>
	/// Z�e�N�X�`��
	/// </summary>
	ID3D11Buffer* zTexConstantBuffer;//z�e�N�X�`���p�R���X�^���g�o�b�t�@
	ID3D11VertexShader* depthVertexShader;//�[�x�e�N�X�`���[�����_�����O�p
	ID3D11PixelShader* depthPixelShader;//�[�x�e�N�X�`���[�����_�����O�p
	ID3D11Texture2D* depthMapTex;//�[�x�}�b�v�e�N�X�`���[
	ID3D11RenderTargetView* depthMapTexRTV;//�[�x�}�b�v�e�N�X�`���[��RTV
	ID3D11ShaderResourceView* depthMapTexSRV;//�[�x�}�b�v�e�N�X�`���[��SRV
	ID3D11Texture2D* depthMapDSTex;//�[�x�}�b�v�e�N�X�`���[�pDS
	ID3D11DepthStencilView* depthMapDSTexDSV;//�[�x�}�b�v�e�N�X�`���[�pDS��DSV	



	/// <summary>
	/// ���W���[��
	/// </summary>
	ID3D11Device* device = Devices::Get().Device().Get();//�f�o�C�X
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g
	ShaderManager shadermanager;//�V�F�[�_�[�֘A
	Line* line;//��
	FollowCamera* camera = FollowCamera::GetInstance();
	std::unique_ptr<Campus>campus;

	//�r���[�|�[�g�T�C�Y
	const UINT DEPTHTEX_WIDTH = Devices::Get().Width() * 2;
	const UINT DEPTHTEX_HEIGHT = Devices::Get().Height() * 2;


	D3DXVECTOR3 m_vLight;





	D3DXMATRIX worldMatrix;
	DirectX::SimpleMath::Vector3 localPosition;


	D3DXMATRIX m_mClipToUV;//�e�N�X�`���s��
	D3DXVECTOR3 m_vLightPos;//���C�h���W
	D3DXMATRIX mLight;//���C�g�s��



	


};




