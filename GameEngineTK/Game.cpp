//
// Game.cpp
//

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")

#include "Game.h"
#include <ctime>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;
using Microsoft::WRL::ComPtr;




Game::Game(): 
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	
	auto& devices = Devices::Get();
	devices.HWnd(window);
	devices.Width(width);
	devices.Height(height);

	// �f�o�C�X�𐶐����� Create Device
	devices.CreateDevice();
	// ���\�[�X�𐶐����� Create Resources
	devices.CreateResources();



	srand(static_cast<unsigned int>(time(nullptr)));

	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	//CreateDevice();

	//CreateResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/




	//	�������͂����ɏ���
	//	�L�[�{�[�h�̐���
	keyboard = std::make_unique<Keyboard>();
	//	�J�����̐���
	m_Camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	//	�J�����ɃL�[�{�[�h���Z�b�g
	m_Camera->SetKeyboard(keyboard.get());



	{//OBj3d�̃V�X�e��������
		Obj3d::CommonDef def;
		def.pCamera = m_Camera.get();
		def.pDevice = devices.Device().Get();
		def.pDeviceContext = devices.Context().Get();
	 //	3D�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
		Obj3d::InitializeCommon(def);
	}
	

	//�y�n�����蔻��
	LandShapeCommonDef lscDef;
	lscDef.pDevice = devices.Device().Get();
	lscDef.pDeviceContext = devices.Context().Get();
	lscDef.pCamera = m_Camera.get();
	//�y�n�̂����蔻�苤�ʏ�����
	LandShape::InitializeCommon(lscDef);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(devices.Context().Get());


	m_effect = std::make_unique<BasicEffect>(devices.Device().Get());

	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		devices.Device().Get()->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//	�ėp�X�e�[�g�𐶐�	q
	m_states = std::make_unique<CommonStates>(devices.Device().Get());

	//	�f�o�b�O�J�����𐶐�
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	//	�G�t�F�N�g�t�@�N�g���̍쐬
	m_factory = std::make_unique<EffectFactory>(devices.Device().Get());
	//	�e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"Resources");
	//	�V�����f���̐���
	m_objSkydome.LoadModel(L"Skydome.cmo");
	//�n�`�f�[�^�̓ǂݍ��� landshepe cmo�t�@�C����
	m_LandShape.Initialize(L"ground200m",L"ground200m");

	
	m_LandShape.SetRot (Vector3(0, 4.0f, 0));
	for (int i = 0; i < 20; i++)
	{
		//	���[���h�s����v�Z
		//	�X�P�[�����O
		Matrix scalemat = Matrix::CreateScale(1.0f);
		//	���[��
		Matrix rotmatz = Matrix::CreateRotationZ(0.0f);
		//	�s�b�`(�p�j
		Matrix rotmatx = Matrix::CreateRotationX(0.0f);
		//	���[(���ʊp�j
		Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(36.0f * (i + 1)));
		//	��]�s��̍���
		Matrix rotmat = rotmatz * rotmatx * rotmaty;
		//	���s�ړ�
		Matrix transmat;
		if (i < 10)
		{
			transmat = Matrix::CreateTranslation(20.0f, 0.0f, 0.0f);
		}
		else
		{
			transmat = Matrix::CreateTranslation(40.0f, 0.0f, 0.0f);
		}
		//	���[���h�s��̍���(SRT)
		m_worldBall[i] = scalemat * transmat * rotmat;
	}

	player = std::make_unique<Player>(keyboard.get());
	player->Initialize();
	//�Ǐ]�J�����Ƀv���C���[���Z�b�g
	m_Camera->SetPlayer(player.get());

	//�G�̐���
	int enemyNum = rand() % 10 + 1;

	m_Enemies.resize(enemyNum);

	for (int i = 0; i < enemyNum; i++)
	{
		m_Enemies[i] = std::make_unique<Enemy>(keyboard.get());
		m_Enemies[i]->Initialize();
	}

	Effect = ModelExpansionEffect::getInstance();

	m_spriteBatch = std::make_unique<SpriteBatch>(devices.Context().Get());
	m_spriteFont = std::make_unique<SpriteFont>(devices.Device().Get(), L"Resources/myfile.spritefont");


	m_str = L"CLEAR";

	
	this->InitD3D();
	this->InitModel();





}


HRESULT Game::InitD3D()
{
	
	auto& devices = Devices::Get();

	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;

	//�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(MakeShader("Shader.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader))) return E_FAIL;


	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}

	SAFE_RELEASE(pCompiledShader);


	////�W�I���g���V�F�[�_�[�쐬
	if (FAILED(MakeShader("Shader.hlsl", "GS_Point", "gs_5_0", (void**)&m_pGeometryShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);



	//�s�N�Z���V�F�[�_�[�쐬
	if (FAILED(MakeShader("Shader.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader))) return E_FAIL;
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
	D3D11_BLEND_DESC bd;
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


	//�p�[�e�B�N��������
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		m_pParticle.push_back(new PARTICLE(D3DXVECTOR3(0, 0, 0)));
	}



	/////////////////OBJ�t�H�[�}�b�g//////////////////////////////////

	////hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	//ID3DBlob *pCompiledShader = NULL;
	//ID3DBlob *pErrors = NULL;
	////�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	//if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "OBJ_VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	//{
	//	MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
	//	return E_FAIL;
	//}
	//SAFE_RELEASE(pErrors);

	//if (FAILED(devices.Device.Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	//{
	//	SAFE_RELEASE(pCompiledShader);
	//	MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
	//	return E_FAIL;
	//}
	////���_�C���v�b�g���C�A�E�g���`	
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//UINT numElements = sizeof(layout) / sizeof(layout[0]);
	////���_�C���v�b�g���C�A�E�g���쐬
	//if (FAILED(devices.Device.Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	//{
	//	return FALSE;
	//}
	////�u���u����s�N�Z���V�F�[�_�[�쐬
	//if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "OBJ_PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	//{
	//	MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
	//	return E_FAIL;
	//}
	//SAFE_RELEASE(pErrors);
	//if (FAILED(devices.Device.Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	//{
	//	SAFE_RELEASE(pCompiledShader);
	//	MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
	//	return E_FAIL;
	//}
	//SAFE_RELEASE(pCompiledShader);
	////�R���X�^���g�o�b�t�@�[�쐬
	//D3D11_BUFFER_DESC cb;
	//cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	//cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cb.MiscFlags = 0;
	//cb.Usage = D3D11_USAGE_DYNAMIC;

	//if (FAILED(devices.Device.Get()->CreateBuffer(&cb, NULL, &m_OBJpConstantBuffer)))
	//{
	//	return E_FAIL;
	//}


	////���b�V���쐬
	//if (FAILED(InitStaticMesh("Geometry.obj", &m_Mesh)))
	//{
	//	return E_FAIL;
	//}


	return S_OK;

}




//HRESULT MAIN::MakeShader(LPSTR szFileName,LPSTR szFuncName,LPSTR szProfileName,void** ppShader,ID3DBlob** ppBlob)
//hlsl�t�@�C����ǂݍ��݃V�F�[�_�[���쐬����
HRESULT Game::MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
{
	
	auto& devices = Devices::Get();


	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		if (FAILED(devices.Device().Get()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		if (FAILED(devices.Device().Get()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		if (FAILED(devices.Device().Get()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		if (FAILED(devices.Device().Get()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		if (FAILED(devices.Device().Get()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		if (FAILED(devices.Device().Get()->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader))) return E_FAIL;
	}


	return S_OK;
}


//
//HRESULT MAIN::InitModel()
//�|���S���A���b�V���Ȃǂ̃W�I���g���֘A���������i�����ł́h�_�h�p�j
HRESULT Game::InitModel()
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


HRESULT Game::InitStaticMesh(LPSTR FileName, MY_MESH* pMesh)
{

	auto& devices = Devices::Get();


	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	char key[190] = { 0 };
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//�܂��͒��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//���_
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}
	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	D3DXVECTOR3* pvVertexBuffer = new D3DXVECTOR3[pMesh->dwNumVert];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

													  //�{�ǂݍ���
	fseek(fp, SEEK_SET, 0);
	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[
	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));
		//���_ �ǂݍ���
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvVertexBuffer[dwVCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			pvVertexBuffer[dwVCount].y = y;
			pvVertexBuffer[dwVCount].z = z;
			dwVCount++;
		}
		//�t�F�C�X�i�|���S���j �ǂݍ���
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d// %d// %d//", &v1, &v2, &v3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			dwFCount++;
		}

	}

	fclose(fp);
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(D3DXVECTOR3) *pMesh->dwNumVert * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//�C���f�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}




void Game::RenderSprite(D3DXMATRIX& WVP)
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
	
	
	devices.Context().Get()->Draw(1, 0);


	//�p�[�e�B�N���̃A�j����i�߂�
	vector<PARTICLE*>::iterator iterator;

	for (iterator = m_pParticle.begin(); iterator != m_pParticle.end(); iterator++)
	{
		(*iterator)->Run();
	}


	devices.Context().Get()->VSSetShader(NULL, NULL, 0);
	devices.Context().Get()->GSSetShader(NULL, NULL, 0);
	devices.Context().Get()->PSSetShader(NULL, NULL, 0);

}



// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{


	// TODO: Add your game logic here.


	//	���t���[������������
	m_debugCamera->Update();

	m_LandShape.Update();

	//	�L�[�{�[�h�̎擾
	Keyboard::State key = keyboard->GetState();


	//if (KeybordTracker.IsKeyPressed(Keyboard::Keys::Space))

	//{
	//	HeadMoveFlag[FiringCnt] = true;
	//	FiringCnt++;
	//}
	//



	////�L�[�{�[�h�̗l�Ԃ��擾
	Keyboard::State kb = keyboard->GetState();
	//�v���C���̃A�b�v�f�[�g�֐�
	player->Update();

	////�L�[�{�[�h�g���b�J�[�̍X�V
	//KeybordTracker.Update(kb);

	// �S�p�[�c���s��X�V
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	{
		//Enemy* enemy = it->get();
		(*it)->Update();
	}



	{//�e�ۂƓG�̂����蔻��
		const Sphere& bulletSphere = player->GetCollisionNodeBullet();
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); )
		{
			Enemy* enemy = it->get();
			//�G�̔��苅���擾
			const Sphere& enemySphere = enemy->GetCollisionNodeBody();

			//��̋����������Ă�����
			if (CheckSpere2Sphere(bulletSphere, enemySphere))

			{
				Effect->Entry(L"Resources/HitEffect.cmo", 10, (*it)->GetTrans(), Vector3(0, 0.2, 0),
					Vector3(0,0,0), Vector3(2.0f, 2.0f, 2.0f), Vector3(4.0f, 4.0f, 4.0f), Vector3(2.0f, 2.0f, 2.0f),
					Vector3(4.0f, 4.0f, 4.0f), 50);
				//�G���E��
				it = m_Enemies.erase(it);
			}
			else
			{
				//�����Ȃ������̂�
				it++;
			}
		}

	}

	Effect->Update();
	//	�J�����̍��W
	//Vector3 CameraPos(head_pos.x, head_pos.y + 1, head_pos.z + 3);

	//
	{//	���@�ɒǏ]����J����
		//m_Camera->SetTarGetTrans(player->GetTrans());
		//m_Camera->SetTargetAngle(player->get);

		m_Camera->Update();
		m_view = m_Camera->GetView();
		m_proj = m_Camera->GetProjection();
	}

	m_objSkydome.Update();

	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Update();
	}

//	std::vector<std::unique_ptr<Enemy>> it = m_Enemies
	
	//std::wstringstream ss;
	////�X�g�����O�X�g���[�����當������擾
	//ss << L"aiueo" << m_cnt;

	//m_str = ss.str();
	

	//���@�̒n�`�ւ̂߂肱�݂���������
	{
		Sphere sphere = player->GetCollisionNodeBody();

		//���@�̃��[���h���W
		Vector3 trans = player->GetTrans();

		Vector3 sphere2player = trans - sphere.Center;
		//�߂肱�ݔr�˃x�N�g��
		Vector3 reject;

		if (m_LandShape.IntersectSphere(sphere,&reject))
		{
			sphere.Center += reject;
		}

		player->SetTrans(sphere.Center + sphere2player);


		player->Calc();

	}
	const Vector3 vel = player->GetVelocity();
	if (vel.y <= 0)
	{//���@���n�ʂɏ�鏈��
		//���@�̓����瑫���ւ̐���
		Segment player_segment;
		//���@�̃��[���h���W
		Vector3 trans = player->GetTrans();
		player_segment.Start = trans + Vector3(0, 1, 0);
		player_segment.End = trans + Vector3(0,-0.5f,0);

		//��_���W
		Vector3 inter;

		//�n�`�Ɛ����̂����蔻��(���C�L���X�g Ray)
		if (m_LandShape.IntersectSegment(player_segment,&inter))
		{
			trans.y = inter.y;
			//�������I��
			player->StopJump();
		}
		else
		{
			player->StartFall();
		}

		//���@���ړ�
		player->SetTrans(trans);
		//���[���h�s��̍X�V
		player->Calc();
	}
	
}

// Draws the scene.
void Game::Render()
{



	uint16_t indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	VertexPositionNormal vertices[] =
	{
		//	���W
		{ Vector3(-1.0f, +1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
		{ Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
		{ Vector3(+1.0f, +1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
		{ Vector3(+1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, +1.0f) },
	};

	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	// TODO: Add your rendering code here.
	//	�`�揈��������
	//m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//m_d3dContext->RSSetState(m_states->Wireframe());

	//	�r���[�s��𐶐�
	//m_view = Matrix::CreateLookAt(
	//	Vector3(0.f, 0.f, 2.f),							//	�J�������_
	//	Vector3(0, 0, 0),								//	�J�����Q�Ɠ_
	//	Vector3(0, 1, 0)								//	��ʏ�����x�N�g��
	//);

	//	�f�o�b�O�J��������r���[�s����擾
	//m_view = m_debugCamera->GetCameraMatrix();

	////	�J�����̈ʒu(���_���W�j
	//Vector3 eyepos(0, 0, 5);
	////	�J�����̌��Ă����(�����_/�Q�Ɠ_)
	//Vector3 refpos(0, 0, 0);
	////	�J�����̏�����x�N�g��
	//static float angle = 0.0f;
	//angle += 0.1f;
	//Vector3 upvec(cosf(angle), sinf(angle), 0);
	//upvec.Normalize();
	////	�r���[�s��̐���
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);

	////	�v���W�F�N�V�����s��𐶐�
	////	������������p
	//float fovY = XMConvertToRadians(60.0f);
	////	�A�X�y�N�g��(���c�̔䗦�j
	//float aspect = (float)m_outputWidth / m_outputHeight;
	////	�j�A�N���b�v(��O�̕\�����E����)
	//float nearclip = 0.1f;
	////	�t�@�[�N���b�v(���̕\�����E����)
	//float farclip = 10.0f;

	//m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearclip, farclip);

	//m_proj = Matrix::CreatePerspectiveFieldOfView(
	//	XM_PI / 4.f,									//	����p(�㉺�����j
	//	float(m_outputWidth) / float(m_outputHeight),	//	�A�X�y�N�g��
	//	0.1f,											//	�j�A�N���b�v
	//	500.f);											//	�t�@�[�N���b�v

	//m_effect->SetView(m_view);
	//m_effect->SetProjection(m_proj);

	//m_effect->Apply(m_d3dContext.Get());
	//m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//	�V�����f���̕`��
	m_objSkydome.Draw();

	//for (int i = 0; i < 40000; i++)
	//{
	//	Matrix matrix = Matrix::CreateTranslation(i / 200 - 100, 0.0f, i % 200 - 100);

	//	//	�n�ʃ��f���̕`��
	//	m_modelGround->Draw(m_d3dContext.Get(), *m_states, matrix, m_view, m_proj);
	//}

	////	�n�ʃ��f���̕`��
	//m_modelGround->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	m_LandShape.Draw();
	//for (int i = 0; i < 20; i++)
	//{
	//	//	�����f���̕`��
	//	m_modelBall->Draw(m_d3dContext.Get(), *m_states, m_worldBall[i], m_view, m_proj);
	//}

	//for (int i = 0; i < 20; i++)
	//{
	//	//	�e�B�[�|�b�g���f���̕`��
	//	m_modelTeapot->Draw(m_d3dContext.Get(), *m_states, m_worldTeapot[i], m_view, m_proj);
	//}

	//for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	//{
	//	it->Draw();
	//}


	//// �S�p�[�c���s��X�V
	//for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	//{
	//	Enemy* enemy = it->get();
	//	(*it)->Draw();
	//}


	////	�p�[�c1�̕`��
	//m_modelHead->Draw(m_d3dContext.Get(), *m_states, head_world, m_view, m_proj);

	////	�p�[�c2�̕`��
	//m_modelHead->Draw(m_d3dContext.Get(), *m_states, head_world2, m_view, m_proj);

	//m_batch->Begin();

	//m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

	player->Draw();

	//Effect->Draw();


	//m_batch->DrawLine(
	//	VertexPositionColor(
	//	SimpleMath::Vector3(0, 0, 0),
	//		SimpleMath::Color(1, 1, 1)),
	//	VertexPositionColor(
	//		SimpleMath::Vector3(800, 600, 0),
	//		SimpleMath::Color(1, 0, 0))
	//);

	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::SkyBlue);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::SandyBrown);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::PaleGoldenrod);
	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::SkyBlue);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::SandyBrown);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::PaleGoldenrod);

	//m_batch->DrawTriangle(v1, v2, v3);

	//m_batch->End();
//	CommonStates states(devices.Device.Get().Get());

	//m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
	//if (m_Enemies.empty() == true)
	//{
	//
	//	


	//	//�X�v���C�g�t�H���g�̕`��
	//	m_spriteFont->DrawString(m_spriteBatch.get(), m_str.c_str(), XMFLOAT2(200, 200));

	//}
	//m_spriteBatch->End();

	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, x, 0, 0);
	World = Tran;
	// �r���[�g�����X�t�H�[��
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f); //���_�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	// �v���W�F�N�V�����g�����X�t�H�[��
	D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f);


	//�p�[�e�B�N���P�����P���|�C���g�X�v���C�g�Ƃ��ĂT�O�O���`��
	vector<PARTICLE*>::iterator iterator;
	for (iterator = m_pParticle.begin(); iterator != m_pParticle.end(); iterator++)
	{
		(*iterator)->Run();
		//���[���h�g�����X�t�H�[���͌X�ňقȂ�
		D3DXMATRIX Scale, Tran;
		D3DXMatrixScaling(&Scale, 0.01, 0.01, 0.01);
		D3DXVECTOR3 ParticlePos = (*iterator)->GetParticlePos();
		D3DXMatrixTranslation(&Tran, ParticlePos.x, ParticlePos.y, ParticlePos.z);
		World = Scale*Tran;

		RenderSprite(World);
	}




	
	////�g�p����V�F�[�_�[�̓o�^	
	//m_d3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	//m_d3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	////�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	//D3D11_MAPPED_SUBRESOURCE pData;
	//SIMPLESHADER_CONSTANT_BUFFER cb;
	//if (SUCCEEDED(m_d3dContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	//{
	//	//���[���h�A�J�����A�ˉe�s���n��
	//	D3DXMATRIX m = World * View * Proj;
	//	D3DXMatrixTranspose(&m, &m);
	//	cb.mWVP = m;

	//	memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
	//	m_d3dContext->Unmap(m_pConstantBuffer, 0);
	//}
	////���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	//m_d3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//m_d3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	////���_�C���v�b�g���C�A�E�g���Z�b�g
	//m_d3dContext->IASetInputLayout(m_pVertexLayout);
	////�v���~�e�B�u�E�g�|���W�[���Z�b�g
	//m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	//UINT stride = sizeof(D3DXVECTOR3);
	//UINT offset = 0;
	//m_d3dContext->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	////�C���f�b�N�X�o�b�t�@�[���Z�b�g
	//stride = sizeof(int);
	//offset = 0;
	//m_d3dContext->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	////�v���~�e�B�u�������_�����O
	//m_d3dContext->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);

	Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	auto& devices = Devices::Get();


	// Clear the views.
	devices.Context().Get()->ClearRenderTargetView(devices.RenderTargetView().Get(), Colors::CornflowerBlue);
	devices.Context().Get()->ClearDepthStencilView(devices.DepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	devices.Context().Get()->OMSetRenderTargets(1, devices.RenderTargetView().GetAddressOf(), devices.DepthStencilView().Get());

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	devices.Context().Get()->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	auto& devices = Devices::Get();
	HRESULT hr = devices.SwapChain().Get()->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		devices.OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}
//
void Game::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	Devices::Get().CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}


void Game::MatrixToD3DXMATRIX(DirectX::SimpleMath::Matrix& matrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	D3DXMATRIX d3dxmatrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			matrix.m[i][j] = d3dxmatrix.m[i][j];
		}
	}
}

void Game::D3DXMATRIXToMatrix(D3DXMATRIX & d3dxmatrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	Matrix matrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			d3dxmatrix.m[i][j] = matrix.m[i][j];
		}
	}
}

// These are the resources that depend on the device.
//void Game::CreateDevice()
//{
////	UINT creationFlags = 0;
////
////#ifdef _DEBUG
////	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
////#endif
////
////	static const D3D_FEATURE_LEVEL featureLevels [] =
////	{
////		// TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
////		D3D_FEATURE_LEVEL_11_1,
////		D3D_FEATURE_LEVEL_11_0,
////		D3D_FEATURE_LEVEL_10_1,
////		D3D_FEATURE_LEVEL_10_0,
////		D3D_FEATURE_LEVEL_9_3,
////		D3D_FEATURE_LEVEL_9_2,
////		D3D_FEATURE_LEVEL_9_1,
////	};
////
////	// Create the DX11 API device object, and get a corresponding context.
////	HRESULT hr = D3D11CreateDevice(
////		nullptr,                                // specify nullptr to use the default adapter
////		D3D_DRIVER_TYPE_HARDWARE,
////		nullptr,
////		creationFlags,
////		featureLevels,
////		_countof(featureLevels),
////		D3D11_SDK_VERSION,
////		devices.Device.Get().ReleaseAndGetAddressOf(),   // returns the Direct3D device created
////		&m_featureLevel,                        // returns feature level of device created
////		m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
////		);
////
////	if (hr == E_INVALIDARG)
////	{
////		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
////		hr = D3D11CreateDevice(nullptr,
////			D3D_DRIVER_TYPE_HARDWARE,
////			nullptr,
////			creationFlags,
////			&featureLevels[1],
////			_countof(featureLevels) - 1,
////			D3D11_SDK_VERSION,
////			devices.Device.Get().ReleaseAndGetAddressOf(),
////			&m_featureLevel,
////			m_d3dContext.ReleaseAndGetAddressOf()
////			);
////	}
////
////	DX::ThrowIfFailed(hr);
////
////#ifndef NDEBUG
////	ComPtr<ID3D11Debug> d3dDebug;
////	if (SUCCEEDED(devices.Device.Get().As(&d3dDebug)))
////	{
////		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
////		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
////		{
////#ifdef _DEBUG
////			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
////			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
////#endif
////			D3D11_MESSAGE_ID hide [] =
////			{
////				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
////				// TODO: Add more message IDs here as needed.
////			};
////			D3D11_INFO_QUEUE_FILTER filter = {};
////			filter.DenyList.NumIDs = _countof(hide);
////			filter.DenyList.pIDList = hide;
////			d3dInfoQueue->AddStorageFilterEntries(&filter);
////		}
////	}
////#endif
////
////	// DirectX 11.1 if present
////	if (SUCCEEDED(devices.Device.Get().As(&devices.Device.Get()1)))
////		(void)m_d3dContext.As(&m_d3dContext1);
////
////	// TODO: Initialize device dependent objects here (independent of window size).
////}
////
////// Allocate all memory resources that change on a window SizeChanged event.
////void Game::CreateResources()
////{
////	// Clear the previous window size specific context.
////	ID3D11RenderTargetView* nullViews [] = { nullptr };
////	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
////	m_renderTargetView.Reset();
////	m_depthStencilView.Reset();
////	m_d3dContext->Flush();
////
////	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
////	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
////	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
////	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
////	UINT backBufferCount = 2;
////
////	// If the swap chain already exists, resize it, otherwise create one.
////	if (m_swapChain)
////	{
////		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);
////
////		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
////		{
////			// If the device was removed for any reason, a new device and swap chain will need to be created.
////			OnDeviceLost();
////
////			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
////			// and correctly set up the new device.
////			return;
////		}
////		else
////		{
////			DX::ThrowIfFailed(hr);
////		}
////	}
////	else
////	{
////		// First, retrieve the underlying DXGI Device from the D3D Device.
////		ComPtr<IDXGIDevice1> dxgiDevice;
////		DX::ThrowIfFailed(devices.Device.Get().As(&dxgiDevice));
////
////		// Identify the physical adapter (GPU or card) this device is running on.
////		ComPtr<IDXGIAdapter> dxgiAdapter;
////		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
////
////		// And obtain the factory object that created it.
////		ComPtr<IDXGIFactory1> dxgiFactory;
////		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));
////
////		ComPtr<IDXGIFactory2> dxgiFactory2;
////		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
////		{
////			// DirectX 11.1 or later
////
////			// Create a descriptor for the swap chain.
////			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
////			swapChainDesc.Width = backBufferWidth;
////			swapChainDesc.Height = backBufferHeight;
////			swapChainDesc.Format = backBufferFormat;
////			swapChainDesc.SampleDesc.Count = 1;
////			swapChainDesc.SampleDesc.Quality = 0;
////			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
////			swapChainDesc.BufferCount = backBufferCount;
////
////			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
////			fsSwapChainDesc.Windowed = TRUE;
////
////			// Create a SwapChain from a Win32 window.
////			DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
////				devices.Device.Get().Get(),
////				m_window,
////				&swapChainDesc,
////				&fsSwapChainDesc,
////				nullptr,
////				m_swapChain1.ReleaseAndGetAddressOf()
////				));
////
////			DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
////		}
////		else
////		{
////			DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
////			swapChainDesc.BufferCount = backBufferCount;
////			swapChainDesc.BufferDesc.Width = backBufferWidth;
////			swapChainDesc.BufferDesc.Height = backBufferHeight;
////			swapChainDesc.BufferDesc.Format = backBufferFormat;
////			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
////			swapChainDesc.OutputWindow = m_window;
////			swapChainDesc.SampleDesc.Count = 1;
////			swapChainDesc.SampleDesc.Quality = 0;
////			swapChainDesc.Windowed = TRUE;
////
////			DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(devices.Device.Get().Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
////		}
////
////		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
////		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
////	}
////
////	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
////	ComPtr<ID3D11Texture2D> backBuffer;
////	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));
////
////	// Create a view interface on the rendertarget to use on bind.
////	DX::ThrowIfFailed(devices.Device.Get()->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));
////
////	// Allocate a 2-D surface as the depth/stencil buffer and
////	// create a DepthStencil view on this surface to use on bind.
////	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
////
////	ComPtr<ID3D11Texture2D> depthStencil;
////	DX::ThrowIfFailed(devices.Device.Get()->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));
////
////	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
////	DX::ThrowIfFailed(devices.Device.Get()->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));
////
////	// TODO: Initialize windows-size dependent objects here.
////}
////
////void Game::OnDeviceLost()
////{
////	// TODO: Add Direct3D resource cleanup here.
////
////	m_depthStencilView.Reset();
////	m_renderTargetView.Reset();
////	m_swapChain1.Reset();
////	m_swapChain.Reset();
////	m_d3dContext1.Reset();
////	m_d3dContext.Reset();
////	devices.Device.Get()1.Reset();
////	devices.Device.Get().Reset();
////
////	CreateDevice();
////
////	CreateResources();
//}