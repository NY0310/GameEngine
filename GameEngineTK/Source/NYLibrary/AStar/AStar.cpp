// AStar.cpp : c��stl��A*���L�q����

#include <iostream>
#include <iomanip>
#include "MapNode.h"
#include "../Math/Math.h"
#include "AStar.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace NYLibrary;

/// <summary>
/// �R���X�g���N�^
/// </summary>
AStar::AStar()
{
}

/// <summary>
/// ������
/// </summary>
void AStar::Initialize()
{
	// [�I��]�m�[�h�𐶐�����
	endNode = new MapNode(m_attributeMap->GetEndX(), m_attributeMap->GetEndY(), AttributeMap::END);
}

/// <summary>
/// �o�H�T��
/// </summary>
void AStar::SearchRoute()
{
	// [�J�n�s]��ݒ肷��
	int row = m_attributeMap->GetStartX();
	// [�J�n��]��ݒ肷��
	int column = m_attributeMap->GetStartY();

	//�}�b�v�̐ݒ�
	SetMap();
	//�}�b�v�̑����̕\��
	PrintMapAttr();

//START:
	for (int g = 0; g < 1;)
	{
		// �B�ߐڂ�8�m�[�h�̑����𒲂ׂāA�m�[�h���I�[�v�����X�g�ɒǉ�����
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				// �e�m�[�h�̓I�[�v�����X�g�ɒǉ����Ȃ�
				if (i == 0 && j == 0)
					continue;
				// �}�b�v�z��̋��E���`�F�b�N����
				if (row + i < 0 || row + i > m_attributeMap->GetMapHight() - 1 ||
					column + j < 0 || column + j > m_attributeMap->GetMapWidth() - 1)
					continue;
				// [�X�R�A]�v�Z���ꂽ�m�[�h�̓I�[�v�����X�g�ɒǉ����Ȃ�
				if (m_map[row + i][column + j]->GetScore() > 0)
					continue;

				// �m�[�h��[�p�X]���ǂ������`�F�b�N����
				if (m_map[row + i][column + j]->GetAttribute() == AttributeMap::NONE) {
					// �m�[�h��e�ɐݒ肷��
					m_map[row + i][column + j]->SetParent(m_map[row][column]);
					// �I�[�v�����X�g�Ƀm�[�h��ǉ�����
					m_openList.push_back(m_map[row + i][column + j]);
				}
				// �m�[�h��[�I��]�ʒu���ǂ������`�F�b�N����
				if (m_map[row + i][column + j]->GetAttribute() == AttributeMap::END) {
					endNode->SetParent(m_map[row][column]);
					g++;
					//goto END;
				}
			}
		}

		// �C�X�R�A�i�R�X�g,�q���[���X�e�B�b�N�j���v�Z����
		cout << "open list" << endl;
		for (m_openListIterator = m_openList.begin(); m_openListIterator != m_openList.end(); m_openListIterator++) {
			// [�R�X�g]���v�Z����
			(*m_openListIterator)->SetCost(Math::Distance((*m_openListIterator)->GetParent()->GetRow(),
				(*m_openListIterator)->GetParent()->GetColumn(),
				(*m_openListIterator)->GetRow(),
				(*m_openListIterator)->GetColumn()));
			// [�q���[���X�e�B�b�N]���v�Z����
			(*m_openListIterator)->SetHeuristic(Math::Distance(m_attributeMap->GetEndX(),
				m_attributeMap->GetEndY(),
				(*m_openListIterator)->GetRow(),
				(*m_openListIterator)->GetColumn()));
			// [�X�R�A]���v�Z����
			(*m_openListIterator)->SetScore((*m_openListIterator)->GetCost() + (*m_openListIterator)->GetHeuristic());
			// �I�[�v�����X�g�̃m�[�h�Ōv�Z���ꂽ[�X�R�A]��\������
			cout << "m_map[" << (*m_openListIterator)->GetRow() << ", " << (*m_openListIterator)->GetColumn() << "] = " << (*m_openListIterator)->GetScore() << endl;
		}

		// �D�}�b�v�̊e�m�[�h[�X�R�A]��\������
		cout << endl << "m_map score" << endl;
		cout << "    0  1  2  3  4  5  6  7  8  9" << endl;
		for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
			cout << setw(2) << i << " ";
			for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
				// [�X�R�A]��\������
				cout << setw(2) << m_map[i][j]->GetScore() << " ";
			}
			cout << endl;
		}

		// �E�I�[�v�����X�g�̍ŏ�[�X�R�A]�̃m�[�h��T��
		int minScore = 0;
		// �ŏ�[�X�R�A]�̃m�[�h���I�[�v�����X�g�̐擪�m�[�h�ɐݒ肷��
		m_minNodeIterator = m_openList.begin();
		for (m_openListIterator = m_openList.begin(), minScore = (*m_openListIterator)->GetScore(); m_openListIterator != m_openList.end(); m_openListIterator++) {
			// �ŏ�[�X�R�A]�̃m�[�h��T��
			if ((*m_openListIterator)->GetScore() <= minScore) {
				// ��菬����[�X�R�A]���擾����
				minScore = (*m_openListIterator)->GetScore();
				// ��菬����[�X�R�A]�̃m�[�h���w�������C�e���[�^��minNodeIterator�ɃZ�b�g����
				m_minNodeIterator = m_openListIterator;
			}
		}
		// �F�ŏ�[�X�R�A]�̃m�[�h��\������
		cout << "min score node: " << "m_map[" << (*m_minNodeIterator)->GetRow() << ", " << (*m_minNodeIterator)->GetColumn() << "] = " << (*m_minNodeIterator)->GetScore() << endl;

		// �ŏ�[�X�R�A]�̃m�[�h�̍s���擾����
		row = (*m_minNodeIterator)->GetRow();
		// �ŏ�[�X�R�A]�̃m�[�h�̗���擾����
		column = (*m_minNodeIterator)->GetColumn();
		// �ŏ�[�X�R�A]�̃m�[�h���N���[�Y�h���X�g�ɒǉ�����
		m_closedList.push_back(*m_minNodeIterator);
		// �ŏ�[�X�R�A]�̃m�[�h���I�[�v�����X�g����폜����
		m_openList.erase(m_minNodeIterator);

		if (m_openList.size() == 0)
			g++;
		//	goto END;
		//goto START;
	}
//END:
	// �G�ŒZ�o�H��\������
	MapNode* node = endNode;
	cout << endl << "shortest path" << endl;
	while (node != nullptr) {
		// �ŒZ�o�H��\������
		cout << node->GetRow() << node->GetColumn() << endl;
		// ���̃m�[�h��ݒ肷��
		node = node->GetParent();
	}
	//�T�����X�g���쐬
	CreateResultPath();
}

/// <summary>
/// �������̍폜
/// </summary>
void AStar::Clear()
{
	// �H�}�b�v�̊eNode�I�u�W�F�N�g��j������
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
			if (m_map[i][j] != nullptr)
			{
				delete m_map[i][j];
				m_map[i][j] = nullptr;
			}
		}
	}
	// �}�b�v�̍s�z����N���A����
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		m_map[i].clear();
	}
	// �}�b�v�z��S�̂��N���A����
	m_map.clear();
	delete endNode;
	endNode = nullptr;
}

/// <summary>
/// �}�b�v�̐ݒ�
/// </summary>
void AStar::SetMap()
{
	// �@2�����z��}�b�v�𐶐�����
	// �}�b�v��[�s]��ݒ肷��
	m_map.resize(m_attributeMap->GetMapHight());
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		// �}�b�v[��]��ݒ肷��
		m_map[i].resize(m_attributeMap->GetMapWidth());
	}

	// 2�����z��}�b�v��Node�I�u�W�F�N�g���i�[����
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
			// �m�[�h�I�u�W�F�N�g�𐶐�����
			m_map[i][j] = new MapNode(i, j, m_attributeMap->GetAttributeMap(i, j));
			// �}�b�v��[��Q��]���Z�b�g����
			if (isWall(i,j))
				m_map[i][j]->SetScore(AttributeMap::WALL);
		}
	}
	// �}�b�v��[�J�n�s]���Z�b�g����i�v�Z���ʂ𕪂���₷�����邽�߃}�b�v�̃X�R�A�ɑ����l��ݒ肷��j
	m_map[m_attributeMap->GetStartX()][m_attributeMap->GetStartY()]->SetScore(AttributeMap::START);
	// �}�b�v��[�I���s]���Z�b�g����i�v�Z���ʂ𕪂���₷�����邽�߃}�b�v�̃X�R�A�ɑ����l��ݒ肷��j
	m_map[m_attributeMap->GetEndX()][m_attributeMap->GetEndY()]->SetScore(AttributeMap::END);
}

void AStar::PrintMapAttr()
{
	// �A�}�b�v�̑�����\������
	cout << "attribute m_map" << endl;
	cout << "    0  1  2  3  4  5  6  7  8  9" << endl;
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		cout << setw(2) << i << " ";
		for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
			// ������\������
			cout << setw(2) << m_map[i][j]->GetAttribute() << " ";
		}
		cout << endl;
	}
}

void AStar::AddOpenList()
{

}

/// <summary>
/// �T�����ʂ��擾����
/// </summary>
void AStar::CreateResultPath()
{
	MapNode* node = endNode;
	while (node != nullptr) {
		resultPath.insert(resultPath.begin(), Vector2(static_cast<float>(node->GetRow()), static_cast<float>(node->GetColumn())));
		// ���̃m�[�h��ݒ肷��
		node = node->GetParent();
	}
}

/// <summary>
/// �ǂ��ǂ���
/// </summary>
/// <param name="i">���s��</param>
/// <param name="j">�����</param>
/// <returns></returns>
bool AStar::isWall(int i, int j)
{
	if (m_map[i][j]->GetAttribute() == AttributeMap::WALL)
		return true;
	return false;
}
