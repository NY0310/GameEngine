#include "Paint.h"

using namespace std;
ComPtr<ID3D11VertexShader> Paint::inkVertexShader;
ComPtr<ID3D11PixelShader> Paint::inkPixelShader;
ComPtr<ID3D11VertexShader> Paint::DripVertexShader;
ComPtr<ID3D11PixelShader>  Paint::DripPixelShader;
ComPtr<ID3D11VertexShader> Paint::updateVertexShader;
ComPtr<ID3D11PixelShader> Paint::updatePixelShader;
ComPtr<ID3D11InputLayout> Paint::inkVertexLayout;

ID3D11ShaderResourceView** Paint::GetInkTexSRV()
{ 
	return dripTextures->GetShaderResourceView().GetAddressOf();
}


HRESULT Paint::Initialize(bool isplane)
{
	textures = make_unique<SimpleTextures>(D3DXVECTOR2(Devices::Get().Width() * 2, Devices::Get().Height() * 2));
	textures->Initialize();
	dripTextures = make_unique<SimpleTextures>(D3DXVECTOR2(Devices::Get().Width() * 2, Devices::Get().Height() * 2));
	dripTextures->Initialize();

	if (!inkVertexShader.Get())
	{
		ID3DBlob *pCompiledShader = nullptr;
		//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[�쐬
		if (FAILED(MakeShader("Resources/HLSL/Campus.hlsl", "VS", "vs_5_0", (void**)inkVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
		//�C���N�e�N�X�`���p���_�C���v�b�g���C�A�E�g���Z�b�g
		D3D11_INPUT_ELEMENT_DESC inkInputLayout[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = sizeof(inkInputLayout) / sizeof(inkInputLayout[0]);
		//���_�C���v�b�g���C�A�E�g���쐬
		if (FAILED(device->CreateInputLayout(inkInputLayout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), inkVertexLayout.ReleaseAndGetAddressOf())))return E_FAIL;
		//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[�쐬
		if (FAILED(MakeShader("Resources/HLSL/Campus.hlsl", "PS", "ps_5_0", (void**)inkPixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;


		//���炷�V�F�[�_�[
		//�o�[�e�b�N�X�V�F�[�_�[
		if (isplane)
		{
			if (FAILED(MakeShader("Resources/HLSL/PlaneDrip.hlsl", "VS", "vs_5_0", (void**)DripVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
			//�s�N�Z���V�F�[�_�[
			if (FAILED(MakeShader("Resources/HLSL/PlaneDrip.hlsl", "PS", "ps_5_0", (void**)DripPixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
		}
		else
		{
			if (FAILED(MakeShader("Resources/HLSL/Drip.hlsl", "VS", "vs_5_0", (void**)DripVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
			//�s�N�Z���V�F�[�_�[
			if (FAILED(MakeShader("Resources/HLSL/Drip.hlsl", "PS", "ps_5_0", (void**)DripPixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

		}

		//�X�V����V�F�[�_�[
		//�o�[�e�b�N�X�V�F�[�_�[
		if (FAILED(MakeShader("Resources/HLSL/PaintUpdate.hlsl", "VS", "vs_5_0", (void**)updateVertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
		//�s�N�Z���V�F�[�_�[
		if (FAILED(MakeShader("Resources/HLSL/PaintUpdate.hlsl", "PS", "ps_5_0", (void**)updatePixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

	}




	//�C���N�p�e�N�X�`���p�R���X�^���g�o�b�t�@
	D3D11_BUFFER_DESC Inkcb;
	Inkcb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Inkcb.ByteWidth = sizeof(D3DXVECTOR4);
	Inkcb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Inkcb.MiscFlags = 0;
	Inkcb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(device->CreateBuffer(&Inkcb, nullptr, inkConstantBuffer.ReleaseAndGetAddressOf())))
	{
		return E_FAIL;
	}

	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	device->CreateSamplerState(&SamDesc, sampleLimear.ReleaseAndGetAddressOf());

	//�C���N�e�N�X�`�����쐬	
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device, "Resources/PNG/pink1.png", nullptr, nullptr,inkTexture.ReleaseAndGetAddressOf(), nullptr)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���[�쐬
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device, "Resources/HeightMap/BrushNormal.jpg", nullptr, nullptr, inkNormalMap.ReleaseAndGetAddressOf(), nullptr)))
	{
		return E_FAIL;
	}



	CreateVertexBuffer();
	return S_OK;

}




void Paint::CreateInk(D3DXVECTOR4 Color, D3DXVECTOR2 uv, float sclae)
{
	InkData inkdata;
	inkdata.Color = Color;
	inkdata.Uv = uv;
	inkdata.Scale  = 0.05f;
	inkdata.vertexBuffer = CreateVertexBuffer(inkdata);
	inkData.emplace_back(inkdata);
}

//�o�[�e�b�N�X�o�b�t�@�[�쐬
void Paint::CreateVertexBuffer()
{
	//���_���`
	PaintVertex vertices[] =
	{
		{D3DXVECTOR3(-1,-1,0),D3DXVECTOR2(0,1)},//���_1	
		{D3DXVECTOR3(-1, 1, 0), D3DXVECTOR2(0, 0)}, //���_2
		{D3DXVECTOR3(1, -1, 0), D3DXVECTOR2(1, 1)},  //���_3
		{D3DXVECTOR3(1, 1, 0), D3DXVECTOR2(1, 0)},//���_4	
	};
	//��̒��_�Ńo�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PaintVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	device->CreateBuffer(&bd, &InitData, dripVertexBuffer.ReleaseAndGetAddressOf());
}


ID3D11Buffer* Paint::CreateVertexBuffer(InkData & inkdata)
{
	//�C���N�T�C�Y�𐳋K�f�o�C�X���W�n�ɂ���
	float uvSize = inkdata.Scale;
	D3DXVECTOR2 Uv = inkdata.Uv;

	PaintVertex vertex[] = {
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y + uvSize,0)),D3DXVECTOR2(0,1) },
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x - uvSize,Uv.y - uvSize,0)),D3DXVECTOR2(0,0) },
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y + uvSize,0)),D3DXVECTOR2(1,1) },
		{ Math::ChangeRegularDevice(D3DXVECTOR3(Uv.x + uvSize,Uv.y - uvSize,0)),D3DXVECTOR2(1,0) },
	};


	//��̒��_�Ńo�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PaintVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertex;
	ID3D11Buffer*  canvasVertexBuffer = nullptr;
	device->CreateBuffer(&bd, &InitData, &canvasVertexBuffer);
	return canvasVertexBuffer;
}


void Paint::Render()
{
	textures->SetRenderTargets();
	InkRender();
	
	
	DripRender();

	dripTextures->SetRenderTargets();
	UpDateRender();
	//�C���N�f�[�^��������
	inkData.clear();
}

void Paint::InkRender()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g


	deviceContext->VSSetShader(inkVertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(inkPixelShader.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, inkTexture.GetAddressOf());//�C���N�̃��N�X�`��
	deviceContext->PSSetShaderResources(1, 1, dripTextures->GetShaderResourceView().GetAddressOf());//�C���N�̃��N�X�`��
	deviceContext->PSSetShaderResources(2, 1, inkNormalMap.GetAddressOf());//�C���N�̃m�[�}���}�b�v	

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	//���̃p�X�Ŏg�p����V�F�[�_�[�̓o�^
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, inkConstantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, inkConstantBuffer.GetAddressOf());
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(inkVertexLayout.Get());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);



	for (auto ink : inkData)
	{

		//�C���N�̐F��n��
		this->InkRender(ink);
	}

}



void Paint::InkRender(InkData& ink)
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g




	D3D11_MAPPED_SUBRESOURCE pData;
	D3DXVECTOR4 cb;
	if (SUCCEEDED(deviceContext->Map(inkConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		cb  = ink.Color;
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(D3DXVECTOR4));
		deviceContext->Unmap(inkConstantBuffer.Get(), 0);

	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(PaintVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &ink.vertexBuffer, &stride, &offset);


	deviceContext->Draw(4, 0);

}


void Paint::DripRender()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g


	deviceContext->VSSetShader(DripVertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(DripPixelShader.Get(), nullptr, 0);

	//�T���v���[�ƃe�N�X�`�����V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());

	if (inkData.size() == 0)
	{
		deviceContext->PSSetShaderResources(0, 1, dripTextures->GetShaderResourceView().GetAddressOf());//�C���N�̃��N�X�`��
	}
	else
	{
		deviceContext->PSSetShaderResources(0, 1, textures->GetShaderResourceView().GetAddressOf());//�C���N�̃��N�X�`��
	}

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(inkVertexLayout.Get());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(PaintVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, dripVertexBuffer.GetAddressOf(), &stride, &offset);


	//�v���~�e�B�u�������_�����O
	deviceContext->Draw(4, 0);
}

void Paint::UpDateRender()
{
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g


	deviceContext->VSSetShader(updateVertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(updatePixelShader.Get(), nullptr, 0);

	static int cnt = 0;
	//�T���v���[�ƃe�N�X�`�����V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, textures->GetShaderResourceView().GetAddressOf());//�C���N�̃��N�X�`��


	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(inkVertexLayout.Get());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(PaintVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, dripVertexBuffer.GetAddressOf(), &stride, &offset);


	//�v���~�e�B�u�������_�����O
	deviceContext->Draw(4, 0);

}






