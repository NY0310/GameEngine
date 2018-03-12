/// <summary>
/// ��{�I�ȃe�N�X�`��
/// �쐬���@�����ėp�r�ɍ����Ă��Ȃ���Ύ��삵�Ă�������
/// </summary>
#pragma once
#include <d3d11_1.h>
#include <D3DX10.h>
#include <memory>
#include "../Device/Device.h"


namespace NYLibrary
{
	class SimpleTextures
	{
	public:
		//�R���X�g���N�^
		SimpleTextures(D3DXVECTOR2 viewSize);
		SimpleTextures() = delete;
		//�f�X�g���N�^
		~SimpleTextures() = default;
		//������
		void Initialize();
		//�I��
		void Finalize();
		//2D�e�N�X�`�����擾
		Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetTexture2D() { return texture2D; }
		//�V�F�[�_�[���\�[�X�r���[���擾
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return shaderResourceView; }
		//�����_�����O�^�[�Q�b�g�r���[���擾
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetView() { return renderTargetView; }
		//�[�x�X�e���V���r���[���擾
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() { return depthStencilView; }
		//�����_�����O�^�[�Q�b�g���g�p����
		void SetRenderTargets();
		//�����_�����O�^�[�Q�b�g���N���A����
		void ClearRenderTargetView();
	private:
		//2D�e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
		//�[�x�X�e���V���p2D�e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture2D;
		//�V�F�[�_�[���\�[�X�r���[
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		//�����_�����O�^�[�Q�b�g�r���[
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		//�[�x�X�e���V���r���[
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		//2D�e�N�X�`���̃t�H�[�}�b�g
		static const DXGI_FORMAT FORMAT;
		//�r���[�̑傫��
		D3DXVECTOR2 viewSize;
		//�f�o�C�X
		Microsoft::WRL::ComPtr<ID3D11Device> device;
	private:
		//2D�e�N�X�`���쐬
		void CreateTexture2D();
		//�V�F�[�_�[���\�[�X�r���[�쐬�@
		void CreateShaderResourceView();
		//�����_�����O�^�[�Q�b�g�r���[�쐬
		void CreateRenderTargetView();
		//�[�x�X�e���V���r���[�쐬
		void CreateDepthStencilView();
	};
};