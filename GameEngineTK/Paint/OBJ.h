/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../MatrixObject.h"
#include "../ShaderManager.h"
#include "../FollowCamera.h"


class Obj
{
protected:

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

	using Vector3 = DirectX::SimpleMath::Vector3;
public:

	Obj();
	virtual ~Obj();
	//����������
	void Initialze();
	HRESULT InitD3D();
	//�X�V
	virtual  void Update();
	//OBJ�t�@�C���ǂݍ���
	void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
	//�e�N�X�`���ǂݍ���
	void LoadTextuerFile(LPSTR filename) { D3DX11CreateShaderResourceViewFromFileA(device, filename, nullptr, nullptr, &texture, nullptr); }
	//�`��
	void Render();
	//Z�e�N�X�`���ɏ�������
	void ZTextureRender();
private:
	//�V�F�[�_�[�쐬
	HRESULT CreateShader();
	//�[�x�}�b�v�V�F�[�_�[�쐬
	HRESULT CreateDepthTextureShader();
	//���_�C���v�b�g���C�A�E�g�쐬
	HRESULT CreateVertexInputLayout(ID3DBlob * compiledshader);
	//�R���X�^���g�o�b�t�@�쐬
	ID3D11Buffer* CreateConstantBuffer(UINT size);
	//�[�x�}�b�v�e�N�X�`���쐬
	HRESULT CreateDepthTexture();
	//�T���v���[�쐬
	HRESULT CreateSampler();
	//OBJ�t�@�C���ǂݍ���
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	//�}�e���A���t�@�C���ǂݍ���
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
protected:

protected:
	/// <summary>
	/// ���b�V��
	/// </summary>
	MY_MESH mesh;//���b�V�����
	MY_MATERIAL material;//�}�e���A���\����
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
	/// �Ϗ�
	/// </summary>
	ID3D11Device* device = Devices::Get().Device().Get();//�f�o�C�X
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g
	FollowCamera* camera = FollowCamera::GetInstance();
	std::unique_ptr<MatrixObject> matrixObject = std::make_unique<MatrixObject>();//�s��



	D3DXMATRIX m_mClipToUV;//�e�N�X�`���s��
	D3DXMATRIX mLight;//���C�g�s��
};



