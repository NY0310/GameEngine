#include "InkParticleRenderer.h"

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
using namespace std;



InkParticleRenderer::~InkParticleRenderer()
{
	vertexShader.Reset();
	geometryShader.Reset();
	pixelShader.Reset();
	constantBuffer.Reset();
	vertexBuffer.Reset();
	vertexLayout.Reset();
	texture.Reset();
	sampler.Reset();
	blendState.Reset();
	depthStencilState.Reset();
}

/// <summary>
/// //�������֐���S�Ď��s����
/// </summary>
HRESULT InkParticleRenderer::Initialize()
{
	auto& devices = Devices::Get();



	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *compiledShader = nullptr;
	D3D11_BLEND_DESC bd;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PointSprite.hlsl", "VS", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;


	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device()->CreateInputLayout(layout, numElements, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf())))
	{
		return FALSE;
	}

	SAFE_RELEASE(compiledShader);


	////�W�I���g���V�F�[�_�[�쐬
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PointSprite.hlsl", "GS_Point", "gs_5_0", (void**)geometryShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;
	SAFE_RELEASE(compiledShader);



	//�s�N�Z���V�F�[�_�[�쐬
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/PointSprite.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &compiledShader))) return E_FAIL;
	SAFE_RELEASE(compiledShader);



	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	if (FAILED(devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}


	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	//png�t�@�C�����ɃA���t�@��񂪂���B�A���t�@�ɂ�蓧�߂���悤�w�肵�Ă���

	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(devices.Device().Get()->CreateBlendState(&bd, blendState.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	vertexData vertices[] =
	{
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertexData);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bufferDesc, &InitData, vertexBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}



	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, sampler.ReleaseAndGetAddressOf());

	//�e�N�X�`���[�ǂݍ���
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), L"Resources/PNG/Snow.png", nullptr, nullptr, texture.ReleaseAndGetAddressOf(), nullptr)))
	{
		MessageBoxA(0, "�e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}




/// <summary>
/// �S�p�[�e�B�N���̃��[���h�s����w�肵�ĕ`�揈�����Ă�
/// </summary>
/// <param name="camera">�Q�ƃJ����</param>
void InkParticleRenderer::Render(const vector<ConstantInkData> inkData)
{

	auto& devices = Devices::Get();
	devices.Context().Get()->VSSetShader(vertexShader.Get(), nullptr, 0);
	devices.Context().Get()->GSSetShader(geometryShader.Get(), nullptr, 0);
	devices.Context().Get()->PSSetShader(pixelShader.Get(), nullptr, 0);

	Matrix World, Scale, Tran, Rot, View, Proj , wvp;

	View = camera->GetView();
	Proj = camera->GetProjection();

	D3DXVECTOR3 vEyePt = Math::VectorToD3DXVECTOR3(camera->GetEyePos());


	//
	Matrix CancelRotation = View;
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0;
	
	CancelRotation.Invert();
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(vertexData);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	//UINT mask = 0xffffffff;
	//devices.Context().Get()->OMSetBlendState(blendState.Get(), nullptr, mask);

	////�[�x�e�X�g���h�����h�ɂ���
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//depthStencilDesc.DepthEnable = false;

	//devices.Device().Get()->CreateDepthStencilState(&depthStencilDesc, depthStencilState.ReleaseAndGetAddressOf());
	//devices.Context().Get()->OMSetDepthStencilState(depthStencilState.Get(), 1);



	//�p�[�e�B�N���P�����P���|�C���g�X�v���C�g�Ƃ��ĂT�O�O���`��
	for(auto data : inkData)
	{
		//���[���h�g�����X�t�H�[���͌X�ňقȂ�
		Scale =	Matrix::CreateScale(data.scale);
		Tran = Matrix::CreateTranslation(data.positon);
		World = Scale  * Tran;
		wvp = World * View * Proj;
		RenderSprite(wvp, data.color);
	}

	devices.Context().Get()->VSSetShader(nullptr, nullptr, 0);
	devices.Context().Get()->GSSetShader(nullptr, nullptr, 0);
	devices.Context().Get()->PSSetShader(nullptr, nullptr, 0);

}


/// <summary>
/// �`�揈��
/// </summary>
/// <param name="WVP"></param>
void InkParticleRenderer::RenderSprite(Matrix& WVP , Vector4& color)
{
	auto& devices = Devices::Get();


	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		Matrix m = WVP;
		m.Invert();
		cb.wvp = m;
		cb.color = color;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer.Get(), 0);
	}


	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	devices.Context().Get()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devices.Context().Get()->GSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());


	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devices.Context().Get()->IASetInputLayout(vertexLayout.Get());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	devices.Context().Get()->PSSetSamplers(0, 1, sampler.GetAddressOf());
	devices.Context().Get()->PSSetShaderResources(0, 1, texture.GetAddressOf());	//�v���~�e�B�u�������_�����O


	devices.Context().Get()->Draw(1, 0);

}
