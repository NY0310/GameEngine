#include "OBJ.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
OBJ::OBJ()
{
	m_vLightPos = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	ZeroMemory(&m_mClipToUV, sizeof(D3DXMATRIX));
	m_mClipToUV._11 = 0.5;
	m_mClipToUV._22 = -0.5;
	m_mClipToUV._33 = 1;
	m_mClipToUV._41 = 0.5;
	m_mClipToUV._42 = 0.5;
	m_mClipToUV._44 = 1;

}



OBJ::~OBJ()
{
}

void OBJ::Init()
{
	shadermanager = ShaderManager::Get();
	InitD3D();

	m_vLight = D3DXVECTOR3(-1, 0, -1);
	D3DXVec3Normalize(&m_vLight, &m_vLight);

	birthcnt = 0;

}

HRESULT OBJ::InitD3D()
{
	auto& devices = Devices::Get();
	//D3D11_TEXTURE2D_DESC descDepth;


	////�[�x�}�b�v�e�N�X�`���[���쐬
	//D3D11_TEXTURE2D_DESC tdesc;
	//ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	//tdesc.Width = DEPTHTEX_WIDTH;
	//tdesc.Height = DEPTHTEX_HEIGHT;
	//tdesc.MipLevels = 1;
	//tdesc.ArraySize = 1;
	//tdesc.MiscFlags = 0;
	//tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	//tdesc.SampleDesc.Count = 1;
	//tdesc.SampleDesc.Quality = 0;
	//tdesc.Usage = D3D11_USAGE_DEFAULT;
	//tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//tdesc.CPUAccessFlags = 0;

	//device->CreateTexture2D(&tdesc, nullptr, &m_pDepthMap_Tex);

	////�[�x�}�b�v�e�N�X�`���[�p�@�����_�[�^�[�Q�b�g�r���[�쐬
	//D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	//DescRT.Format = tdesc.Format;
	//DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//DescRT.Texture2D.MipSlice = 0;

	//device->CreateRenderTargetView(m_pDepthMap_Tex, &DescRT, &m_pDepthMap_TexRTV);

	////�[�x�}�b�v�e�N�X�`���������_�[�^�[�Q�b�g�ɂ���ۂ̃f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
	//descDepth.Width = DEPTHTEX_WIDTH;
	//descDepth.Height = DEPTHTEX_HEIGHT;
	//descDepth.MipLevels = 1;
	//descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	//descDepth.SampleDesc.Count = 1;
	//descDepth.SampleDesc.Quality = 0;
	//descDepth.Usage = D3D11_USAGE_DEFAULT;
	//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//descDepth.CPUAccessFlags = 0;
	//descDepth.MiscFlags = 0;

	//device->CreateTexture2D(&descDepth, nullptr, &m_pDepthMap_DSTex);

	////���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	//device->CreateDepthStencilView(m_pDepthMap_DSTex, nullptr, &m_pDepthMap_DSTexDSV);

	////�[�x�}�b�v�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[(SRV)�쐬	
	//D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	//ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	//SRVDesc.Format = tdesc.Format;
	//SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//SRVDesc.Texture2D.MipLevels = 1;

	//device->CreateShaderResourceView(m_pDepthMap_Tex, &SRVDesc, &m_pDepthMap_TexSRV);



	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_DEBUG, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_DEBUG, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//�[�x�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Geometry.hlsl",  "VS_Depth", "vs_5_0", (void**)&m_pDepthVertexShader, &pCompiledShader)))return E_FAIL;
	if (FAILED(shadermanager.MakeShader("Geometry.hlsl", "PS_Depth", "ps_5_0", (void**)&m_pDepthPixelShader, &pCompiledShader)))return E_FAIL;

	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, nullptr, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC Zcb;
	Zcb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Zcb.ByteWidth = sizeof(ZTEXTURE_CONSTANT_BUFFER);
	Zcb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Zcb.MiscFlags = 0;
	Zcb.Usage = D3D11_USAGE_DYNAMIC;





	if (FAILED(devices.Device().Get()->CreateBuffer(&Zcb, nullptr, &m_pZTexConstantBuffer)))
	{
		return E_FAIL;
	}


	//�|���S���쐬
	if (FAILED(InitStaticMesh("Geometry+Normal+UV.obj", &m_Mesh)))
	{
		return E_FAIL;
	}
	//�e�N�X�`���[�쐬
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), "Hand_ColorMap.bmp", NULL, NULL, &m_pTexture, NULL)))
	{
		return E_FAIL;
	}

	//�C���N�e�N�X�`�����쐬	
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), "pink1.png", nullptr, nullptr, &inkTexture, nullptr)))
	{
		return E_FAIL;
	}

	////���b�V���쐬
	//if (FAILED(InitStaticMesh("Geometry.obj", &m_Mesh)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

HRESULT OBJ::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
{
	auto& devices = Devices::Get();


	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	char key[110] = { 0 };
	while (!feof(fp))
	{
	D3DXVECTOR4 v(0, 0, 0, 0);
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s", key, sizeof(key));
		//�}���e���A����
		if (strcmp(key,"newmtl")==0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			strcpy_s(m_Material.szName, key);
		}
		//Ka �A���r�G���g
		if (strcmp(key,"Ka")==0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ka = v;
		}
		//Kb �f�B�q���[�Y
		if (strcmp(key,"Kd")==0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Kd = v;
		}
		//Ks �X�y�L�����[
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			m_Material.Ks = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &m_Material.szTextureName, sizeof(m_Material.szTextureName));
			//�e�N�X�`���[�쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), m_Material.szTextureName, NULL, NULL, &m_pTexture, NULL)))
			{
				return E_FAIL;
			}
		}

	}
	fclose(fp);

	return S_OK;
}


HRESULT OBJ::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{

	auto& devices = Devices::Get();


	float x = 0, y = 0, z = 0;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVTCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[
	pMesh->dwNumVert = 0;
	pMesh->dwNumFace = 0;


	char key[200] = { 0 };
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A���ǂݍ���
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(key, &m_Material);
		}
		//���_
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		//�@��
		if (strcmp(key, "vn") == 0)
		{
			dwVNCount++;
		}
		//�e�N�X�`���[���W
		if (strcmp(key, "vt") == 0)
		{
			dwVTCount++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	SimpleVertex* pvVertexBuffer = new SimpleVertex[pMesh->dwNumFace * 3];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];
	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

													  //�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	dwVCount = 0;
	dwVTCount = 0;
	dwVNCount = 0;
	dwFCount = 0;

	SimpleVertex test[6000];

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;			
			
			dwVCount++;
		}
		//�@�� �ǂݍ���
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}


		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[dwVTCount].x = x;
			pvTexture[dwVTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
			dwVTCount++;
		}
		//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
		if (strcmp(key, "f") == 0)
		{
			//fscanf_s(fp, "%d// %d// %d//", &v1, &v2, &v3);
			fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			piFaceBuffer[dwFCount * 3] = dwFCount * 3;
			piFaceBuffer[dwFCount * 3 + 1] = dwFCount * 3 + 1;
			piFaceBuffer[dwFCount * 3 + 2] = dwFCount * 3 + 2;
			//���_�\���̂ɑ��
			pvVertexBuffer[dwFCount * 3].Pos = pvCoord[v1 - 1];
			pvVertexBuffer[dwFCount * 3].Normal = pvNormal[vn1 - 1];
			pvVertexBuffer[dwFCount * 3].Tex = pvTexture[vt1 - 1];
			pvVertexBuffer[dwFCount * 3 + 1].Pos = pvCoord[v2 - 1];
			pvVertexBuffer[dwFCount * 3 + 1].Normal = pvNormal[vn2 - 1];
			pvVertexBuffer[dwFCount * 3 + 1].Tex = pvTexture[vt2 - 1];
			pvVertexBuffer[dwFCount * 3 + 2].Pos = pvCoord[v3 - 1];
			pvVertexBuffer[dwFCount * 3 + 2].Normal = pvNormal[vn3 - 1];
			pvVertexBuffer[dwFCount * 3 + 2].Tex = pvTexture[vt3 - 1];

			Triangle tri;
			ComputeTriangle(shadermanager.D3DXVECTOR3ToVector(pvVertexBuffer[dwFCount * 3].Pos), shadermanager.D3DXVECTOR3ToVector(pvVertexBuffer[dwFCount * 3 + 1].Pos), shadermanager.D3DXVECTOR3ToVector(pvVertexBuffer[dwFCount * 3 + 2].Pos), &tri);
			tri.Uv0 = shadermanager.D3DXVECTOR2ToVector(pvTexture[vt1 - 1]);
			tri.Uv1 = shadermanager.D3DXVECTOR2ToVector(pvTexture[vt2 - 1]);
			tri.Uv2 = shadermanager.D3DXVECTOR2ToVector(pvTexture[vt3 - 1]);


			triangles.emplace_back(tri);
			dwFCount++;
		}
	}

	fclose(fp);

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) *pMesh->dwNumFace * 3;
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
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;

	

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}

void OBJ::Render(unique_ptr<FollowCamera>& camera)
{
	auto& devices = Devices::Get();





	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());




	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->GetEyePos());

	//auto& devices = Devices::Get();


	//D3DXMATRIX View;
	//D3DXMATRIX Proj;
	////���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;
	localPosition = Vector3(x, 1, 0);
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, localPosition.x, localPosition.y, localPosition.z);
	World = Tran;

	//�g�p����V�F�[�_�[�̓o�^	
	devices.Context().Get()->VSSetShader(m_pVertexShader, nullptr, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, nullptr, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		birthcnt+= 10;
		//���[���h�g�����X�t�H�[���͌X�ňقȂ�
		D3DXMATRIX Scale, Tran, Rot;

		//���[���h�s��v�Z
		D3DXMatrixScaling(&World, 5, 5, 5);
		//World *= Scale;
		 

		////���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		D3DXMatrixRotationY(&Rot, birthcnt / 1000.0f);//�P����yaw��]������
		World *= Rot;

		D3DXMatrixTranslation(&Tran, 0, 1, 0);

		World *= Tran;

		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = World *View * Proj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		//���[���h�A���C�g�r���[�A�ˉe�s���n��
		cb.mWLP = World * mLight * Proj;

		//���[���h�A���C�g�r���[�A�ˉe�s��A�e�N�X�`���s��
		cb.mWLPT = World * mLight * Proj * m_mClipToUV;

		//���C�g�̕�����n��
		D3DXVECTOR3 vLightDir(-1, 0, -1);
		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);

		//�C���N�̐F��n��
		Vector4 color = Colors::Green;
		cb.inkColor = shadermanager.VectorToD3DXVECTOR4(color);

		////�C���N��UV���W
		cb.inkUv = D3DXVECTOR2(0,0);

		////�C���N�̃T�C�Y�@
		cb.inkScale = 1.0f;


		////�f�B�t���[�Y�J���[��n��
		//cb.vDiffuse = m_Material.Kd;
		////�X�y�L�����[��n��
		//cb.vSpecular = m_Material.Ks;

		//���_�ʒu��n��
		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);
	
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);

	}
	m_pSampleLimear = nullptr;
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLimear); 
	devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);
	devices.Context().Get()->PSSetShaderResources(1, 1, &inkTexture);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	devices.Context().Get()->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�������_�����O
	devices.Context().Get()->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);


}

void OBJ::ZTextureRender(unique_ptr<FollowCamera>& camera)
{
	////////�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);
	deviceContext->OMSetRenderTargets(1, &m_pDepthMap_TexRTV, m_pDepthMap_DSTexDSV);

	float ClearColor[4] = { 0,0,1,1 };
	deviceContext->ClearRenderTargetView(m_pDepthMap_TexRTV, ClearColor);
	deviceContext->ClearDepthStencilView(m_pDepthMap_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A




	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());



	//���̃p�X�Ŏg�p����V�F�[�_�[�̓o�^
	deviceContext->VSSetShader(m_pDepthVertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pDepthPixelShader, nullptr, 0);
	//�����_�����O�^�[�Q�b�g��[�x�e�N�X�`���ɕύX
	deviceContext->OMSetRenderTargets(1, &m_pDepthMap_TexRTV, m_pDepthMap_DSTexDSV);

	////���f�����[���h�s�� 
	//D3DXMATRIX mR;

	//// �r���[�g�����X�t�H�[�� �����ł̓��C�g����̎��E
	////���C�g����]������
	mLight;
	D3DXVECTOR3 vLight = m_vLightPos;
	//static float angle = 0;
	//angle += 0.01f;
	//D3DXMatrixRotationY(&mR, angle);
	//D3DXVec3TransformCoord(&vLight, &vLight, &mR);


	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	ZTEXTURE_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(m_pZTexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{


		//���[���h�A���C�g�A�ˉe�s���n��
		D3DXMATRIX m = World * mLight * Proj;

		D3DXMatrixTranspose(&m, &m);
		cb.mWLP = m;




		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ZTEXTURE_CONSTANT_BUFFER));
		deviceContext->Unmap(m_pZTexConstantBuffer, 0);

	}
	m_pSampleLimear = nullptr;
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, &m_pSampleLimear);
	deviceContext->PSSetShaderResources(0, 1, &m_pDepthMap_TexSRV);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, &m_pZTexConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pZTexConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	deviceContext->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�������_�����O
	deviceContext->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);


}


//void OBJ::Render(unique_ptr<FollowCamera>& camera, ID3D11ShaderResourceView* & texture,D3DXMATRIX& world)
//{
//	auto& devices = Devices::Get();
//	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
//	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());
//
//
//
//
//	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->GetEyePos());
//
//	//auto& devices = Devices::Get();
//
//
//	//D3DXMATRIX View;
//	//D3DXMATRIX Proj;
//	////���[���h�g�����X�t�H�[��
//	static float x = 0;
//	x += 0.00001;
//	localPosition = Vector3(x, 1, 0);
//	D3DXMATRIX Tran;
//	D3DXMatrixTranslation(&Tran, localPosition.x, localPosition.y, localPosition.z);
//	World = Tran;
//
//	//�g�p����V�F�[�_�[�̓o�^	
//	devices.Context().Get()->VSSetShader(m_pVertexShader, nullptr, 0);
//	devices.Context().Get()->PSSetShader(m_pPixelShader, nullptr, 0);
//	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
//	D3D11_MAPPED_SUBRESOURCE pData;
//	SIMPLESHADER_CONSTANT_BUFFER cb;
//	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
//	{
//		birthcnt += 10;
//		//���[���h�g�����X�t�H�[���͌X�ňقȂ�
//		D3DXMATRIX Scale, Tran, Rot;
//
//		//���[���h�s��v�Z
//		D3DXMatrixScaling(&World, 5, 5, 5);
//		//World *= Scale;
//
//
//		////���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
//		D3DXMatrixRotationY(&Rot, birthcnt / 1000.0f);//�P����yaw��]������
//		World *= Rot;
//
//		D3DXMatrixTranslation(&Tran, 0, 1, 0);
//
//		World *= Tran;
//
//		//���[���h�A�J�����A�ˉe�s���n��
//		D3DXMATRIX m = World *View * Proj;
//		D3DXMatrixTranspose(&m, &m);
//		cb.mWVP = m;
//
//		//���C�g�̕�����n��
//		D3DXVECTOR3 vLightDir(-1, 0, -1);
//		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);
//
//		////�f�B�t���[�Y�J���[��n��
//		//cb.vDiffuse = m_Material.Kd;
//		////�X�y�L�����[��n��
//		//cb.vSpecular = m_Material.Ks;
//
//		//���_�ʒu��n��
//		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);
//
//	
//
//
//
//		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
//		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
//
//
//	}
//	m_pSampleLimear = nullptr;
//	//m_pTexture = NULL;
//	//�e�N�X�`���[���V�F�[�_�[�ɓn��
//	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLimear);
//	devices.Context().Get()->PSSetShaderResources(0, 1, &texture);
//	//devices.Context().Get()->PSSetShaderResources(1, 1, &inkTexture);	
//	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
//	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
//	//���_�C���v�b�g���C�A�E�g���Z�b�g
//	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
//	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
//	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
//	UINT stride = sizeof(SimpleVertex);
//	UINT offset = 0;
//	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
//	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
//	stride = sizeof(int);
//	offset = 0;
//	devices.Context().Get()->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	//�v���~�e�B�u�������_�����O
//	devices.Context().Get()->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);
//
//
//}




void OBJ::Render(std::unique_ptr<FollowCamera>& camera, D3DXVECTOR3&& worldPosition)
{
	D3DXMATRIX worldMatrix;
	D3DXMatrixTranslation(&worldMatrix, worldPosition.x, worldPosition.y, worldPosition.z);
	this->Render(camera);
	//this->Render(camera,inkTexture, worldMatrix);

	Matrix m;

}




//--------------------------------------------------------------------------------------
// �n�`�Ɛ����̌�������
// segment : ����
// �i�o�́jinter : ��_�i�|���S���̕��ʏ�ŁA�_�Ƃ̍Đڋߓ_�̍��W��Ԃ��j
//--------------------------------------------------------------------------------------
bool OBJ::IntersectSegment(const Segment& segment, Vector3* inter, unique_ptr<FollowCamera> camera)
{

	if (m_Mesh.pIndexBuffer == nullptr) return false;

	// �q�b�g�t���O��������
	bool hit = false;
	// �傫�������ŏ�����
	float distance = 1.0e5;
	// �p�x����p�ɒn�ʂƂ݂Ȃ��p�x�̌��E�l<�x>
	const float limit_angle = XMConvertToRadians(30.0f);
	Vector3 l_inter;



	// �t�s����v�Z
	D3DXMATRIX d3dWorldLocal;
	D3DXMatrixInverse(&d3dWorldLocal, nullptr, &d3dWorldLocal);
	Matrix WorldLocal = shadermanager.D3DXMATRIXToMatrix(d3dWorldLocal);
	// �R�s�[
	Segment localSegment = segment;
	// ���������[���h���W���烂�f�����W�n�Ɉ�������
	localSegment.Start = Vector3::Transform(localSegment.Start, WorldLocal);
	localSegment.End = Vector3::Transform(localSegment.End, WorldLocal);
	// �����̕����x�N�g�����擾
	Vector3 segmentNormal = localSegment.End - localSegment.Start;
	segmentNormal.Normalize();

	// �O�p�`�̐�
	int nTri = m_Mesh.dwNumFace;
	// �S�Ă̎O�p�`�ɂ���
	for (int i = 0; i < nTri; i++)
	{
		float temp_distance;
		Vector3 temp_inter;

		// ������x�N�g���Ɩ@���̓���
		// �������P�̃x�N�g���Q���m�̓��ς́A�R�T�C���i�x�N�g���̓��ς̒�`���j
		float cosine = -segmentNormal.Dot(triangles[i].Normal);
		//// �R�T�C���l����A������Ƃ̊p�x�����v�Z
		//float angle = acosf(cosine);
		//// ������Ƃ̊p�x�����E�p���傫����΁A�ʂ̌X�����傫���̂ŁA�n�ʂƂ݂Ȃ����X�L�b�v
		//if ( angle > limit_angle ) continue;

		//--������--
		const float limit_cosine = cosf(limit_angle);
		// �R�T�C�����P�̂Ƃ��Ƀx�N�g���Ԃ̊p�x��0�x�ł���A�x�N�g���̊p�x�����傫���قǁA�R�T�C���͏������̂ŁA
		// �R�T�C���l�̂܂ܔ�r����ƁA�p�x�̔�r�̏ꍇ�Ƒ召�֌W���t�ł���
		// �܂�A�R�T�C���l�����l��菬������΁A�ʂ̌X�����傫���̂ŁA�n�ʂƂ݂Ȃ����X�L�b�v
		if (cosine < limit_cosine) continue;
		//--�����ł����܂�--
		// �����ƎO�p�`�i�|���S���j�̌�������
		if (CheckSegment2Triangle(localSegment, triangles[i], &temp_inter))
		{
			hit = true;
			// �����̎n�_�ƏՓ˓_�̋������v�Z�i�߂肱�݋����j
			temp_distance = Vector3::Distance(localSegment.Start, temp_inter);
			// �߂肱�݋�������܂łōŏ��Ȃ�
			if (temp_distance < distance)
			{
				// �Փ˓_�̍��W�A�߂肱�݋������L�^
				l_inter = temp_inter;
				distance = temp_distance;


				Vector3 p1 = triangles[i].P0;
				Vector3 p2 = triangles[i].P1;
				Vector3 p3 = triangles[i].P2;
				Vector3 p = localPosition;

				Vector2 uv1 = triangles[i].Uv0;
				Vector2 uv2 = triangles[i].Uv1;
				Vector2 uv3 = triangles[i].Uv2;
				
				//�h����I�u�W�F�N�g�̃��[���h���W��������
				//PerspectiveCollect(�������e���l������UV���)
				Matrix mvp =  camera->GetProjection() * camera->GetView() * shadermanager.D3DXMATRIXToMatrix(World);
				//�e�_��ProjectionSpace�ւ̕ϊ�
				Vector4 p1_p = MatrixTimes(mvp , p1);
				Vector4 p2_p = MatrixTimes(mvp , p2);
				Vector4 p3_p = MatrixTimes(mvp , p3);
				Vector4 p_p = MatrixTimes(mvp , p);
				
					
				//�ʏ���W�ւ̕ϊ�(ProjectionSpace)
				Vector2 p1_n = Vector2(p1_p.x, p1_p.y) / p1_p.w;
				Vector2 p2_n = Vector2(p2_p.x, p2_p.y) / p2_p.w;
				Vector2 p3_n = Vector2(p3_p.x, p3_p.y) / p3_p.w;
				Vector2 p_n = Vector2(p_p.x, p_p.y) / p_p.w;


				//���_�̂Ȃ��O�p�`��_p�ɂ��3�������A�K�v�ɂȂ�ʐς��v�Z
				float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
				float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
				float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
				//�ʐϔ䂩��uv����
				float u = s1 / s;
				float v = s2 / s;
				float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
				Vector2 uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);

				inkData.Uv = shadermanager.VectorToD3DXVECTOR2(uv);
				return true;
			}
		}
	}

	//if (hit && inter != nullptr)
	//{
	//	// �Փ˓_�̍��W�����f�����W�n���烏�[���h���W�n�ɕϊ�
	//	const Matrix& localworld = m_Obj.GetLocalWorld();
	//	*inter = Vector3::Transform(l_inter, localworld);
	//}

	return hit;
}


Vector4 OBJ::MatrixTimes(const Matrix& matrix,const Vector3& vector)
{
	Vector4 vec;
	vec.x = matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * 0;
	vec.y = matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * 0;
	vec.z = matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * 0;
	vec.w = matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * 0;
	return vec;
}