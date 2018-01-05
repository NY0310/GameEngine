#pragma once

class Node {
public:
	// コンストラクタ
	Node(int row, int column, int attr)
		: m_row(row), m_column(column), m_attr(attr), m_cost(0), m_heuristic(0),  m_score(0), m_parent(nullptr) {
	}
	// 行を取得する
	int  GetRow() { return this->m_row; }
	// 行を設定する
	void SetRow(int row) { this->m_row = row; }
	// 列を取得する
	int  GetColumn() { return this->m_column; }
	// 列を設定する
	void SetColumn(int column) { this->m_column = column; }
	// 属性を取得する
	int  GetAttribute() { return m_attr; }
	// 属性を設定する
	void SetAttribute(int attr) { this->m_attr = attr; }
	// コストを取得する
	int  GetCost() { return this->m_cost; }
	// コストを設定する
	void SetCost(int cost) { this->m_cost = cost; }
	// ヒューリスティックを取得する
	int  GetHeuristic() { return this->m_heuristic; }
	// ヒューリスティックを設定する
	void SetHeuristic(int heuristic) { this->m_heuristic = heuristic; }
	// スコアを取得する
	int  GetScore() { return this->m_score; }
	// スコアを設定する
	void SetScore(int score) { this->m_score = score; }
	// 親を取得する
	Node* GetParent() { return this->m_parent; }
	// 親を設定する
	void SetParent(Node* parent) { this->m_parent = parent; }
private:
	// 行
	int m_row;
	// 列
	int m_column;
	// 属性
	int m_attr;
	// コスト
	int m_cost;
	// ヒューリスティック
	int m_heuristic;
	// スコア
	int m_score;
	// 親Nodeへのポインタ
	Node* m_parent;
};