#include "OBJ.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

#include <vector>

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



	line = new Line();
	line->InitD3D();


	// �����o�ϐ�������
	world = D3DXVECTOR3(0, 0, 0);


	campus = make_unique<Campus>();
}

void OBJ::SetPosition(const D3DXVECTOR3& position) {
	world = position;
}

void OBJ::InkRender()
{
	campus->Render();
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
	campus->InitD3D();

}

HRESULT OBJ::InitD3D()
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






	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "VS", "vs_5_0", (void**)&vertexShader, &pCompiledShader)))return E_FAIL;

	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(device->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &vertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "PS", "ps_5_0", (void**)&pixelShader, &pCompiledShader)))return E_FAIL;
	//�[�x�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "VS_Depth", "vs_5_0", (void**)&depthVertexShader, &pCompiledShader)))return E_FAIL;
	if (FAILED(shadermanager.MakeShader("Resources/HLSL/OBJ.hlsl", "PS_Depth", "ps_5_0", (void**)&depthPixelShader, &pCompiledShader)))return E_FAIL;

	SAFE_RELEASE(pCompiledShader);





	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(device->CreateBuffer(&cb, nullptr, &constantBuffer)))
	{
		return E_FAIL;
	}
	//Z�e�N�X�`���p�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC Zcb;
	Zcb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Zcb.ByteWidth = sizeof(ZTEXTURE_CONSTANT_BUFFER);
	Zcb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Zcb.MiscFlags = 0;
	Zcb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(device->CreateBuffer(&Zcb, nullptr, &zTexConstantBuffer)))
	{
		return E_FAIL;
	}



	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	device->CreateSamplerState(&SamDesc, &sampleLimear);


	//�|���S���쐬
	if (FAILED(InitStaticMesh("Resources/OBJ/Geometry+Normal+UV.obj", &mesh)))
	{
		return E_FAIL;
	}
	//�e�N�X�`���[�쐬
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(device, "Resources/BMP/Hand_ColorMap.bmp", nullptr, nullptr, &texture, nullptr)))
	{
		return E_FAIL;
	}





	////���b�V���쐬
	//if (FAILED(InitStaticMesh("Geometry.obj", &mesh)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}



HRESULT OBJ::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
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


HRESULT OBJ::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
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

void OBJ::Render()
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
		//���[���h�g�����X�t�H�[���͌X�ňقȂ�
		D3DXMATRIX Scale, Tran, Rot;
		//���[���h�s��v�Z
		ObjScale = D3DXVECTOR3(10, 10, 10);
		D3DXMatrixScaling(&World, ObjScale.x, ObjScale.y, ObjScale.z);

		static float rotY = 3.7f;
		//rotY += 0.001f;
		////���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		D3DXMatrixRotationZ(&Rot, rotY);//�P����yaw��]������
		World *= Rot;

		localPosition = Vector3(2,2,2);
		D3DXMatrixTranslation(&Tran, world.x, world.y, world.z);

		World *= Tran;
		worldMatrix = World;

		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = World  *View * Proj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//���[���h�A���C�g�r���[�A�ˉe�s���n��
		cb.mWLP = World * mLight * Proj;

		//���[���h�A���C�g�r���[�A�ˉe�s��A�e�N�X�`���s��
		cb.mWLPT = World * mLight * Proj * m_mClipToUV;

		//���C�g�̕�����n��
		D3DXVECTOR3 vLightDir(-1, 0, -1);
		cb.vLightDir = D3DXVECTOR4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);

		////�f�B�t���[�Y�J���[��n��
		//cb.vDiffuse = material.Kd;
		////�X�y�L�����[��n��
		//cb.vSpecular = material.Ks;

		//���_�ʒu��n��
		cb.vEyes = D3DXVECTOR4(vEyePt.x, vEyePt.y, vEyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(constantBuffer, 0);

	}
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, &sampleLimear);
	deviceContext->PSSetShaderResources(0, 1, &texture); 
	deviceContext->PSSetShaderResources(1, 1,&campus->GetInkTexSRV());//�S�C���N�������_�����O�����e�N�X�`��
	//deviceContext->PSSetShaderResources(2, 1, &campus->GetInkNormalMap());
	deviceContext->PSSetShaderResources(2, 1, &depthMapTexSRV);//���C�g�r���[�ł̐[�x�e�N�X�`���쐬
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

	line->Render();
}





void OBJ::ZTextureRender()
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
	deviceContext->OMSetRenderTargets(1, &depthMapTexRTV, depthMapDSTexDSV);

	float ClearColor[4] = { 0,0,1,1 };
	deviceContext->ClearRenderTargetView(depthMapTexRTV, ClearColor);
	deviceContext->ClearDepthStencilView(depthMapDSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A




	D3DXMATRIX View = shadermanager.MatrixToD3DXMATRIX(camera->GetView());
	D3DXMATRIX Proj = shadermanager.MatrixToD3DXMATRIX(camera->GetProjection());



	//���̃p�X�Ŏg�p����V�F�[�_�[�̓o�^
	deviceContext->VSSetShader(depthVertexShader, nullptr, 0);
	deviceContext->PSSetShader(depthPixelShader, nullptr, 0);
	//�����_�����O�^�[�Q�b�g��[�x�e�N�X�`���ɕύX
	deviceContext->OMSetRenderTargets(1, &depthMapTexRTV, depthMapDSTexDSV);


	//// �r���[�g�����X�t�H�[�� �����ł̓��C�g����̎��E
	////���C�g����]������
	mLight;
	D3DXVECTOR3 vLight = m_vLightPos;


	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	ZTEXTURE_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(zTexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		//���[���h�A���C�g�A�ˉe�s���n��
		D3DXMATRIX m = worldMatrix * mLight * Proj;

		D3DXMatrixTranspose(&m, &m);
		cb.mWLP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ZTEXTURE_CONSTANT_BUFFER));
		deviceContext->Unmap(zTexConstantBuffer, 0);

	}
	sampleLimear = nullptr;
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	deviceContext->PSSetSamplers(0, 1, &sampleLimear);
	deviceContext->PSSetShaderResources(2, 1, &depthMapTexSRV);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, &zTexConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &zTexConstantBuffer);
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





void OBJ::Render(D3DXVECTOR3&& worldPosition)
{
	D3DXMATRIX worldMatrix;
	D3DXMatrixTranslation(&worldMatrix, worldPosition.x, worldPosition.y, worldPosition.z);
	this->Render();
	//this->Render(camera,inkTexture, worldMatrix);

	Matrix m;

}


//--------------------------------------------------------------------------------------
// �n�`�Ɛ����̌�������
// segment : ����
// �i�o�́jinter : ��_�i�|���S���̕��ʏ�ŁA�_�Ƃ̍Đڋߓ_�̍��W��Ԃ��j
//--------------------------------------------------------------------------------------
bool OBJ::IntersectSegment(const Segment& segment, D3DXVECTOR2& uv)
{

	if (mesh.pIndexBuffer == nullptr) return false;

	// �傫�������ŏ�����
	float distance = 1.0e5;
	// �p�x����p�ɒn�ʂƂ݂Ȃ��p�x�̌��E�l<�x>
	const float limit_angle = XMConvertToRadians(30.0f);
	Vector3 l_inter;


	// �t�s����v�Z
	D3DXMATRIX d3dWorldLocal;
	D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldMatrix);
	Matrix WorldLocal = shadermanager.D3DXMATRIXToMatrix(d3dWorldLocal);


	// �R�s�[
	Segment localSegment = segment;
	// ���������[���h���W���烂�f�����W�n�Ɉ�������
	localSegment.Start = Vector3::Transform(localSegment.Start, WorldLocal);
	localSegment.End = Vector3::Transform(localSegment.End, WorldLocal);
	// �����̕����x�N�g�����擾
	Vector3 segmentNormal = localSegment.End - localSegment.Start;
	segmentNormal.Normalize();

	// �S�Ă̎O�p�`�ɂ���
	for (auto triangle : triangles)
	{
		float temp_distance;
		Vector3 temp_inter;

		// ������x�N�g���Ɩ@���̓���
		// �������P�̃x�N�g���Q���m�̓��ς́A�R�T�C���i�x�N�g���̓��ς̒�`���j
		float cosine = -segmentNormal.Dot(triangle.Normal);
		//// �R�T�C���l����A������Ƃ̊p�x�����v�Z
		//float angle = acosf(cosine);
		//// ������Ƃ̊p�x�����E�p���傫����΁A�ʂ̌X�����傫���̂ŁA�n�ʂƂ݂Ȃ����X�L�b�v
		//if ( angle > limit_angle ) continue;

		//--������--
		const float limit_cosine = cosf(limit_angle);
		// �R�T�C�����P�̂Ƃ��Ƀx�N�g���Ԃ̊p�x��0�x�ł���A�x�N�g���̊p�x�����傫���قǁA�R�T�C���͏������̂ŁA
		// �R�T�C���l�̂܂ܔ�r����ƁA�p�x�̔�r�̏ꍇ�Ƒ召�֌W���t�ł���
		// �܂�A�R�T�C���l�����l��菬������΁A�ʂ̌X�����傫���̂ŁA�n�ʂƂ݂Ȃ����X�L�b�v
		//if (cosine < limit_cosine) continue;
		//--�����ł����܂�--
		// �����ƎO�p�`�i�|���S���j�̌�������
		if (CheckSegment2Triangle(localSegment, triangle, &temp_inter))
		{
			// �����̎n�_�ƏՓ˓_�̋������v�Z�i�߂肱�݋����j
			temp_distance = Vector3::Distance(localSegment.Start, temp_inter);
			// �߂肱�݋�������܂łōŏ��Ȃ�
			{
				// �Փ˓_�̍��W�A�߂肱�݋������L�^
				l_inter = temp_inter;
				distance = temp_distance;


				Vector3 p1 = triangle.P0;
				Vector3 p2 = triangle.P1;
				Vector3 p3 = triangle.P2;
				Vector3 p = l_inter;

				Vector2 uv1 = triangle.Uv0;
				Vector2 uv2 = triangle.Uv1;
				Vector2 uv3 = triangle.Uv2;

				//�h����I�u�W�F�N�g�̃��[���h���W��������
				//PerspectiveCollect(�������e���l������UV���)
				Matrix mvp = shadermanager.D3DXMATRIXToMatrix(worldMatrix) * camera->GetView()* camera->GetProjection();
				//�e�_��ProjectionSpace�ւ̕ϊ�
				Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 0));
				Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 0));
				Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 0));
				Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 0));


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
				Vector2 _uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);
				uv =  shadermanager.VectorToD3DXVECTOR2(_uv);
				return true;
			}
		}
	}



	return false;
}



////--------------------------------------------------------------------------------------
//// �O�p�`�Ɖ~�̌�������
//// segment : ����
//// �i�o�́jinter : ��_�i�|���S���̕��ʏ�ŁA�_�Ƃ̍Đڋߓ_�̍��W��Ԃ��j
////--------------------------------------------------------------------------------------
bool OBJ::IntersectSphere(const Sphere& sphere)
{

	if (mesh.pIndexBuffer == nullptr) return false;
	// �t�s����v�Z
	D3DXMATRIX d3dWorldLocal;
	D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldMatrix);
	Matrix WorldLocal = shadermanager.D3DXMATRIXToMatrix(d3dWorldLocal);

	// �q�b�g�t���O��������
	bool hit = false;
	// �傫�������ŏ�����
	float over_length = 1.0e5;
	Vector3 l_inter;
	Vector3 l_normal;
	//	Vector3 l_down;
	// �X�P�[�����擾
	float scale = ObjScale.x;

	// �����R�s�[
	Sphere localsphere = sphere;

	// �X�P�[��0�̏ꍇ�A���肵�Ȃ�
	if (scale <= 1.0e-10) return false;

	// ���̒��S�_�����[���h���W���烂�f�����W�n�Ɉ�������
	localsphere.Center = Vector3::Transform(sphere.Center, WorldLocal);
	// ���a�����[���h�����[���h���W�n���烂�f�����W�n�ɕϊ�
	localsphere.Radius = sphere.Radius / scale;
	// �O�p�`�̐�
	int nTri = mesh.dwNumFace;
	// �S�Ă̎O�p�`�ɂ���
	for (int i = 0; i < nTri; i++)
	{
		float temp_over_length;
		Vector3 temp_inter;

		Triangle& tri = triangles[i];

		// �O�p�`�Ƌ��̓����蔻��
		if (CheckSphere2Triangle(localsphere, tri, &temp_inter))
		{

			Vector3 p1 = triangles[i].P0;
			Vector3 p2 = triangles[i].P1;
			Vector3 p3 = triangles[i].P2;
			Vector3 p = localPosition;

			Vector2 uv1 = triangles[i].Uv0;
			Vector2 uv2 = triangles[i].Uv1;
			Vector2 uv3 = triangles[i].Uv2;

			//�h����I�u�W�F�N�g�̃��[���h���W��������
			//PerspectiveCollect(�������e���l������UV���)
			Matrix mvp = shadermanager.D3DXMATRIXToMatrix(worldMatrix) * camera->GetView()* camera->GetProjection();
			//�e�_��ProjectionSpace�ւ̕ϊ�
			Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 1));
			Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 1));
			Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 1));
			Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 1));


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

		//	InkData inkdata;
			//	inkdata.Uv = shadermanager.VectorToD3DXVECTOR2(uv);

			//	inkData.emplace_back(inkdata);
			return true;
		}
	}
	return hit;
}

void OBJ::MouseRay( unique_ptr<Player>&  player)
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	if (mouse->IsTriggered(MouseUtil::Button::Left))
	{
		Vector3 pos;
		int x = mouse->GetPos().x;
		int y = mouse->GetPos().y;
		Matrix view = camera->GetView();
		Matrix proj = camera->GetProjection();
		CalcScreenToXZ(&pos, x, y, Devices::Get().Width(), Devices::Get().Height(), &view, &proj);
		Segment segment;
		segment.End = camera->GetEyePos();
		segment.Start = pos;
		line->SetVertex(shadermanager.VectorToD3DXVECTOR3(segment.End), shadermanager.VectorToD3DXVECTOR3(segment.Start));
		D3DXVECTOR4 color = Colors::Green;
		D3DXVECTOR2 uv;
		if(IntersectSegment(segment, uv))
		campus->CreateInk(color,uv,0.1f);
	}
	mouse = nullptr;
}


// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
Vector3* OBJ::CalcScreenToXZ(
	Vector3* pout,
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	Matrix* View,
	Matrix* Prj
) {
	Vector3 nearpos;
	Vector3 farpos;
	Vector3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	ray.Normalize();

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0) {
		// ����_
		float Lray = ray.Dot(Vector3(0, 1, 0));
		float LP0 = (-nearpos).Dot(Vector3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}

	return pout;
}





Vector4 OBJ::MatrixTimes(const Matrix& matrix, const Vector4& vector)
{
	Vector4 vec;
	vec.x = matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * vector.w;
	vec.y = matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * vector.w;
	vec.z = matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * vector.w;
	vec.w = matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * vector.w;
	return vec;
}


// �X�N���[�����W�����[���h���W�ɕϊ�
Vector3* OBJ::CalcScreenToWorld(
	Vector3* pout,
	float Sx,  // �X�N���[��X���W
	float Sy,  // �X�N���[��Y���W
	float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
	float Screen_w,
	float Screen_h,
	Matrix* View,
	Matrix* Prj
) {
	// �e�s��̋t�s����Z�o
	Matrix InvView, InvPrj, VP, InvViewport;
	//D3DXMatrixInverse(&InvView, NULL, View);
	//	���̑���
	XMMATRIX XMview = XMLoadFloat4x4(View);
	InvView = XMMatrixInverse(nullptr, XMview);

	//D3DXMatrixInverse(&InvPrj, nullptr, Prj);
	//	���̑���
	XMMATRIX XMprj = XMLoadFloat4x4(Prj);
	InvPrj = XMMatrixInverse(nullptr, XMprj);

	//D3DXMatrixIdentity(&VP);
	//	���̑���
	VP = XMMatrixIdentity();

	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	//D3DXMatrixInverse(&InvViewport, nullptr, &VP);
	XMMATRIX XMvp = XMLoadFloat4x4(&VP);
	InvViewport = XMMatrixInverse(nullptr, XMvp);

	//// �t�ϊ�
	Matrix tmp = InvViewport * InvPrj * InvView;
	//D3DXVec3TransformCoord(pout, &Vector3(Sx, Sy, fZ), &tmp);

	XMVECTOR vec = XMLoadFloat3(&Vector3(Sx, Sy, fZ));

	XMMATRIX XMtmp = XMLoadFloat4x4(&tmp);

	XMStoreFloat3(pout, XMVector3TransformCoord(vec, XMtmp));

	return pout;
}


