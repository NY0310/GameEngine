#include "Sprite.h"

using namespace std;
using namespace NYLibrary;

ComPtr<ID3D11VertexShader> Sprite::vertexShader2D;//�o�[�e�b�N�X�V�F�[�_
ComPtr<ID3D11VertexShader> Sprite::vertexShader3D;//�o�[�e�b�N�X�V�F�[�_
ComPtr<ID3D11PixelShader> Sprite::pixelShader;//�s�N�Z���V�F�[�_
ComPtr<ID3D11InputLayout> Sprite::vertexLayout;//���_�C���b�v�b�g���C�A�E�g
ComPtr<ID3D11Buffer>Sprite::constantBuffer3D;
ComPtr<ID3D11Buffer> Sprite::constantBuffer2D;


Sprite::Sprite(LPCWSTR FileName, Dimension dimension)
	:dimension(dimension)
{
	LoadTexture(FileName);
}

Sprite::~Sprite()
{
	vertexShader2D.Reset();
	vertexShader3D.Reset();
	pixelShader.Reset();
	constantBuffer2D.Reset();
	constantBuffer3D.Reset();
	vertexBuffer.Reset();
	vertexLayout.Reset();
	texture.Reset();
	sampler.Reset();
}

void Sprite::Initialize()
{
	transparency = 1;
	anchor = D3DXVECTOR2(0.5f, 0.5f);

	if (vertexShader2D.Get())
		return;
	auto& devices = Devices::Get();
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *compiledShader = nullptr;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	MakeShader("Resources/HLSL/Sprite.hlsl", "VS2D", "vs_5_0", (void**)vertexShader2D.ReleaseAndGetAddressOf(), &compiledShader);

	MakeShader("Resources/HLSL/Sprite.hlsl", "VS3D", "vs_5_0", (void**)vertexShader3D.ReleaseAndGetAddressOf(), &compiledShader);
	


	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬

	devices.Device()->CreateInputLayout(layout, numElements, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), vertexLayout.ReleaseAndGetAddressOf());

	SAFE_RELEASE(compiledShader);



	//�s�N�Z���V�F�[�_�[�쐬
	MakeShader("Resources/HLSL/Sprite.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &compiledShader);
	SAFE_RELEASE(compiledShader);



	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer3D);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer3D.ReleaseAndGetAddressOf());

	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer2D);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	devices.Device()->CreateBuffer(&cb, nullptr, constantBuffer2D.ReleaseAndGetAddressOf());



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


	return S_OK;
}

void Sprite::Render()
{
	auto& devices = Devices::Get();


	if (dimension == Dimension2)
	{
		CreateVertexBuffer2D();
		devices.Context()->VSSetShader(vertexShader2D.Get(), nullptr, 0);
		SetConstantBuffer3D();
		SetConstantBuffer2D();
	}
	else
	{
		CreateVertexBuffer3D();
		devices.Context()->VSSetShader(vertexShader3D.Get(), nullptr, 0);
		SetConstantBuffer3D();
	}
	devices.Context()->PSSetShader(pixelShader.Get(), nullptr, 0);

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

void Sprite::SetConstantBuffer3D()
{
	auto& devices = Devices::Get();

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBuffer3D cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer3D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		cb.wvp = GetWVP();
		D3DXMatrixTranspose(&cb.wvp, &cb.wvp);
		cb.color = D3DXVECTOR4(GetColor().x, GetColor().y, GetColor().z,GetIsUseColor());
		cb.transparency = transparency;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer3D.Get(), 0);
	}


	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	devices.Context().Get()->VSSetConstantBuffers(0, 1, constantBuffer3D.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(0, 1, constantBuffer3D.GetAddressOf());

}



void Sprite::SetConstantBuffer2D()
{
	auto& devices = Devices::Get();

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBuffer2D cb;
	if (SUCCEEDED(devices.Context().Get()->Map(constantBuffer2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		cb.world = GetWorldMatrix();
		D3DXMatrixTranspose(&cb.world, &cb.world);

		cb.viewPortX = devices.Width();
		cb.viewPortY = devices.Height();

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(constantBuffer2D.Get(), 0);
	}


	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	devices.Context().Get()->VSSetConstantBuffers(1, 1, constantBuffer2D.GetAddressOf());
	devices.Context().Get()->PSSetConstantBuffers(1, 1, constantBuffer2D.GetAddressOf());

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
	float width = static_cast<float>(devices.Width());
	float hight = static_cast<float>(devices.Height());
	float shiftX = static_cast<float>(widthSize  / 2);
	float shiftY = static_cast<float>(hightSize  / 2);

	//���_���`
	VertexData vertices[] =
	{
		D3DXVECTOR3(-shiftX * (1 - anchor.x), shiftY * anchor.y,0),D3DXVECTOR2(0,1),//���_1,
		D3DXVECTOR3(-shiftX * (1 - anchor.x),-shiftY * (1 - anchor.y),0), D3DXVECTOR2(0,0),//���_2
		D3DXVECTOR3(shiftX *  anchor.x, shiftY * anchor.y,0),D3DXVECTOR2(1,1), //���_3
		D3DXVECTOR3(shiftX *  anchor.x,-shiftY * (1 - anchor.y),0),D3DXVECTOR2(1,0), //���_4
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
	return S_OK;
}


HRESULT Sprite::CreateVertexBuffer3D()
{
	auto& devices = Devices::Get();
	float width = static_cast<float>(devices.Width());
	float hight = static_cast<float>(devices.Height());

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

	return S_OK;

}
