#pragma once
#include "ShaderManager.h"
#include "FollowCamera.h"

#include<d3dx9.h>
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

class BumpMapping
{
private:
	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;//�ʒu
		D3DXVECTOR3 Normal;//�@��
		D3DXVECTOR2 vTex; //�e�N�X�`���[���W
		D3DXVECTOR4 vTangent; //�^���W�F���g�i�ڐ��j
	};
	//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mW;//���[���h�s��
		D3DXMATRIX mWIT;//���[���h�̋t�̓]�u�s��
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
		D3DXVECTOR4 vLightDir;//���C�g����
		D3DXVECTOR4 Ambient;//����
		D3DXVECTOR4 Diffuse;//�g�U���ˌ�
		D3DXVECTOR4 Specular;//���ʔ��ˌ�
		D3DXVECTOR4 vEye;//�J�����ʒu�i���_�ʒu�j
		D3DXVECTOR4 WaveMove;//�g�̓���
	};

	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();
public:
	BumpMapping();
	~BumpMapping();
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Render(std::unique_ptr<FollowCamera>& camera);

	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	D3DXVECTOR3 m_vLight;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	ID3D11Texture2D* m_pNormalTexture;//�m�[�}���}�b�v�̃e�N�X�`���[
	ID3D11ShaderResourceView* m_pNormalTextureSRV;//�m�[�}���}�b�v�̃e�N�X�`���[��SRV

};

