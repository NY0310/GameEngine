#pragma once
#include <vector>
#include "MatrixObject.h"
#include "ShaderManager.h"
#include "FollowCamera.h"


class InkObj2
{
protected:
	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
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
		std::vector<Triangle> triangles;
		MY_MESH mesh;
	};


public:
	//�R���X�^���g�o�b�t�@�[�o�̓f�[�^
	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct InkData
	{
		DirectX::SimpleMath::Matrix wvp;//���[���h����ˉe�܂ł̕ϊ��s��
		DirectX::SimpleMath::Vector4 color;
	};
	InkObj2();

	//����������
	void Initialize();
	HRESULT InitD3D();
	//OBJ�t�@�C���ǂݍ���
	void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
	//�`��
	void Render(vector<InkData> inkDataList);
	void Render(InkData inkdata);
	std::unique_ptr<MatrixObject> matrixObject = std::make_unique<MatrixObject>();//�s��

private:
	//�V�F�[�_�[�쐬
	HRESULT CreateShader();
	//���_�C���v�b�g���C�A�E�g�쐬
	HRESULT CreateVertexInputLayout(ID3DBlob * compiledshader);
	//�R���X�^���g�o�b�t�@�쐬
	ID3D11Buffer* CreateConstantBuffer(UINT size);
	//OBJ�t�@�C���ǂݍ���
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);

protected:
	/// <summary>
	/// ���b�V��
	/// </summary>
	MY_MESH mesh;//���b�V�����
	ComPtr<ID3D11VertexShader> vertexShader;//�o�b�e�b�N�X�V�F�[�_�[
	ComPtr<ID3D11PixelShader> pixelShader;//�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11Buffer> constantBuffer;//�R���X�^���g�o�b�t�@
	ComPtr<ID3D11InputLayout> vertexLayout;//���_�C���v�b�g���C�A�E�g



	static std::map<LPSTR, MeshAndTriangles> modelDatas;//OBJ���f�����
	std::vector<Triangle> triangles;



	ID3D11Device* device = Devices::Get().Device().Get();//�f�o�C�X
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g
	FollowCamera* camera = FollowCamera::GetInstance();


};

