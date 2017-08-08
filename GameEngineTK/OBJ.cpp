#include "OBJ.h"


OBJ::OBJ()
{
}


OBJ::~OBJ()
{
}

void OBJ::Init()
{
	shadermanager = ShaderManager::Get();
	this->InitD3D();
}

HRESULT OBJ::InitD3D()
{
	auto& devices = Devices::Get();

	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(devices.Device().Get()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_Obj_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Geometry.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(devices.Device().Get()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_Obj_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_OBJpConstantBuffer)))
	{
		return E_FAIL;
	}


	//���b�V���쐬
	if (FAILED(InitStaticMesh("Geometry.obj", &m_Mesh)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT OBJ::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
{
	auto& devices = Devices::Get();

	pMesh->pIndexBuffer = NULL;
	pMesh->pVertexBuffer = NULL;
	pMesh->dwNumFace = 0;
	pMesh->dwNumVert = 0;
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	char key[500] = { 0 };
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//�܂��͒��_���A�|���S�����𒲂ׂ�
	int a = 0;
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//���_
		if (strcmp(key, "v") == 0)
		{
			a++;
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

void OBJ::Render(std::unique_ptr<FollowCamera>& camera)
{

	auto& devices = Devices::Get();


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


	//�g�p����V�F�[�_�[�̓o�^	
	devices.Context().Get()->VSSetShader(m_Obj_pVertexShader, NULL, 0);
	devices.Context().Get()->PSSetShader(m_Obj_pPixelShader, NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_OBJpConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�g�����X�t�H�[���͌X�ňقȂ�
		D3DXMATRIX Scale, Tran;
		D3DXMatrixTranslation(&Tran, 0, 0, 0);

		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMatrixScaling(&Scale, 5, 5, 5);
		World = Scale*CancelRotation*Tran;

		D3DXMATRIX m = World *View * Proj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		devices.Context().Get()->Unmap(m_OBJpConstantBuffer, 0);
	}


	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_OBJpConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_OBJpConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	devices.Context().Get()->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�������_�����O
	devices.Context().Get()->DrawIndexed(m_Mesh.dwNumFace * 3, 0, 0);


}
