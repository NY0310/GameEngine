#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"


#pragma once
class DisplacementMapping
{
private:
	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;//�ʒu
		D3DXVECTOR2 UV;
	};

	//�R���X�^���g�o�b�t�@�[
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;//���[���h�̃v���W�F�N�V�����s��
		D3DXVECTOR4 vLightDir;//���C�g����
	};




	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();

public:
	DisplacementMapping();
	~DisplacementMapping();
	HRESULT InitD3D();
	HRESULT InitShader();
	HRESULT InitPolygon();
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
	ID3D11SamplerState* m_pSamPoint;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pHeightTexture;//�e�N�X�`���[

};

