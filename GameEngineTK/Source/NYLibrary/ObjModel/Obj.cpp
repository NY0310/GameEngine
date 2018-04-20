#include "Obj.h"


using namespace std;
using namespace NYLibrary;



std::map < LPSTR, Obj::MeshAndTriangles> Obj::modelDatas;
const float Obj::MAX_BREAK_CNT = 150.0f;//�j��J�E���g�̏��


Obj::Obj(LPSTR FileName)
{
	ZeroMemory(&clipToUV, sizeof(D3DXMATRIX));
	clipToUV._11 = 0.5f;
	clipToUV._22 = -0.5f;
	clipToUV._33 = 1;
	clipToUV._41 = 0.5f;
	clipToUV._42 = 0.5f;
	clipToUV._44 = 1;
	LoadOBJFile(FileName);
}


Obj::~Obj()
{
}

void Obj::CreateAddChild()
{
	breakLerp = make_shared<Lerp>(0, BREAK_CNT,100.0f);
	breakLerp->DisableUpdate();
	std::function<void()> thisFunction = std::bind(&Obj::OnBreakEnd, this);
	breakLerp->addListener(thisFunction);
	AddChild(breakLerp);
}

void Obj::Initialize()
{
	////�V�F�[�_�[�쐬
	if (!vertexShader.Get())
	{
		CreateShader();
	}
	//�T���v���[�쐬
	CreateSampler();
	//�R���X�^���g�o�b�t�@�쐬
	constantBuffer = CreateConstantBuffer(sizeof(SIMPLESHADER_CONSTANT_BUFFER));
	breakTime = 0;
}



HRESULT Obj::CreateShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(MakeShader("Resources/HLSL/OBJ.hlsl", "VS", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
	CreateVertexInputLayout(pCompiledShader);
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(MakeShader("Resources/HLSL/OBJ.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
	//�W�I���g���V�F�[�_�[�쐬
	if (FAILED(MakeShader("Resources/HLSL/OBJ.hlsl", "GS", "gs_5_0", (void**)geometryShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

	return S_OK;
}


HRESULT Obj::CreateVertexInputLayout(ID3DBlob *compiledshader)
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

/// <summary>
/// �R���X�^���g�o�b�t�@�[�쐬
/// </summary>
/// <param name="size">�T�C�Y</param>
/// <returns>�쐬�����R���X�^���g�o�b�t�@�[</returns>
ID3D11Buffer* Obj::CreateConstantBuffer(UINT size)
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


HRESULT Obj::CreateSampler()
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



HRESULT Obj::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
{
	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = nullptr;
	LPSTR upName = "Resources/MTL/";
	char* buf = new char[100];

	strcpy(buf, upName);
	strcat(buf, FileName);
	fopen_s(&fp, buf, "rt");
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
	delete[] buf;
	return S_OK;
}


HRESULT Obj::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{
	//���ɓǂݍ���ł��邩
	if (modelDatas.count(FileName) != 0)
	{
		triangles = modelDatas[FileName].triangles;
		SetPolygons(triangles);
		mesh = modelDatas[FileName].mesh;
		SetLocalSize(modelDatas[FileName].localSize);
		return S_OK;
	}

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
	Triangle tri;

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			if (x == 0.0f)
			{
				pvCoord[dwVCount].x = x;
			}
			else
			{
				pvCoord[dwVCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			}
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;


			dwVCount++;
		}
		//�@�� �ǂݍ���
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			if (x == 0.0f)
			{
				pvNormal[dwVNCount].x = x;
			}
			else
			{
				pvNormal[dwVNCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			}
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;


			tri.normal = pvNormal[dwVNCount];
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
				ComputeTriangle((pvVertexBuffer[dwFCount * 3].Pos),pvVertexBuffer[dwFCount * 3 + 1].Pos, pvVertexBuffer[dwFCount * 3 + 2].Pos, &tri);
				tri.uv0 = pvTexture[vt1 - 1];
				tri.uv1 = pvTexture[vt2 - 1];
				tri.uv2 = pvTexture[vt3 - 1];
				triangles.emplace_back(tri);




				dwFCount++;
			}


			//�@�� �ǂݍ���
			if (strcmp(key, "s") == 0)
			{
				fscanf_s(fp, "%f %f %f", &x, &y ,&z);
				SetLocalSizeX(x);
				SetLocalSizeY(y);
				SetLocalSizeZ(z);
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

		struct MeshAndTriangles data;
		data.mesh = mesh;
		data.triangles = triangles;
		data.localSize = GetLocalSize();
		//���X�g�Ƀ��f�������i�[����
		modelDatas[FileName] = data;
		SetPolygons(triangles);
		//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
		delete pvCoord;
		delete pvNormal;
		delete pvTexture;
		delete[] pvVertexBuffer;
		delete[] piFaceBuffer;

		return S_OK;
}



/// <summary>
/// �`��
/// </summary>
void Obj::Render()
{

	Light* light = Light::GetInstance();
	ShadowMap* shadowMap = ShadowMap::GetInstance();

	D3DXMATRIX View = camera->GetView();
	D3DXMATRIX Proj = camera->GetProjection();



	D3DXVECTOR3 vEyePt = camera->GetEyePos();

	////���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;
	D3DXMATRIX World;
	//�g�p����V�F�[�_�[�̓o�^	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		//���[���h�s���n��
		world = GetWorldMatrix();
		D3DXMatrixTranspose(&world, &world);
		//���[���h�A�J�����A�ˉe�s���n��
		cb.wvp = GetWVP();
		D3DXMatrixTranspose(&cb.wvp, &cb.wvp);
		//���[���h�A���C�g�r���[�A�ˉe�s���n��
		cb.wlp = world * mLight * Proj;
		D3DXMatrixTranspose(&cb.wlp, &cb.wlp);
		//���[���h�A���C�g�r���[�A�ˉe�s��A�e�N�X�`���s��
		cb.wlpt = world * mLight * Proj * clipToUV;
		D3DXMatrixTranspose(&cb.wlpt, &cb.wlpt);

		//���C�g�̕�����n��
		cb.lightDir = D3DXVECTOR4(light->GetPosition().x, light->GetPosition().y, light->GetPosition().z, 1.0f);

		//���_�ʒu��n��
		cb.eyesPos = D3DXVECTOR4(camera->GetEyePos().x,camera->GetEyePos().y,camera->GetEyePos().z,0);



		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}

	//�T���v���[��n��
	deviceContext->PSSetSamplers(0, 1, sampleLimear.GetAddressOf());
	//���f���̃e�N�X�`����n��
	deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	//�[�x�e�N�X�`����n��
	deviceContext->PSSetShaderResources(2, 1, shadowMap->GetShaderResourceView().GetAddressOf());
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf() );
	deviceContext->GSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf() );
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(vertexLayout.Get());
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
	deviceContext->GSSetConstantBuffers(0, 1, nullptr);
	deviceContext->GSSetShader(nullptr, nullptr, 0);


}

/// <summary>
/// �V���h�E�}�b�v�Ƀ��C�g�r���[���猩���[�x�l���o��
/// </summary>
void Obj::ShadowRender()
{
	auto light = Light::GetInstance();
	ShadowMap* shadowMap = ShadowMap::GetInstance();



	D3DXMATRIX View = camera->GetView();
	D3DXMATRIX Proj = camera->GetProjection();


	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	ShadowMap::ConstantBuffer cb;
	if (SUCCEEDED(deviceContext->Map(shadowMap->GetConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX lightView;
		lightView._11 = 0.9999f;
		lightView._12 = 0.0f;
		lightView._13 = 0.0f;
		lightView._14 = 0.0f;
		lightView._21 = 0.0f;
		lightView._22 = -4.3711;
		lightView._23 = -0.9999f;
		lightView._24 = 0.0f;
		lightView._31 = 0.0f;
		lightView._32 = 0.999f;
		lightView._33 = -4.37f;
		lightView._34 = 0.0f;
		lightView._41 = 12.99;
		lightView._42 = 1.44;
		lightView._43 = 32.999f;
		lightView._44 = 0.999f;

		D3DXMATRIX a_world = GetWorldMatrix();
		D3DXMATRIX a_light = light->GetLightView();
		D3DXMATRIX a_view = camera->GetView();
		D3DXMATRIX a_projection = camera->GetProjection();

		//	�r���[�s��̐���
		D3DXVECTOR3 eyepos(31,57,7.03f);
		D3DXVECTOR3 refpos(28,55,7.06f);
		D3DXVECTOR3 upvec(0,1,0);
		D3DXMatrixLookAtLH(&a_light, &eyepos, &refpos, &upvec);

		//���[���h�A���C�g�A�ˉe�s���n��
		cb.wlp = GetWorldMatrix() * light->GetLightView() * camera->GetProjection();
	//	cb.wlp = GetWorldMatrix() *  * camera->GetProjection();
		//�]�u�s��ɕϊ�
		D3DXMatrixTranspose(&cb.wlp, &cb.wlp);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ShadowMap::ConstantBuffer));
		deviceContext->Unmap(shadowMap->GetConstantBuffer().Get(), 0);

	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, shadowMap->GetConstantBuffer().GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, shadowMap->GetConstantBuffer().GetAddressOf());
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	deviceContext->IASetInputLayout(vertexLayout.Get());
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
