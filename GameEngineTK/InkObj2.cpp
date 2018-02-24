#include "InkObj2.h"


using namespace std;
std::map < LPSTR, InkObj2::MeshAndTriangles> InkObj2::modelDatas;

InkObj2::InkObj2()
{
	// �����o�ϐ�������
	matrixObject->SetPosition(D3DXVECTOR3(0, 0, 0));
	matrixObject->SetScale(D3DXVECTOR3(7, 7, 7));
	matrixObject->SetRotation(D3DXVECTOR3(0, 0, 0));
}


void InkObj2::Initialize()
{
	InitD3D();
}



HRESULT InkObj2::CreateShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/InkObj.hlsl", "VS", "vs_5_0", (void**)vertexShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;
	CreateVertexInputLayout(pCompiledShader);
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(ShaderManager::MakeShader("Resources/HLSL/InkObj.hlsl", "PS", "ps_5_0", (void**)pixelShader.ReleaseAndGetAddressOf(), &pCompiledShader)))return E_FAIL;

	return S_OK;
}


HRESULT InkObj2::CreateVertexInputLayout(ID3DBlob *compiledshader)
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(device->CreateInputLayout(layout, numElements, compiledshader->GetBufferPointer(), compiledshader->GetBufferSize(), &vertexLayout)))
	{
		return FALSE;
	}

	return S_OK;

}

ID3D11Buffer* InkObj2::CreateConstantBuffer(UINT size)
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



HRESULT InkObj2::InitD3D()
{
	//�V�F�[�_�[�쐬
	CreateShader();
	//�R���X�^���g�o�b�t�@�쐬
	constantBuffer = CreateConstantBuffer(sizeof(InkData));
	return S_OK;
}





HRESULT InkObj2::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{
	////���ɓǂݍ���ł��邩
	//if (modelDatas.count(FileName) != 0)
	//{
	//	mesh = modelDatas[FileName].mesh;
	//}

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
		////�}�e���A���ǂݍ���
		//if (strcmp(key, "mtllib") == 0)
		//{
		//	fscanf_s(fp, "%s ", key, sizeof(key));
		//	LoadMaterialFromFile(key, &material);
		//}
		//���_
		if (strcmp(key, "v") == 0)
		{
			pMesh->dwNumVert++;
		}
		////�@��
		//if (strcmp(key, "vn") == 0)
		//{
		//	dwVNCount++;
		//}
		////�e�N�X�`���[���W
		//if (strcmp(key, "vt") == 0)
		//{
		//	dwVTCount++;
		//}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			pMesh->dwNumFace++;
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	D3DXVECTOR3* pvVertexBuffer = new D3DXVECTOR3[pMesh->dwNumFace * 3];
	//Vector3* pvCoord = new Vector3[pMesh->dwNumVert];
	//Vector3* pvNormal = new Vector3[dwVNCount];
	//Vector2* pvTexture = new Vector2[dwVTCount];
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
			pvVertexBuffer[dwVCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			pvVertexBuffer[dwVCount].y = y;
			pvVertexBuffer[dwVCount].z = z;

			dwVCount++;
		}
		////�@�� �ǂݍ���
		//if (strcmp(key, "vn") == 0)
		//{
		//	fscanf_s(fp, "%f %f %f", &x, &y, &z);
		//	pvNormal[dwVNCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
		//	pvNormal[dwVNCount].y = y;
		//	pvNormal[dwVNCount].z = z;
		//	dwVNCount++;
		//}


		////�e�N�X�`���[���W �ǂݍ���
		//if (strcmp(key, "vt") == 0)
		//{
		//	fscanf_s(fp, "%f %f", &x, &y);
		//	pvTexture[dwVTCount].x = x;
		//	pvTexture[dwVTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
		//	dwVTCount++;
		//}
		//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
		if (strcmp(key, "f") == 0)
		{
			fscanf_s(fp, "%d %d %d", &v1, &v2, &v3);
			piFaceBuffer[dwFCount * 3] = v1 - 1;
			piFaceBuffer[dwFCount * 3 + 1] = v2 - 1;
			piFaceBuffer[dwFCount * 3 + 2] = v3 - 1;
			//pvVertexBuffer[dwFCount * 3].pos = pvCoord[v1 - 1];
			//pvVertexBuffer[dwFCount * 3].normal = pvNormal[vn1 - 1];
			//pvVertexBuffer[dwFCount * 3].uv = pvTexture[vt1 - 1];
			//pvVertexBuffer[dwFCount * 3 + 1].pos = pvCoord[v2 - 1];
			//pvVertexBuffer[dwFCount * 3 + 1].normal = pvNormal[vn2 - 1];
			//pvVertexBuffer[dwFCount * 3 + 1].uv = pvTexture[vt2 - 1];
			//pvVertexBuffer[dwFCount * 3 + 2].pos = pvCoord[v3 - 1];
			//pvVertexBuffer[dwFCount * 3 + 2].normal = pvNormal[vn3 - 1];
			//pvVertexBuffer[dwFCount * 3 + 2].uv = pvTexture[vt3 - 1];
			dwFCount++;
		}
	}

	fclose(fp);

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(D3DXVECTOR3) *pMesh->dwNumFace * 3;
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
	//���X�g�Ƀ��f�������i�[����
	modelDatas[FileName] = data;

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	//delete pvCoord;
	//delete pvNormal;
	//delete pvTexture;
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return S_OK;
}




void InkObj2::Render(vector<InkData> inkDataList)
{
	//�g�p����V�F�[�_�[�̓o�^	
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
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

	for (auto data : inkDataList)
	{

		////���[���h�g�����X�t�H�[��
		static float x = 0;
		x += 0.00001;
		D3DXMATRIX World;


		Render(data);
	}

}

void InkObj2::Render(InkData inkData)
{

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	InkData cb;
	if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		cb.wvp = inkData.wvp.Transpose();

		cb.color = inkData.color;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(InkData));
		deviceContext->Unmap(constantBuffer.Get(), 0);

	}
	//�v���~�e�B�u�������_�����O
	deviceContext->DrawIndexed(mesh.dwNumFace * 3, 0, 0);

}
