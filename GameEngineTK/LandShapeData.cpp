#include <fstream>
#include <algorithm>
#include "LandShapeData.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

// ���f���f�[�^�ǂݍ���
std::unique_ptr<LandShapeData> LandShapeData::CreateFromData(const char* meshData)
{
	// �ŐV�̓ǂݍ��݈ʒu�������|�C���^
	const void* head = meshData;

	std::unique_ptr<LandShapeData> landshape(new LandShapeData());

	// �w�b�_�������ǂݎ��
	const char* str = static_cast<const char*>(head);

	// �������w�b�_������
	const string header_str = "LAND_SHAPE";

	// �w�b�_�����񂪕s��v�Ȃ�A�n�`�f�[�^�t�@�C���ł͂Ȃ�
	if (strncmp(str, header_str.c_str(), header_str.length()) != 0) return nullptr;

	// �ǂݍ��݈ʒu��i�߂�
	head = str + header_str.length();

	// �m�[�h����ǂݎ��
	const UINT* p_nNode = static_cast<const UINT*>(head);

	// �m�[�h���擾
	UINT nNode = *p_nNode;
	// �ǂݍ��݈ʒu��i�߂�
	head = p_nNode + 1;

	// �m�[�h����ǂݎ��
	const NodeInfo* nodeInfo_array = static_cast<const NodeInfo*>(head);
	head = nodeInfo_array + nNode;

	int mesh = 0;
	// ���b�V���̒��_�f�[�^�ƃC���f�b�N�X�f�[�^��ǂݍ���
	landshape->m_Indices.clear();
	// �m�[�h�����~�e�m�[�h�̒��_�f�[�^���̂Q�����z��
	vector<vector<VERTEX_LANDSHAPE>>	vertex_array;
	vector<vector<USHORT>>				index_array;
	for (UINT i = 0; i < nNode; i++)
	{
		// �m�[�h���P�����擾
		const NodeInfo* nodeInfo = &nodeInfo_array[i];

		vector<VERTEX_LANDSHAPE> va;
		vector<USHORT> ia;

		vertex_array.push_back(va);
		index_array.push_back(ia);

		// ���b�V���Ȃ�
		if (nodeInfo->kind == 1)
		{
			// ���_�f�[�^����ǂݎ��
			const UINT* p_nVertex = static_cast<const UINT*>(head);
			head = p_nVertex + 1;
			// ���_�f�[�^��
			UINT nVertex = *p_nVertex;
			vertex_array[i].reserve(nVertex);
			size_t vbBytes = sizeof(VERTEX_LANDSHAPE_FILE)* nVertex;
			// ���_�f�[�^��ǂݎ��
			const VERTEX_LANDSHAPE_FILE* node_vertex_array = static_cast<const VERTEX_LANDSHAPE_FILE*>(head);
			head = node_vertex_array + nVertex;

			// �S�Ă̒��_�ɑ΂���
			for (UINT j = 0; j < nVertex; j++)
			{
				// ���_�f�[�^�P���̃A�h���X���v�Z
				const VERTEX_LANDSHAPE_FILE* node_vertex = &node_vertex_array[j];

				// �i�[���钸�_1���̃f�[�^
				VERTEX_LANDSHAPE vertex;

				// ���W���R�s�[
				vertex.Pos = node_vertex->Pos;
				// �@������U�N���A
				vertex.Normal = Vector3(0, 0, 0);

				// ���_�f�[�^��z��Ɋi�[
				vertex_array[i].push_back(vertex);
			}

			//�C���f�b�N�X�f�[�^����ǂݎ��
			const UINT* p_nIndices = static_cast<const UINT*>(head);
			head = p_nIndices + 1;
			//�C���f�b�N�X�f�[�^��
			UINT nIndices = *p_nIndices;
			index_array[i].reserve(nIndices);

			size_t ibBytes = sizeof(USHORT)* nIndices;
			// �C���f�b�N�X�f�[�^��ǂݎ��
			const USHORT* node_index_array = static_cast<const USHORT*>(head);
			head = node_index_array + nIndices;

			// �S�ẴC���f�b�N�X�ɑ΂���
			for (UINT j = 0; j < nIndices; j++)
			{
				// �C���f�b�N�X�P���̃A�h���X���擾
				const USHORT* index = &node_index_array[j];
				// �C���f�b�N�X��z��Ɋi�[
				index_array[i].push_back(*index);
			}
		}
	}

	// �S�Ă̒��_�f�[�^�����f�����W�n�ɕϊ�
	UpdateNodeMatrices(0, nodeInfo_array, nullptr, vertex_array);

	// �S�m�[�h�̒��_�f�[�^��z��Ɋi�[
	landshape->m_Vertices.clear();
	landshape->m_Indices.clear();
	int vertex_count = 0;
	for (UINT i = 0; i < nNode; i++)
	{
		vector<USHORT>::iterator it;

		// �C���f�b�N�X��S�Ẵm�[�h�̒ʂ��ԍ��ɕϊ�����
		for (it = index_array[i].begin(); it != index_array[i].end(); it++)
		{
			*it += vertex_count;
		}

		vertex_count += vertex_array[i].size();

		// �Ō���ɘA������
		landshape->m_Vertices.insert(landshape->m_Vertices.end(), vertex_array[i].begin(), vertex_array[i].end());
		landshape->m_Indices.insert(landshape->m_Indices.end(), index_array[i].begin(), index_array[i].end());
	}

	char log_str[256];

	// ���_���A�C���f�b�N�X�������O�o��
	sprintf_s(log_str, "VertexNum:%d  IndexNum:%d\n", landshape->m_Vertices.size(), landshape->m_Indices.size());
	OutputDebugStringA(log_str);

	// �C���f�b�N�X�ƒ��_����A�O�p�`�f�[�^���\�z
	{
		// ���_�C���f�b�N�X�̐����R�Ŋ����āA�O�p�`�̐����v�Z
		int nTri = landshape->m_Indices.size() / 3;

		// �S�Ă̎O�p�`�ɂ���
		for (int i = 0; i < nTri; i++)
		{
			// �O�p�`�̊e���_�̃C���f�b�N�X���擾
			int index0 = landshape->m_Indices[i * 3];
			int index1 = landshape->m_Indices[i * 3 + 1];
			int index2 = landshape->m_Indices[i * 3 + 2];
			// �e���_�̍��W���擾
			Vector3 pos0 = landshape->m_Vertices[index0].Pos;
			Vector3 pos1 = landshape->m_Vertices[index1].Pos;
			Vector3 pos2 = landshape->m_Vertices[index2].Pos;

			Triangle tri;

			// 3�_����O�p�`���\�z
			ComputeTriangle(pos0, pos1, pos2, &tri);

			landshape->m_Vertices[index0].Normal = tri.Normal;
			landshape->m_Vertices[index1].Normal = tri.Normal;
			landshape->m_Vertices[index2].Normal = tri.Normal;

			// �O�p�`�f�[�^�ɒǉ�
			landshape->m_Triangles.push_back(tri);
		}
	}


	return landshape;
}

// ���f���f�[�^�ǂݍ���
std::unique_ptr<LandShapeData> LandShapeData::CreateFromFile(const wchar_t* szFileName)
{
	ifstream ifs;
	std::unique_ptr<char[]> data;

	// �t�@�C���I�[�v��
	ifs.open(szFileName, ios::in | ios::binary);

	// �ǂݍ��ݎ��s
	assert(ifs && "CreateFromMDL Error: Failed to open MDL.");

	// �t�@�C���T�C�Y���擾
	ifs.seekg(0, fstream::end);
	streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, fstream::beg);
	streamoff begPos = ifs.tellg();
	streamoff size = eofPos - begPos;

	// �ǂݍ��ނ��߂̃��������m�ہi�G���[�`�F�b�N�͂��Ă��Ȃ��j
	data.reset(new char[(UINT)size]);

	// �t�@�C���擪����o�b�t�@�փR�s�[ 
	ifs.read(data.get(), size);

	// �t�@�C���N���[�Y
	ifs.close();

	OutputDebugStringW(szFileName);
	OutputDebugStringW(L"\n");

	// �ǂݍ��񂾃f�[�^���烂�f���f�[�^�𐶐�
	auto model = CreateFromData(data.get());

	// ���O�擾
	model->name = szFileName;

	return model;
}

//--------------------------------------------------------------------------------------
// �e�m�[�h�̒��_��e�̍s��ŕϊ�����
//--------------------------------------------------------------------------------------
void LandShapeData::UpdateNodeMatrices(int index, const NodeInfo* nodeInfo_array, Matrix* pParentMatrix, vector<vector<VERTEX_LANDSHAPE>>& vertexArray)
{
	// �m�[�h���P�����擾
	const NodeInfo* nodeInfo = &nodeInfo_array[index];
	//�X�P�[�����O
	Matrix scalem = Matrix::CreateScale(nodeInfo->scale);
	// ���f���s����쐬
	Matrix modelm;
	// ��]�s��
	Matrix rotm = Matrix::CreateFromQuaternion(nodeInfo->rotation);
	// ���s�ړ��s��
	Matrix transm = Matrix::CreateTranslation(nodeInfo->translation);

	// �s�������
	modelm *= scalem;
	modelm *= rotm;
	modelm *= transm;
	// �e�s�񂪂���΁A������
	if (pParentMatrix != nullptr)
	{
		modelm = modelm * (*pParentMatrix);
	}

	// ���b�V���Ȃ�
	if (nodeInfo->kind == 1)
	{
		vector<VERTEX_LANDSHAPE>::iterator it;

		for (it = vertexArray[index].begin(); it != vertexArray[index].end(); it++)
		{
			// ���W��ϊ��i�m�[�h���W�n�����f�����W�n�j
			it->Pos = Vector3::Transform(it->Pos, modelm);

			char str[256];

			// �ϊ���̒��_���W�����O�o��
			sprintf_s(str, "(%.3f,%.3f,%.3f)\n", it->Pos.x, it->Pos.y, it->Pos.z);

			OutputDebugStringA(str);
		}
	}

	// �Z��m�[�h�́A�����Ƌ��ʂ̐e�s��ŕϊ�
	if (nodeInfo->sibling > 0)
	{
		// �ċA�Ăяo��
		UpdateNodeMatrices(nodeInfo->sibling, nodeInfo_array, pParentMatrix, vertexArray);
	}

	// �q�m�[�h�͎�����e�s��Ƃ��ĕϊ�
	if (nodeInfo->child > 0)
	{
		// �ċA�Ăяo��
		UpdateNodeMatrices(nodeInfo->child, nodeInfo_array, &modelm, vertexArray);
	}
}