#include "ToManageParticle.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")



/// <summary>
/// //�������֐���S�Ď��s����
/// </summary>
void ToManageParticle::Init()
{
	shadermanager = ShaderManager::Get();

	this->InitD3D();
	this->InitModel();
}

/// <summary>
/// �V�F�[�_�[�A�R���X�^���g�o�b�t�@�[�쐬
/// </summary>
/// <returns></returns>
HRESULT ToManageParticle::InitD3D()
{
	auto& devices = Devices::Get();



	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	D3D11_BLEND_DESC bd;
	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Shader.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader))) return E_FAIL;


	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	SAFE_RELEASE(pCompiledShader);


	////�W�I���g���V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Shader.hlsl", "GS_Point", "gs_5_0", (void**)&m_pGeometryShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);



	//�s�N�Z���V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Shader.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);



	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;


	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}


	//�[�x�e�X�g���h�����h�ɂ���
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = false;

	devices.Device().Get()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
	devices.Context().Get()->OMSetDepthStencilState(m_DepthStencilState, 1);

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

	if (FAILED(devices.Device().Get()->CreateBlendState(&bd, &m_pBlendState)))
	{
		return E_FAIL;
	}

	UINT mask = 0xffffffff;
	devices.Context().Get()->OMSetBlendState(m_pBlendState, NULL, mask);






	return S_OK;
}


/// <summary>
/// ���f���̏�����
/// </summary>
/// <returns></returns>
HRESULT ToManageParticle::InitModel()
{
	auto& devices = Devices::Get();


	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	devices.Device().Get()->CreateSamplerState(&SamDesc, &m_pSampler);
	//�e�N�X�`���[�ǂݍ���
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), L"particle.png", NULL, NULL, &m_pTexture, NULL)))
	{
		MessageBoxA(0, "�e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

/// <summary>
/// �X�V����
/// </summary>
void ToManageParticle::Update()
{

	std::vector<PARTICLE*>::iterator partecle = m_pParticle.begin();





	for (std::vector<int>::iterator it = m_Particlecnt.begin(); it != m_Particlecnt.end(); )
	{
		(*it)++;
		(*partecle)->Run();

		if ((*it) == 90)
		{
			partecle = m_pParticle.erase(partecle);
			it = m_Particlecnt.erase(it);
		}
		else
		{
			partecle++;
			it++;
		}
	}
	for (vector<PARTICLE*>::iterator iterator = m_pParticle.begin(); iterator != m_pParticle.end(); iterator++)
			{ 				(*iterator)->Run();
			}
			

}


/// <summary>
/// �S�p�[�e�B�N���̃��[���h�s����w�肵�ĕ`�揈�����Ă�
/// </summary>
/// <param name="camera">�Q�ƃJ����</param>
void ToManageParticle::Render(std::unique_ptr<FollowCamera>& camera)
{

	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, x, 0, 0);
	World = Tran;


	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->m_eyepos);
	D3DXVECTOR3 vLookatPt = shadermanager.VectorToD3DXVECTOR3(camera->m_refpos);
	D3DXVECTOR3 vUpVec = shadermanager.VectorToD3DXVECTOR3(camera->m_upvec);

	D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovRH(&Proj, camera->m_fovY, camera->m_aspect, camera->m_NearClip, camera->m_FarClip);



	D3DXMATRIX CancelRotation = View;
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0;
	D3DXMatrixInverse(&CancelRotation, NULL, &CancelRotation);
	World = CancelRotation * World;


	//�p�[�e�B�N���P�����P���|�C���g�X�v���C�g�Ƃ��ĂT�O�O���`��
	for (vector<PARTICLE*>::iterator iterator = m_pParticle.begin(); iterator != m_pParticle.end(); iterator++)
	{
		if ((*iterator) != nullptr)
		{
			for (int i = 0; i < MAX_PARTICLE; i++)
			{
				//���[���h�g�����X�t�H�[���͌X�ňقȂ�
				D3DXMATRIX Scale, Tran;
				D3DXMatrixScaling(&Scale, 5, 5, 5);
				D3DXVECTOR3 ParticlePos = (*iterator)->GetParticlePos(i);
				D3DXMatrixTranslation(&Tran, ParticlePos.x, ParticlePos.y, ParticlePos.z);
				World = Scale*CancelRotation*Tran;
				RenderSprite(World * View * Proj);
			}
		}
	}
}


/// <summary>
/// �`�揈��
/// </summary>
/// <param name="WVP"></param>
void ToManageParticle::RenderSprite(D3DXMATRIX& WVP)
{
	auto& devices = Devices::Get();


	//�g�p����V�F�[�_�[�̃Z�b�g
	devices.Context().Get()->VSSetShader(m_pVertexShader, NULL, 0);
	devices.Context().Get()->GSSetShader(m_pGeometryShader, NULL, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, NULL, 0);


	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = WVP;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
	}


	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);


	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampler);
	devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);	//�v���~�e�B�u�������_�����O

																		//devices.Context().Get()->RSSetState(m_states->CullNone());

	devices.Context().Get()->Draw(1, 0);


	devices.Context().Get()->VSSetShader(NULL, NULL, 0);
	devices.Context().Get()->GSSetShader(NULL, NULL, 0);
	devices.Context().Get()->PSSetShader(NULL, NULL, 0);

}

/// <summary>
/// �p�[�e�B�N���ǉ�
/// </summary>
void ToManageParticle::AddParticle(Vector3 pos)
{
   	m_pParticle.push_back(new PARTICLE(MAX_PARTICLE, shadermanager.VectorToD3DXVECTOR3(pos)));
 	m_Particlecnt.push_back(0);
}
