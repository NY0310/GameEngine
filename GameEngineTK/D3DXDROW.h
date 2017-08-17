#pragma once
#include "ShaderManager.h"
#include "D3DXREAD.h"
#include "FollowCamera.h"



class D3DXDROW
{
private:
	//���_�̍\����
	struct MY_VERTEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR2 vTex;
	};
	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
	};
	//�I���W�i�����b�V��
	struct MY_MESH
	{
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};
public:
	D3DXDROW();
	~D3DXDROW();
	HRESULT InitShader();//�V�F�[�_������
	HRESULT InitD3D();//
	void Render(std::unique_ptr<FollowCamera>& camera);//�`��
private:
	ShaderManager shadermanager;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	D3DXREAD* m_d3dxread;
	//�����f������
	MY_MESH m_Mesh;
	ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[
	int birthcnt;

};

