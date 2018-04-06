#include "ShadowMap.h"

using namespace NYLibrary;
using namespace std;

unique_ptr<ShadowMap> ShadowMap::shadowMap;


/// <summary>
/// �V���h�E�}�b�v�̃C���X�^���X���擾����
/// </summary>
/// <returns>�V���h�E�}�b�v�̃C���X�^���X</returns>
ShadowMap * ShadowMap::GetInstance()
{
	if (!shadowMap.get())
	{
		shadowMap.reset(new ShadowMap);
	}

	return shadowMap.get();
}

/// <summary>
/// ������
/// </summary>
void ShadowMap::Initilize()
{
	CreateDSVTex2D();
	CreateRTVTex2D();
	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateShaderResourceView();
	CreateShader();
	CreateConstantBuffer();
	SamplerCreate();
}

/// <summary>
/// �I��
/// </summary>
void ShadowMap::Finalize()
{
	shadowMap.release();
	DSVTex2D.Reset();
	RTVTex2D.Reset();
	depthStencilView.Reset();
	renderTargetView.Reset();
	shaderResourceView.Reset();
	vertexShader.Reset();
	pixelShader.Reset();
}

/// <summary>
/// �f�v�X�X�e���V���p�̃e�N�X�`���쐬
/// </summary>
void ShadowMap::CreateDSVTex2D()
{
	auto& devices = Devices::Get();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = devices.Width();
	descDepth.Height = devices.Height();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	devices.Device().Get()->CreateTexture2D(&descDepth, nullptr, DSVTex2D.ReleaseAndGetAddressOf());
}

/// <summary>
/// �[�x�}�b�v�p�e�N�X�`��
/// </summary>
void ShadowMap::CreateRTVTex2D()
{
	auto& devices = Devices::Get();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = devices.Width();
	descDepth.Height = devices.Height();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.MiscFlags = 0;
	descDepth.Format = DXGI_FORMAT_R32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	devices.Device().Get()->CreateTexture2D(&descDepth, nullptr, RTVTex2D.ReleaseAndGetAddressOf());

}


/// <summary>
/// �[�x�X�e���V���r���[�쐬
/// </summary>
void ShadowMap::CreateDepthStencilView()
{
	auto& device = Devices::Get().Device();
	//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	device->CreateDepthStencilView(DSVTex2D.Get(), nullptr, depthStencilView.ReleaseAndGetAddressOf());
}

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�쐬
/// </summary>
void ShadowMap::CreateRenderTargetView()
{
	auto& device = Devices::Get().Device();

	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = DXGI_FORMAT_R32_FLOAT;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(RTVTex2D.Get(), &DescRT, renderTargetView.ReleaseAndGetAddressOf());
}

/// <summary>
/// �V�F�[�_�[���\�[�X�r���[���쐬
/// </summary>
void ShadowMap::CreateShaderResourceView()
{
	auto& device = Devices::Get().Device();

	//�[�x�}�b�v�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[(SRV)�쐬	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(RTVTex2D.Get(), &SRVDesc,shaderResourceView.GetAddressOf());
}

/// <summary>
/// �V�F�[�_�[�쐬
/// </summary>
void ShadowMap::CreateShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//�[�x�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[�쐬
	MakeShader("Resources/HLSL/ShadowMap.hlsl", "VS_Depth", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader);
	MakeShader("Resources/HLSL/ShadowMap.hlsl", "PS_Depth", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader);
}

/// <summary>
/// �R���X�^���g�o�b�t�@�[�쐬
/// </summary>
void ShadowMap::CreateConstantBuffer()
{
	auto& device = Devices::Get().Device();

	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&cb, nullptr, constantBuffer.ReleaseAndGetAddressOf());

}


/// <summary>
/// �����_�����O�^�[�Q�b�g���N���A����
/// </summary>
void ShadowMap::ClearRTVAndDSV()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
	float ClearColor[4] = { 0,0,0,1};
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), ClearColor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A
}


/// <summary>
/// �����_�[�^�[�Q�b�g�r���[��ݒ�
/// </summary>
void ShadowMap::OMSetRenderTargets()
{
	auto& deviceContext = Devices::Get().Context();
	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(Devices::Get().Width());
	vp.Height = static_cast<float>(Devices::Get().Height());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);
	//�����_�[�^�[�Q�b�g��[�x�e�N�X�`���[�ɕύX
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(),	depthStencilView.Get());
}

/// <summary>
/// �V���h�E�}�b�v�̃V�F�[�_�[��ݒ肷��
/// </summary>
void ShadowMap::SetShader()
{
	auto& deviceContext = Devices::Get().Context();
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
}



void ShadowMap::SamplerCreate()
{
	auto& device = Devices::Get().Device();

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 1;
	desc.MipLODBias = 0;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = +FLT_MAX;
	
	 // �T���v���[�X�e�[�g�𐶐�.
	device->CreateSamplerState(&desc, sampler.GetAddressOf());

}