#include "OBJAbstract.h"


using namespace std;

OBJAbstract::OBJAbstract()
{
	m_vLightPos = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	ZeroMemory(&m_mClipToUV, sizeof(D3DXMATRIX));
	m_mClipToUV._11 = 0.5;
	m_mClipToUV._22 = -0.5;
	m_mClipToUV._33 = 1;
	m_mClipToUV._41 = 0.5;
	m_mClipToUV._42 = 0.5;
	m_mClipToUV._44 = 1;

	// �����o�ϐ�������
	world = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	rotation = D3DXVECTOR3(0, 0, 0);
	IsUseQuternion = false;
}


OBJAbstract::~OBJAbstract()
{
}

void OBJAbstract::Init()
{
	InitD3D();

	m_vLight = D3DXVECTOR3(-1, 0, -1);
	D3DXVec3Normalize(&m_vLight, &m_vLight);
}

/// <summary>
/// �S�s��쐬
/// </summary>
void OBJAbstract::AllMatrixCreate()
{
	D3DXMatrixTranslation(&worldMatrix, this->world.x, this->world.y, this->world.z);
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
	if (IsUseQuternion)
	{
		D3DXMatrixRotationQuaternion(&rotationMatrix, &quaternion);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.x, rotation.y, rotation.z);
	}
}

HRESULT OBJAbstract::CreateShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "VS", "vs_5_0", (void**)&vertexShader, &pCompiledShader)))return E_FAIL;
	CreateVertexInputLayout(pCompiledShader);
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "PS", "ps_5_0", (void**)&pixelShader, &pCompiledShader)))return E_FAIL;

	return S_OK;
}

HRESULT OBJAbstract::CreateDepthTextureShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//�[�x�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "VS_Depth", "vs_5_0", (void**)&depthVertexShader, &pCompiledShader)))return E_FAIL;
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "PS_Depth", "ps_5_0", (void**)&depthPixelShader, &pCompiledShader)))return E_FAIL;
	return S_OK;
}

HRESULT OBJAbstract::CreateVertexInputLayout(ID3DBlob *compiledshader)
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(device->CreateInputLayout(layout, numElements, compiledshader->GetBufferPointer(), compiledshader->GetBufferSize(), &vertexLayout)))
	{
		return FALSE;
	}

	return S_OK;

}

ID3D11Buffer* OBJAbstract::CreateConstantBuffer(UINT size )
{
	ID3D11Buffer* buffer;
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = size;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&cb, nullptr, &buffer);
	
	return buffer;
}

HRESULT OBJAbstract::CreateDepthTexture()
{
	//�[�x�}�b�v�e�N�X�`���[���쐬
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	tdesc.Width = DEPTHTEX_WIDTH;
	tdesc.Height = DEPTHTEX_HEIGHT;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.MiscFlags = 0;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	device->CreateTexture2D(&tdesc, nullptr, &depthMapTex);

	//�[�x�}�b�v�e�N�X�`���[�p�@�����_�[�^�[�Q�b�g�r���[�쐬
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(depthMapTex, &DescRT, &depthMapTexRTV);





	//�[�x�}�b�v�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[(SRV)�쐬	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = tdesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(depthMapTex, &SRVDesc, &depthMapTexSRV);

	//�[�x�}�b�v�e�N�X�`���������_�[�^�[�Q�b�g�ɂ���ۂ̃f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = DEPTHTEX_WIDTH;
	descDepth.Height = DEPTHTEX_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	device->CreateTexture2D(&descDepth, nullptr, &depthMapDSTex);


	//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	device->CreateDepthStencilView(depthMapDSTex, nullptr, &depthMapDSTexDSV);

	return S_OK;

}

HRESULT OBJAbstract::CreateSampler()
{
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	device->CreateSamplerState(&SamDesc, &sampleLimear);

	

	return S_OK;

}

HRESULT OBJAbstract::InitD3D()
{

	//�V�F�[�_�[�쐬
	CreateShader();
	//�[�x�V�F�[�_�[�쐬
	CreateDepthTextureShader();
	//�[�x�e�N�X�`���쐬
	CreateDepthTexture();
	//�T���v���[�쐬
	CreateSampler();
	//�R���X�^���g�o�b�t�@�쐬
	constantBuffer = CreateConstantBuffer(sizeof(SIMPLESHADER_CONSTANT_BUFFER));
	//�[�x�e�N�X�`���R���X�^���g�o�b�t�@�쐬
	zTexConstantBuffer = CreateConstantBuffer(sizeof(ZTEXTURE_CONSTANT_BUFFER));

	return S_OK;
}




HRESULT OBJAbstract::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
{
	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rt");

	char key[110] = { 0 };
	while (!feof(fp))
	{
		D3DXVECTOR4 v(0, 0, 0, 0);
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s", key, sizeof(key));
		//�}���e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			strcpy_s(material.szName, key);
		}
		//Ka �A���r�G���g
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			material.Ka = v;
		}
		//Kb �f�B�q���[�Y
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			material.Kd = v;
		}
		//Ks �X�y�L�����[
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			material.Ks = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &material.szTextureName, sizeof(material.szTextureName));
			//�e�N�X�`���[�쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device, material.szTextureName, nullptr, nullptr, &texture, nullptr)))
			{
				return E_FAIL;
			}
		}

	}
	fclose(fp);

	return S_OK;
}


HRESULT OBJAbstract::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{

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
	FILE* fp = nullptr;
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
			LoadMaterialFromFile(key, &material);
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
	if (FAILED(device->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//�C���f�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	if (FAILED(device->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
		return FALSE;



	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}




void OBJAbstract::Render()
{
	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());



	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->GetEyePos());

	////���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//�g�p����V�F�[�_�[�̓o�^	
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		AllMatrixCreate();
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		world = scaleMatrix * rotationMatrix *  worldMatrix;
		//���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = world  *View * Proj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		//���[���h�A���C�g�r���[�A�ˉe�s���n��
		cb.mWLP = world * mLight * Proj;

		//���[���h�A���C�g�r���[�A�ˉe�s��A�e�N�X�`���s��
		cb.mWLPT = world * mLight * Proj * m_mClipToUV;

		//���C�g�̕�����n��
		D3DXVECTOR3 vLightDir(-1, 0, -1);
		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);

		//���_�ʒu��n��
		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(constantBuffer, 0);

	}
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, &sampleLimear);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	//deviceContext->PSSetShaderResources(2, 1, &depthMapTexSRV);//���C�g�r���[�ł̐[�x�e�N�X�`���쐬
															   //���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(vertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mesh.pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	deviceContext->IASetIndexBuffer(mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�������_�����O
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);


}

void OBJAbstract::ZTextureRender()
{

	//////////�r���[�|�[�g�̐ݒ�
	//D3D11_VIEWPORT vp;
	//vp.Width = DEPTHTEX_WIDTH;
	//vp.Height = DEPTHTEX_HEIGHT;
	//vp.MinDepth = 0.0f;
	//vp.MaxDepth = 1.0f;
	//vp.TopLeftX = 0;
	//vp.TopLeftY = 0;
	//deviceContext->RSSetViewports(1, &vp);
	//deviceContext->OMSetRenderTargets(1, &depthMapTexRTV, depthMapDSTexDSV);

	//float ClearColor[4] = { 0,0,1,1 };
	//deviceContext->ClearRenderTargetView(depthMapTexRTV, ClearColor);
	//deviceContext->ClearDepthStencilView(depthMapDSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A




	//D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	//D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());



	////���̃p�X�Ŏg�p����V�F�[�_�[�̓o�^
	//deviceContext->VSSetShader(depthVertexShader, nullptr, 0);
	//deviceContext->PSSetShader(depthPixelShader, nullptr, 0);
	////�����_�����O�^�[�Q�b�g��[�x�e�N�X�`���ɕύX
	//deviceContext->OMSetRenderTargets(1, &depthMapTexRTV, depthMapDSTexDSV);


	////// �r���[�g�����X�t�H�[�� �����ł̓��C�g����̎��E
	//////���C�g����]������
	//mLight;
	//D3DXVECTOR3 vLight = m_vLightPos;


	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	//D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	////�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	//D3D11_MAPPED_SUBRESOURCE pData;
	//ZTEXTURE_CONSTANT_BUFFER cb;
	//if (SUCCEEDED(deviceContext->Map(zTexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	//{

	//	//���[���h�A���C�g�A�ˉe�s���n��
	//	D3DXMATRIX m = worldMatrix * mLight * Proj;

	//	D3DXMatrixTranspose(&m, &m);
	//	cb.mWLP = m;

	//	memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ZTEXTURE_CONSTANT_BUFFER));
	//	deviceContext->Unmap(zTexConstantBuffer, 0);

	//}
	//sampleLimear = nullptr;
	////�e�N�X�`���[���V�F�[�_�[�ɓn��
	//deviceContext->PSSetSamplers(0, 1, &sampleLimear);
	//deviceContext->PSSetShaderResources(2, 1, &depthMapTexSRV);
	////���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	//deviceContext->VSSetConstantBuffers(0, 1, &zTexConstantBuffer);
	//deviceContext->PSSetConstantBuffers(0, 1, &zTexConstantBuffer);
	////���_�C���v�b�g���C�A�E�g���Z�b�g
	//deviceContext->IASetInputLayout(vertexLayout);
	////�v���~�e�B�u�E�g�|���W�[���Z�b�g
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	//UINT stride = sizeof(SimpleVertex);
	//UINT offset = 0;
	//deviceContext->IASetVertexBuffers(0, 1, &mesh.pVertexBuffer, &stride, &offset);
	////�C���f�b�N�X�o�b�t�@�[���Z�b�g
	//stride = sizeof(int);
	//offset = 0;
	//deviceContext->IASetIndexBuffer(mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	////�v���~�e�B�u�������_�����O
	//deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);

}
