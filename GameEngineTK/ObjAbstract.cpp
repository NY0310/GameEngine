//#include "ObjAbstract.h"
//
//
//ObjAbstract::ObjAbstract()
//{
//}
//
//
//ObjAbstract::~ObjAbstract()
//{
//}
//
//
//
//HRESULT ObjAbstract::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMarial)
//{
//
//	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
//	FILE* fp = nullptr;
//	fopen_s(&fp, FileName, "rt");
//
//	char key[110] = { 0 };
//	while (!feof(fp))
//	{
//		D3DXVECTOR4 v(0, 0, 0, 0);
//		//�L�[���[�h�ǂݍ���
//		fscanf_s(fp, "%s", key, sizeof(key));
//		//�}���e���A����
//		if (strcmp(key, "newmtl") == 0)
//		{
//			fscanf_s(fp, "%s", key, sizeof(key));
//			strcpy_s(m_Material.szName, key);
//		}
//		//Ka �A���r�G���g
//		if (strcmp(key, "Ka") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
//			m_Material.Ka = v;
//		}
//		//Kb �f�B�q���[�Y
//		if (strcmp(key, "Kd") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
//			m_Material.Kd = v;
//		}
//		//Ks �X�y�L�����[
//		if (strcmp(key, "Ks") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
//			m_Material.Ks = v;
//		}
//		//map_Kd�@�e�N�X�`���[
//		if (strcmp(key, "map_Kd") == 0)
//		{
//			fscanf_s(fp, "%s", &m_Material.szTextureName, sizeof(m_Material.szTextureName));
//			//�e�N�X�`���[�쐬
//			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, m_Material.szTextureName, nullptr, nullptr, &m_pTexture, nullptr)))
//			{
//				return E_FAIL;
//			}
//		}
//
//	}
//	fclose(fp);
//
//	return S_OK;
//}
//
//
//
//
//
//HRESULT ObjAbstract::InitStaticMesh(LPSTR FileName, MY_MESH * pMesh)
//{
//
//	float x = 0, y = 0, z = 0;
//	int v1 = 0, v2 = 0, v3 = 0;
//	int vn1 = 0, vn2 = 0, vn3 = 0;
//	int vt1 = 0, vt2 = 0, vt3 = 0;
//	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
//	DWORD dwVNCount = 0;//�ǂݍ��݃J�E���^�[
//	DWORD dwVTCount = 0;//�ǂݍ��݃J�E���^�[
//	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[
//	pMesh->dwNumVert = 0;
//	pMesh->dwNumFace = 0;
//
//
//	char key[200] = { 0 };
//	//OBJ�t�@�C�����J���ē��e��ǂݍ���
//	FILE* fp = NULL;
//	fopen_s(&fp, FileName, "rt");
//
//	//���O�ɒ��_���A�|���S�����𒲂ׂ�
//	while (!feof(fp))
//	{
//		//�L�[���[�h�ǂݍ���
//		fscanf_s(fp, "%s ", key, sizeof(key));
//		//�}�e���A���ǂݍ���
//		if (strcmp(key, "mtllib") == 0)
//		{
//			fscanf_s(fp, "%s ", key, sizeof(key));
//			LoadMaterialFromFile(key, &m_Material);
//		}
//		//���_
//		if (strcmp(key, "v") == 0)
//		{
//			pMesh->dwNumVert++;
//		}
//		//�@��
//		if (strcmp(key, "vn") == 0)
//		{
//			dwVNCount++;
//		}
//		//�e�N�X�`���[���W
//		if (strcmp(key, "vt") == 0)
//		{
//			dwVTCount++;
//		}
//		//�t�F�C�X�i�|���S���j
//		if (strcmp(key, "f") == 0)
//		{
//			pMesh->dwNumFace++;
//		}
//	}
//
//	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
//	SimpleVertex* pvVertexBuffer = new SimpleVertex[pMesh->dwNumFace * 3];
//	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
//	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
//	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];
//	int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
//
//													  //�{�ǂݍ���	
//	fseek(fp, SEEK_SET, 0);
//	dwVCount = 0;
//	dwVTCount = 0;
//	dwVNCount = 0;
//	dwFCount = 0;
//
//	SimpleVertex test[6000];
//
//	while (!feof(fp))
//	{
//		//�L�[���[�h �ǂݍ���
//		ZeroMemory(key, sizeof(key));
//		fscanf_s(fp, "%s ", key, sizeof(key));
//
//		//���_ �ǂݍ���
//		if (strcmp(key, "v") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &x, &y, &z);
//			pvCoord[dwVCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
//			pvCoord[dwVCount].y = y;
//			pvCoord[dwVCount].z = z;
//
//			dwVCount++;
//		}
//		//�@�� �ǂݍ���
//		if (strcmp(key, "vn") == 0)
//		{
//			fscanf_s(fp, "%f %f %f", &x, &y, &z);
//			pvNormal[dwVNCount].x = -x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
//			pvNormal[dwVNCount].y = y;
//			pvNormal[dwVNCount].z = z;
//			dwVNCount++;
//		}
//
//
//		//�e�N�X�`���[���W �ǂݍ���
//		if (strcmp(key, "vt") == 0)
//		{
//			fscanf_s(fp, "%f %f", &x, &y);
//			pvTexture[dwVTCount].x = x;
//			pvTexture[dwVTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
//			dwVTCount++;
//		}
//		//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
//		if (strcmp(key, "f") == 0)
//		{
//			//fscanf_s(fp, "%d// %d// %d//", &v1, &v2, &v3);
//			fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
//			piFaceBuffer[dwFCount * 3] = dwFCount * 3;
//			piFaceBuffer[dwFCount * 3 + 1] = dwFCount * 3 + 1;
//			piFaceBuffer[dwFCount * 3 + 2] = dwFCount * 3 + 2;
//			//���_�\���̂ɑ��
//			pvVertexBuffer[dwFCount * 3].Pos = pvCoord[v1 - 1];
//			pvVertexBuffer[dwFCount * 3].Normal = pvNormal[vn1 - 1];
//			pvVertexBuffer[dwFCount * 3].Tex = pvTexture[vt1 - 1];
//			pvVertexBuffer[dwFCount * 3 + 1].Pos = pvCoord[v2 - 1];
//			pvVertexBuffer[dwFCount * 3 + 1].Normal = pvNormal[vn2 - 1];
//			pvVertexBuffer[dwFCount * 3 + 1].Tex = pvTexture[vt2 - 1];
//			pvVertexBuffer[dwFCount * 3 + 2].Pos = pvCoord[v3 - 1];
//			pvVertexBuffer[dwFCount * 3 + 2].Normal = pvNormal[vn3 - 1];
//			pvVertexBuffer[dwFCount * 3 + 2].Tex = pvTexture[vt3 - 1];
//
//			test[dwFCount * 3 + 0] = pvVertexBuffer[dwFCount * 3 + 0];
//			test[dwFCount * 3 + 1] = pvVertexBuffer[dwFCount * 3 + 1];
//			test[dwFCount * 3 + 2] = pvVertexBuffer[dwFCount * 3 + 2];
//
//			dwFCount++;
//		}
//	}
//
//	fclose(fp);
//
//	//�o�[�e�b�N�X�o�b�t�@�[���쐬
//	D3D11_BUFFER_DESC bd;
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(SimpleVertex) *pMesh->dwNumFace * 3;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.MiscFlags = 0;
//	D3D11_SUBRESOURCE_DATA InitData;
//	InitData.pSysMem = pvVertexBuffer;
//	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
//		return FALSE;
//
//	//�C���f�b�N�X�o�b�t�@�[���쐬
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(int) * pMesh->dwNumFace * 3;
//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.MiscFlags = 0;
//	InitData.pSysMem = piFaceBuffer;
//	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pIndexBuffer)))
//		return FALSE;
//
//	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
//	delete pvCoord;
//	delete pvNormal;
//	delete pvTexture;
//	delete[] pvVertexBuffer;
//	delete[] piFaceBuffer;
//
//
//
//	return S_OK;
//}
