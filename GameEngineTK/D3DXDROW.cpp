#include "D3DXDROW.h"

using namespace std;

D3DXDROW::D3DXDROW()
{
	birthcnt = 0;
}


D3DXDROW::~D3DXDROW()
{
}

HRESULT D3DXDROW::InitD3D()
{

	auto& devices = Devices::Get();


	//�V�F�[�_������
	if (FAILED(InitShader())) {
		return E_FAIL;
	}

	//���b�V���t�@�C���ǂݍ���
	m_d3dxread = new D3DXREAD();
	m_d3dxread->InitDx9();
	if (FAILED(m_d3dxread->LoadXMesh("Geometry+UV.x")))
	{
		return E_FAIL;
	}

	//���̎��_�ŁA�t�@�C��������o�������b�V���f�[�^���ADx9��D3DX���b�V���ɓ����Ă���A
	//���Ƃ́A��������D���ȏ������o����Dx11�̎��O���b�V���ɗ��p���邾���B
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	//�o�[�e�b�N�X�o�b�t�@�[���쐬

	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�o�[�e�b�N�X������o���Ȃ��̂Ń��b�N����B
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_d3dxread->GetpMesh()->GetVertexBuffer(&pVB);

	//DWORD dwStride = m_d3dxread->GetpMesh()->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MY_VERTEX *pvVertex = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pvVertex = (MY_VERTEX*)pVertices;
		//Dx9��D3DMESH�̃o�[�e�b�N�X�o�b�t�@�[����̏��ŁADx11�̃o�[�e�b�N�X�o�b�t�@���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) *m_d3dxread->GetpMesh()->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pvVertex;
		if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_Mesh.pVertexBuffer)))
			return FALSE;

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	//�C���f�b�N�X�o�b�t�@�[���쐬

	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�C���f�b�N�X�o�b�t�@�[������o���Ȃ��̂Ń��b�N����B
	int* pIndex = NULL;
	m_d3dxread->GetpMesh()->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);
	//Dx9��D3DMESH�̃C���f�b�N�X�o�b�t�@�[����̏��ŁADx11�̃C���f�b�N�X�o�b�t�@���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * m_d3dxread->GetpMesh()->GetNumFaces() * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = pIndex;
	//InitData.SysMemPitch = 2;
	//InitData.SysMemSlicePitch = 0;
	if (FAILED(devices.Device().Get()->CreateBuffer(&bd, &InitData, &m_Mesh.pIndexBuffer)))
		return FALSE;
	m_d3dxread->GetpMesh()->UnlockIndexBuffer();
	//�e�N�X�`���[�쐬
	if (m_d3dxread->m_TextureFileName[0][0] != NULL)
	{
		if (FAILED(D3DX11CreateShaderResourceViewFromFileA(devices.Device().Get(), m_d3dxread->m_TextureFileName[0], NULL, NULL, &m_pTexture, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;

}


HRESULT D3DXDROW::InitShader()
{
	auto& devices = Devices::Get();

	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
		{ "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(devices.Device().Get()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"D3DXMESH.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(devices.Device().Get()->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;

}

void D3DXDROW::Render(unique_ptr<FollowCamera>& camera)
{
	auto& devices = Devices::Get();


	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//���[���h�g�����X�t�H�[��
	static float x = 0;
	x += 0.00001;



	D3DXVECTOR3 vEyePt = shadermanager.VectorToD3DXVECTOR3(camera->m_eyepos);
	D3DXVECTOR3 vLookatPt = shadermanager.VectorToD3DXVECTOR3(camera->m_refpos);
	D3DXVECTOR3 vUpVec = shadermanager.VectorToD3DXVECTOR3(camera->m_upvec);

	D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovRH(&Proj, camera->m_fovY, camera->m_aspect, camera->m_NearClip, camera->m_FarClip);



	//�g�p����V�F�[�_�[�̓o�^
	devices.Context().Get()->VSSetShader(m_pVertexShader, NULL, 0);
	devices.Context().Get()->PSSetShader(m_pPixelShader, NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(devices.Context().Get()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX Tran, Scale, Rot;
		//���[���h�s��v�Z
		D3DXMatrixScaling(&World, 3, 3, 3);
		//World *= Scale;

		birthcnt += 10;
		////���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		D3DXMatrixRotationY(&Rot, birthcnt / 1000.0f);//�P����yaw��]������
		World *= Rot;

		D3DXMatrixTranslation(&Tran, 2, 1, 2);

		World *= Tran;

		//���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = World*View*Proj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(SIMPLESHADER_CONSTANT_BUFFER));
		devices.Context().Get()->Unmap(m_pConstantBuffer, 0);
	}
	m_pSampleLinear = NULL;
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	devices.Context().Get()->PSSetSamplers(0, 1, &m_pSampleLinear);
	devices.Context().Get()->PSSetShaderResources(0, 1, &m_pTexture);

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	devices.Context().Get()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	devices.Context().Get()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devices.Context().Get()->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devices.Context().Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	devices.Context().Get()->IASetVertexBuffers(0, 1, &m_Mesh.pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	devices.Context().Get()->IASetIndexBuffer(m_Mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�������_�����O
	devices.Context().Get()->DrawIndexed(m_d3dxread->GetpMesh()->GetNumFaces() * 3, 0, 0);

}
