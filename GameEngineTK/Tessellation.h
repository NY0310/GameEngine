#pragma once
#include "ShaderManager.h"
#include "FollowCamera.h"

#include<d3dx9.h>
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


class Tessellation
{
private:
	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
	};
	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	};
	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();

public:
	Tessellation();
	~Tessellation();
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Render(std::unique_ptr<FollowCamera>& camera);
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//�����f������
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11DepthStencilState* m_DepthStencilState;

};

