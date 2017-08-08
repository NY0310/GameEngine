#include <d3d11_1.h>
#include <SimpleMath.h>
#include "FollowCamera.h"
#include <vector>
#include "Device.h"
#include "PARTICLE.h"
#include "ShaderManager.h"


#pragma comment(lib, "d3dcompiler.lib")

//���_�̍\����
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
					 //D3DXVECTOR2 Tex; //�e�N�X�`���[���W
};
//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
};


#pragma once
class ToManageParticle
{
public:
	inline ToManageParticle() {}
	void Init();//�������֐���S�Ď��s����
	HRESULT InitD3D();//d3d������
	HRESULT InitModel();//���f��������
	void Update();//�X�V����
	void Render(std::unique_ptr<FollowCamera>& camera);//�S�p�[�e�B�N���̃��[���h�s����w�肵�ĕ`�揈�����Ă�
	void RenderSprite(D3DXMATRIX & WVP);//�`�揈��

	void AddParticle(DirectX::SimpleMath::Vector3 enemypos);//�p�[�e�B�N���ǉ�
	//Getter
	inline const std::vector<PARTICLE*>& GetParticle() { return m_pParticle; }
	//Setter
private:
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* pCompiledShader2;

	ID3D11GeometryShader* m_pGeometryShader;//�W�I���g���V�F�[�_
	ID3D11PixelShader* m_pPixelShader;//�s�N�Z���V�F�[�_
	ID3D11VertexShader* m_pVertexShader;//�o�[�e�b�N�X�V�F�[�_
	ID3D11InputLayout* m_pVertexLayout;

	ID3D11SamplerState* m_pSampler;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11BlendState* m_pBlendState;
	ID3D11DepthStencilState* m_DepthStencilState;




	std::vector< PARTICLE*> m_pParticle;//�p�[�e�B�N��
	std::vector<int> m_Particlecnt;

	int Particlecnt = 0;
	int MAX_PARTICLE = 100;


	ShaderManager shadermanager;//�V�F�[�_�[�֘A
};

