#include "SimpleTextures.h"
using namespace std;
using namespace NYLibrary;

const DXGI_FORMAT SimpleTextures::FORMAT = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;



/// <summary>
/// �R���X�g���N�^
/// </summary>
SimpleTextures::SimpleTextures(D3DXVECTOR2 viewSize) :
	viewSize(viewSize)
{}

/// <summary>
/// ������
/// </summary>
/// <param name="view">�r���[�|�[�g�̑傫��</param>
/// <returns>���N���X�̃C���X�^���X</returns>
void SimpleTextures::Initialize()
{
	//�f�o�C�X�擾
	device  = Devices::Get().Device();
	//2D�e�N�X�`���쐬
	CreateTexture2D();
	//�V�F�[�_�[���\�[�X�r���[�쐬�@
	CreateShaderResourceView();
	//�����_�����O�^�[�Q�b�g�r���[�쐬
	CreateRenderTargetView();
	//�����_�����O�^�[�Q�b�g���N���A
	ClearRenderTargetView();
}


/// <summary>
/// �I��
/// </summary>
void SimpleTextures::Finalize()
{
	texture2D.Reset();
	shaderResourceView.Reset();
	renderTargetView.Reset();
}

/// <summary>
/// �����_�����O�^�[�Q�b�g���g�p����
/// </summary>
void SimpleTextures::SetRenderTargets()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	//////�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = Devices::Get().Width() * 2;
	vp.Height = Devices::Get().Height() * 2;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

}

/// <summary>
/// �����_�����O�^�[�Q�b�g���N���A����
/// </summary>
void SimpleTextures::ClearRenderTargetView()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	float ClearColor[4] = { 0,0,0,0 };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), ClearColor);

}

/// <summary>
/// 2D�e�N�X�`���쐬
/// </summary>
void SimpleTextures::CreateTexture2D()
{
	//�C���N�p�e�N�X�`�����쐬
	D3D11_TEXTURE2D_DESC inkdesc;
	ZeroMemory(&inkdesc, sizeof(D3D11_TEXTURE2D_DESC));
	inkdesc.Width = viewSize.x;
	inkdesc.Height = viewSize.y;
	inkdesc.MipLevels = 1;
	inkdesc.ArraySize = 1;
	inkdesc.MiscFlags = 0;
	inkdesc.Format = FORMAT;
	inkdesc.SampleDesc.Count = 1;
	inkdesc.SampleDesc.Quality = 0;
	inkdesc.Usage = D3D11_USAGE_DEFAULT;
	inkdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	inkdesc.CPUAccessFlags = 0;
	device.Get()->CreateTexture2D(&inkdesc, nullptr, texture2D.ReleaseAndGetAddressOf());
}



/// <summary>
/// �V�F�[�_�[���\�[�X�r���[�쐬�@
/// </summary>
void SimpleTextures::CreateShaderResourceView()
{
	//�C���N�p�e�N�X�`���@�V�F�[�_�[���\�[�X�r���[�쐬�i�e�N�X�`���[�m�F���p�j	
	D3D11_SHADER_RESOURCE_VIEW_DESC inkSRVDesc;
	ZeroMemory(&inkSRVDesc, sizeof(inkSRVDesc));
	inkSRVDesc.Format = FORMAT;
	inkSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	inkSRVDesc.Texture2D.MipLevels = 1;
	device.Get()->CreateShaderResourceView(texture2D.Get(), &inkSRVDesc, shaderResourceView.ReleaseAndGetAddressOf());
}

/// <summary>
/// �����_�����O�^�[�Q�b�g�r���[�쐬
/// </summary>
void SimpleTextures::CreateRenderTargetView()
{
	D3D11_RENDER_TARGET_VIEW_DESC inkDescRT;
	inkDescRT.Format = FORMAT;
	inkDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	inkDescRT.Texture2D.MipSlice = 0;
	device.Get()->CreateRenderTargetView(texture2D.Get(), &inkDescRT, renderTargetView.ReleaseAndGetAddressOf());
}

