/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include <vector>
#include <map>
#include "../MatrixObject.h"
#include "../ShaderManager.h"
#include "../FollowCamera.h"
#include "../UsingDirectSimpleMath.h"

using namespace UsingDirectSimpleMath;

using namespace Microsoft::WRL;


class InkObj
{
private:

	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 pos; //�ʒu
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

	struct MeshAndTriangles
	{
		MY_MESH mesh;
	};

	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct ConstantBuffer
	{
		Matrix wvp;//���[���h�s��
		Vector4 color;
	};
public:

	//�R���X�^���g�o�b�t�@�[�o�̓f�[�^
	struct ConstantInkData
	{
		Vector3 positon;
		Vector3 scale;
		Vector3	rotation;
		Vector4 color;
	};

	InkObj();
	virtual ~InkObj();
	//����������
	void Initialize();
	HRESULT InitD3D();
	//�X�V
	virtual  void Update();
	//OBJ�t�@�C���ǂݍ���
	void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
	//�`��
	void Render(const vector<ConstantInkData> inkData);
	void Render(Matrix & wvp, Vector4 & color);
	std::unique_ptr<MatrixObject> matrixInkObject;//�s��

private:
	//�V�F�[�_�[�쐬
	HRESULT CreateShader();
	//���_�C���v�b�g���C�A�E�g�쐬
	HRESULT CreateVertexInputLayout(ID3DBlob * compiledshader);
	//�R���X�^���g�o�b�t�@�쐬
	ID3D11Buffer* CreateConstantBuffer(UINT size);
	//�T���v���[�쐬
	HRESULT CreateSampler();
	//OBJ�t�@�C���ǂݍ���
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	//�}�e���A���t�@�C���ǂݍ���
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
protected:
	/// <summary>
	/// ���b�V��
	/// </summary>
	MY_MESH mesh;//���b�V�����
	MY_MATERIAL material;//�}�e���A���\����
	ComPtr<ID3D11VertexShader> vertexShader;//�o�b�e�b�N�X�V�F�[�_�[
	ComPtr<ID3D11PixelShader> pixelShader;//�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11Buffer> constantBuffer;//�R���X�^���g�o�b�t�@
	ComPtr<ID3D11InputLayout> vertexLayout;//���_�C���v�b�g���C�A�E�g
	ComPtr<ID3D11SamplerState> sampleLimear;//�e�N�X�`���T���v��
	ComPtr<ID3D11ShaderResourceView> texture;//�e�N�X�`��


	

	static std::map<LPSTR, MeshAndTriangles> modelDatas;//OBJ���f�����

	ID3D11Device* device = Devices::Get().Device().Get();//�f�o�C�X
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g
	FollowCamera* camera = FollowCamera::GetInstance();
	D3DXMATRIX mLight;//���C�g�s��
};



