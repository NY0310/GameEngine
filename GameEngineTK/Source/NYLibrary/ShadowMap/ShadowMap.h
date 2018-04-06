/// <summary>
/// �V���h�E�}�b�v(�V���O���g��)
/// </summary>
#pragma once
#include <d3d11_1.h>
#include <wrl/client.h>
#include <memory>

#include "../ShaderManager/ShaderManager.h"
#include "../Device/Device.h"



using namespace Microsoft::WRL;

namespace NYLibrary
{
	class ShadowMap
	{
	public:
		struct ConstantBuffer
		{
			D3DXMATRIX wlp;//���[���h���C�g�v���W�F�N�V�����s��
		};

		//�f�X�g���N�^
		~ShadowMap() = default;
		//�R�s�[�R���X�g���N�^�֎~
		ShadowMap(const ShadowMap&) = delete;
		//����֎~
		ShadowMap& operator=(const ShadowMap&) = delete;
		//�V���h�E�}�b�v�̃C���X�^���X���擾����
		static ShadowMap* GetInstance();
		//������
		void Initilize();
		//�I��
		void Finalize();
		//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���N���A
		void ClearRTVAndDSV();
		//�����_�[�^�[�Q�b�g�r���[��ݒ�
		void OMSetRenderTargets();
		//�V���h�E�}�b�v�̃V�F�[�_�[��ݒ肷��
		void SetShader();
		//�[�x�X�e���V���r���[���擾
		const ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() { return depthStencilView; }
		//�����_�[�^�[�Q�b�g�r���[���擾����
		const ComPtr<ID3D11RenderTargetView>& GetRenderTargetView() { return  renderTargetView; }
		//�V�F�[�_�[���\�[�X�r���[���擾
		const ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return shaderResourceView; }
		//�R���X�^���g�o�b�t�@�[�擾
		const ComPtr<ID3D11Buffer>& GetConstantBuffer() { return constantBuffer; }
		//�T���v���[�擾
		const ComPtr<ID3D11SamplerState>& GetSamplerState() { return sampler; }
	private:
		//�R���X�g���N�^
		ShadowMap() = default;
		//�f�v�X�X�e���V���p�̃e�N�X�`���쐬
		void CreateDSVTex2D();
		//�[�x�}�b�v�p�e�N�X�`��
		void CreateRTVTex2D();
		//�[�x�X�e���V���r���[�쐬
		void CreateDepthStencilView();
		//�����_�[�^�[�Q�b�g�r���[�쐬
		void CreateRenderTargetView();
		//�V�F�[�_�[���\�[�X�r���[���쐬
		void CreateShaderResourceView();
		//�V�F�[�_�[�쐬
		void CreateShader();
		//�R���X�^���g�o�b�t�@�[�쐬
		void CreateConstantBuffer();
		//�T���v���[�쐬
		void SamplerCreate();
		//���N���X�C���X�^���X
		static std::unique_ptr<ShadowMap> shadowMap;
		//�f�v�X�X�e���V���r���[�p�̃e�N�X�`��
		ComPtr<ID3D11Texture2D> DSVTex2D;
		//�[�x�}�b�v�p�e�N�X�`��
		ComPtr<ID3D11Texture2D> RTVTex2D;
		//�[�x�f�v�X�X�e���V���r���[
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		//�����_�����O�^�[�Q�b�g�r���[
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		//�V�F�[�_�[���\�[�X�r���[
		ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		//�o�[�e�b�N�X�V�F�[�_�[
		ComPtr<ID3D11VertexShader> vertexShader;
		//�s�N�Z���V�F�[�_
		ComPtr<ID3D11PixelShader> pixelShader;
		//�R���X�^���g�o�b�t�@�[
		ComPtr<ID3D11Buffer> constantBuffer;
		//�T���v���[
		ComPtr<ID3D11SamplerState> sampler;
	};
}