#pragma once

class Node {
public:
	// �R���X�g���N�^
	Node(int row, int column, int attr)
		: m_row(row), m_column(column), m_attr(attr), m_cost(0), m_heuristic(0),  m_score(0), m_parent(nullptr) {
	}
	// �s���擾����
	int  GetRow() { return this->m_row; }
	// �s��ݒ肷��
	void SetRow(int row) { this->m_row = row; }
	// ����擾����
	int  GetColumn() { return this->m_column; }
	// ���ݒ肷��
	void SetColumn(int column) { this->m_column = column; }
	// �������擾����
	int  GetAttribute() { return m_attr; }
	// ������ݒ肷��
	void SetAttribute(int attr) { this->m_attr = attr; }
	// �R�X�g���擾����
	int  GetCost() { return this->m_cost; }
	// �R�X�g��ݒ肷��
	void SetCost(int cost) { this->m_cost = cost; }
	// �q���[���X�e�B�b�N���擾����
	int  GetHeuristic() { return this->m_heuristic; }
	// �q���[���X�e�B�b�N��ݒ肷��
	void SetHeuristic(int heuristic) { this->m_heuristic = heuristic; }
	// �X�R�A���擾����
	int  GetScore() { return this->m_score; }
	// �X�R�A��ݒ肷��
	void SetScore(int score) { this->m_score = score; }
	// �e���擾����
	Node* GetParent() { return this->m_parent; }
	// �e��ݒ肷��
	void SetParent(Node* parent) { this->m_parent = parent; }
private:
	// �s
	int m_row;
	// ��
	int m_column;
	// ����
	int m_attr;
	// �R�X�g
	int m_cost;
	// �q���[���X�e�B�b�N
	int m_heuristic;
	// �X�R�A
	int m_score;
	// �eNode�ւ̃|�C���^
	Node* m_parent;
};