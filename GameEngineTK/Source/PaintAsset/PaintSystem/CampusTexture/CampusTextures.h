/// <summary>
/// ��{�I�ȃe�N�X�`��
/// �쐬���@�����ėp�r�ɍ����Ă��Ȃ���Ύ��삵�Ă�������
/// </summary>
#pragma once
#include <d3d11_1.h>
#include <D3DX10.h>
#include <memory>
#include "../../../NYLibrary/Device/Device.h"


class CampusTextures
{
public:
	//�R���X�g���N�^
	CampusTextures(D3DXVECTOR2 viewSize);
	CampusTextures() = delete;
	//�f�X�g���N�^
	~CampusTextures() = default;
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
	//�����_�����O�^�[�Q�b�g���g�p����
	void SetRenderTargets();
	//�����_�����O�^�[�Q�b�g���N���A����
	void ClearRenderTargetView();
private:
	//2D�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	//�V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	//�����_�����O�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
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
};
