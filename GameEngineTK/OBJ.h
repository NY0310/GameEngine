#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"

//���_�̍\����
struct OBJSimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
					 //D3DXVECTOR2 Tex; //�e�N�X�`���[���W
};

//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
};


//�I���W�i�����b�V��
struct MY_MESH
{
	DWORD dwNumVert;
	DWORD dwNumFace;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
};


class OBJ
{
public:
	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
private:
	ID3D11PixelShader* m_Obj_pPixelShader;
	ID3D11VertexShader* m_Obj_pVertexShader;

	ID3D11Buffer* m_OBJpConstantBuffer;
	MY_MESH m_Mesh;
	ID3D11InputLayout* m_pVertexLayout;

	ShaderManager shadermanager;//�V�F�[�_�[�֘A

};

