#pragma once

#include <vector>
#include <memory>
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "Collision.h"


// �n�`������f�[�^
class LandShapeData
{
public:
	friend class LandShape;

	// ���_���̍\���́i�t�@�C������ǂݍ��ރf�[�^�j
	struct VERTEX_LANDSHAPE_FILE
	{
		DirectX::SimpleMath::Vector3	Pos;		// ���W
	};

	// ���_���̍\����
	struct VERTEX_LANDSHAPE
	{
		DirectX::SimpleMath::Vector3	Pos;		// ���W
		DirectX::SimpleMath::Vector3	Normal;		// �@��
	};

	// �m�[�h���
	struct NodeInfo
	{
		// ��� (0:bone 1:mesh)
		int kind;

		// ���O
		char name[32];

		// �Z��̃m�[�h�ԍ�
		int sibling;

		// �q���̃m�[�h�ԍ�
		int child;

		// ��{�p��
		DirectX::SimpleMath::Vector3 translation;
		DirectX::SimpleMath::Quaternion rotation;
		DirectX::SimpleMath::Vector3 scale;
	};

	// ���O
	std::wstring name;

	// ���_�f�[�^�z��
	std::vector<VERTEX_LANDSHAPE> m_Vertices;
	// �C���f�b�N�X�f�[�^�z��
	std::vector<USHORT> m_Indices;
	// �O�p�`�f�[�^�z��
	std::vector<Triangle> m_Triangles;

	// ���f����ǂݍ���
	static std::unique_ptr<LandShapeData> CreateFromData(const char* meshData);
	static std::unique_ptr<LandShapeData> CreateFromFile(const wchar_t* szFileName);
	// ���W�ϊ�
	static void UpdateNodeMatrices(int index, const NodeInfo* nodeInfo_array, DirectX::SimpleMath::Matrix* pParentMatrix, std::vector<std::vector<VERTEX_LANDSHAPE>>& vertexArray);
};