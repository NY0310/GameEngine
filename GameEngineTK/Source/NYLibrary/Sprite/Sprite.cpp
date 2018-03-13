#include "Sprite.h"

using namespace std;
using namespace NYLibrary;

Sprite::Sprite(LPCWSTR FileName, Dimension dimension)
	:dimension(dimension)
{
	LoadTexture(FileName);
	transparency = 0;
}

Sprite::~Sprite()
{
	vertexShader.Reset();
	pixelShader.Reset();
	constantBuffer.Reset();
	vertexBuffer.Reset();
	vertexLayout.Reset();
	texture.Reset();
	sampler.Reset();
}

void Sprite::Initialize()
{
	transparency = 1;
	vertexBufferPosition = D3DXVECTOR2(0, 0);
	auto& devices = Devices::Get();
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *compiledShader = nullptr;
	D3D11_BLEND_DESC bd;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (dimension == Dimension2)
	{
		if (FAILED(MakeShader("Resources/HLSL/Sprite.hlsl", "VS2D", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader)));
	}
	else
	{
		if (FAILED(MakeShader("Resources/HLSL/Sprite.hlsl", "VS3D", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &compiledShader)));
	}


	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device()->CreateInputLayout(layout, numElements, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf())))

	SAFE_RELEASE(compiledShader);



	//�s�N�Z���V�F�[�_�[�쐬
	if (FAILED(MakeShader("Resources/HLSL/Sprite.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &compiledShader)));
	SAFE_RELEASE(compiledShader);



	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	if (FAILED(devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer.ReleaseAndGetAddressOf())));



	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, sampler.ReleaseAndGetAddressOf());


}

/// <summary>
/// �e�N�X�`����ǂݍ���
/// </summary>
/// <param name="FileName">�t�@�C����</param>
/// <returns>���s����</returns>	
HRESULT Sprite::LoadTexture(LPCWSTR FileName)
{
	auto& devices = Devices::Get();


	//�e�N�X�`���[�ǂݍ���
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), FileName, nullptr, nullptr, texture.ReleaseAndGetAddressOf(), nullptr)))
	{
		MessageBoxA(0, "�e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
		return E_FAIL;
	}

	//�e�N�X�`���̃T�C�Y���擾
	GetTextureSize(texture.Get());
	if (dimension == Dimension3)
	CreateVertexBuffer3D();

}

void Sprite::Render()
{
	auto& devices = Devices::Get();

	devices.Context()->VSSetShader(vertexShader.Get(), nullptr, 0);
	devices.Context()->PSSetShader(pixelShader.Get(), nullptr, 0);

	SetConstantBuffer();


	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devices.Context().Get()->IASetInputLayout(vertexLayout.Get());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	devices.Context().Get()->PSSetSamplers(0, 1, sampler.GetAddressOf());
	devices.Context().Get()->PSSetShaderResources(0, 1, texture.GetAddressOf());	//�v���~�e�B�u�������_�����O

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(VertexData);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1,vertexBuffer.GetAddressOf(), &stride, &offset);

	devices.Context().Get()->Draw(4, 0);

}

void Sprite::SetConstantBuffer()
{
	auto& devices = Devices::Get();

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		cb.wvp = GetWVP();
		D3DXMatrixTranspose(&cb.wvp, &cb.wvp);
		cb.transparency = transparency;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer.Get(), 0);
	}


	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	devices.Context().Get()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

}

/// <summary>
/// �e�N�X�`���̃T�C�Y���擾
/// </summary>
/// <param name="srv">�V�F�[�_�[���\�[�X�r���[</param>
void Sprite::GetTextureSize(ID3D11ShaderResourceView * srv)
{
	ID3D11Resource* redource;
	srv->GetResource(&redource);
	D3D11_RESOURCE_DIMENSION type;
	redource->GetType(&type);
	if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		ID3D11Texture2D* tex2D = reinterpret_cast<ID3D11Texture2D*>(redource);
		D3D11_TEXTURE2D_DESC desc;
		tex2D->GetDesc(&desc);
		widthSize = desc.Width;
		hightSize = desc.Height;
	}
	else
	{
		MessageBoxA(0, "�e�N�X�`���[�̃T�C�Y���擾�ł��܂���ł����B", "", MB_OK);
	}

}

/// <summary>
/// �o�[�e�b�N�X�o�b�t�@�[�쐬
/// </summary>
/// <returns>���s����</returns>
HRESULT Sprite::CreateVertexBuffer2D()
{
	auto& devices = Devices::Get();
	float width = devices.Width() ;
	float hight = devices.Height();
	float shiftX = widthSize / 2;
	float shiftY = hightSize / 2;
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	//���_���`
	VertexData vertices[] =
	{
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x - shiftX) / width,(vertexBufferPosition.y - shiftY) / hight,0)), D3DXVECTOR2(0,1) },//���_1	
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x - shiftX) / width,(vertexBufferPosition.y + shiftY) / hight, 0)), D3DXVECTOR2(0, 0) }, //���_2
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x + shiftX) / width, (vertexBufferPosition.y - shiftY) / hight, 0)), D3DXVECTOR2(1, 1) },  //���_3
		{ Math::ChangeRegularDevice(D3DXVECTOR3((vertexBufferPosition.x + shiftX) / width,  (vertexBufferPosition.y + shiftY) / hight, 0)), D3DXVECTOR2(1, 0) },//���_4	
	};


	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexData) * 4;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bufferDesc, &InitData, vertexBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}

}


HRESULT Sprite::CreateVertexBuffer3D()
{
	auto& devices = Devices::Get();
	float width = devices.Width();
	float hight = devices.Height();

	//���_���`
	VertexData vertices[] =
	{
		{ D3DXVECTOR3(-widthSize / width,-hightSize / hight,1),D3DXVECTOR2(0,1) },//���_1	
		{ D3DXVECTOR3(-widthSize / width, hightSize / hight, 1), D3DXVECTOR2(0, 0) }, //���_2
		{ D3DXVECTOR3(widthSize / width, -hightSize / hight, 1), D3DXVECTOR2(1, 1) },  //���_3
		{ D3DXVECTOR3(widthSize / width, hightSize / hight, 1), D3DXVECTOR2(1, 0) },//���_4	
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexData) * 4;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bufferDesc, &InitData, vertexBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}


}