// AStar.cpp : cとstlでA*を記述する

#include <iostream>
#include <iomanip>
#include "MapNode.h"
#include "../Math/Math.h"
#include "AStar.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace NYLibrary;

/// <summary>
/// コンストラクタ
/// </summary>
AStar::AStar()
{
}

/// <summary>
/// 初期化
/// </summary>
void AStar::Initialize()
{
	// [終了]ノードを生成する
	endNode = new MapNode(m_attributeMap->GetEndX(), m_attributeMap->GetEndY(), AttributeMap::END);
}

/// <summary>
/// 経路探索
/// </summary>
void AStar::SearchRoute()
{
	// [開始行]を設定する
	int row = m_attributeMap->GetStartX();
	// [開始列]を設定する
	int column = m_attributeMap->GetStartY();

	//マップの設定
	SetMap();
	//マップの属性の表示
	PrintMapAttr();

//START:
	for (int g = 0; g < 1;)
	{
		// ③近接の8ノードの属性を調べて、ノードをオープンリストに追加する
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				// 親ノードはオープンリストに追加しない
				if (i == 0 && j == 0)
					continue;
				// マップ配列の境界をチェックする
				if (row + i < 0 || row + i > m_attributeMap->GetMapHight() - 1 ||
					column + j < 0 || column + j > m_attributeMap->GetMapWidth() - 1)
					continue;
				// [スコア]計算されたノードはオープンリストに追加しない
				if (m_map[row + i][column + j]->GetScore() > 0)
					continue;

				// ノードが[パス]かどうかをチェックする
				if (m_map[row + i][column + j]->GetAttribute() == AttributeMap::NONE) {
					// ノードを親に設定する
					m_map[row + i][column + j]->SetParent(m_map[row][column]);
					// オープンリストにノードを追加する
					m_openList.push_back(m_map[row + i][column + j]);
				}
				// ノードが[終了]位置かどうかをチェックする
				if (m_map[row + i][column + j]->GetAttribute() == AttributeMap::END) {
					endNode->SetParent(m_map[row][column]);
					g++;
					//goto END;
				}
			}
		}

		// ④スコア（コスト,ヒューリスティック）を計算する
		cout << "open list" << endl;
		for (m_openListIterator = m_openList.begin(); m_openListIterator != m_openList.end(); m_openListIterator++) {
			// [コスト]を計算する
			(*m_openListIterator)->SetCost(Math::Distance((*m_openListIterator)->GetParent()->GetRow(),
				(*m_openListIterator)->GetParent()->GetColumn(),
				(*m_openListIterator)->GetRow(),
				(*m_openListIterator)->GetColumn()));
			// [ヒューリスティック]を計算する
			(*m_openListIterator)->SetHeuristic(Math::Distance(m_attributeMap->GetEndX(),
				m_attributeMap->GetEndY(),
				(*m_openListIterator)->GetRow(),
				(*m_openListIterator)->GetColumn()));
			// [スコア]を計算する
			(*m_openListIterator)->SetScore((*m_openListIterator)->GetCost() + (*m_openListIterator)->GetHeuristic());
			// オープンリストのノードで計算された[スコア]を表示する
			cout << "m_map[" << (*m_openListIterator)->GetRow() << ", " << (*m_openListIterator)->GetColumn() << "] = " << (*m_openListIterator)->GetScore() << endl;
		}

		// ⑤マップの各ノード[スコア]を表示する
		cout << endl << "m_map score" << endl;
		cout << "    0  1  2  3  4  5  6  7  8  9" << endl;
		for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
			cout << setw(2) << i << " ";
			for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
				// [スコア]を表示する
				cout << setw(2) << m_map[i][j]->GetScore() << " ";
			}
			cout << endl;
		}

		// ⑥オープンリストの最小[スコア]のノードを探す
		int minScore = 0;
		// 最小[スコア]のノードをオープンリストの先頭ノードに設定する
		m_minNodeIterator = m_openList.begin();
		for (m_openListIterator = m_openList.begin(), minScore = (*m_openListIterator)->GetScore(); m_openListIterator != m_openList.end(); m_openListIterator++) {
			// 最小[スコア]のノードを探す
			if ((*m_openListIterator)->GetScore() <= minScore) {
				// より小さな[スコア]を取得する
				minScore = (*m_openListIterator)->GetScore();
				// より小さな[スコア]のノードを指し示すイテレータをminNodeIteratorにセットする
				m_minNodeIterator = m_openListIterator;
			}
		}
		// ⑦最小[スコア]のノードを表示する
		cout << "min score node: " << "m_map[" << (*m_minNodeIterator)->GetRow() << ", " << (*m_minNodeIterator)->GetColumn() << "] = " << (*m_minNodeIterator)->GetScore() << endl;

		// 最小[スコア]のノードの行を取得する
		row = (*m_minNodeIterator)->GetRow();
		// 最小[スコア]のノードの列を取得する
		column = (*m_minNodeIterator)->GetColumn();
		// 最小[スコア]のノードをクローズドリストに追加する
		m_closedList.push_back(*m_minNodeIterator);
		// 最小[スコア]のノードをオープンリストから削除する
		m_openList.erase(m_minNodeIterator);

		if (m_openList.size() == 0)
			g++;
		//	goto END;
		//goto START;
	}
//END:
	// ⑧最短経路を表示する
	MapNode* node = endNode;
	cout << endl << "shortest path" << endl;
	while (node != nullptr) {
		// 最短経路を表示する
		cout << node->GetRow() << node->GetColumn() << endl;
		// 次のノードを設定する
		node = node->GetParent();
	}
	//探索リストを作成
	CreateResultPath();
}

/// <summary>
/// メモリの削除
/// </summary>
void AStar::Clear()
{
	// ⑨マップの各Nodeオブジェクトを破棄する
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
			if (m_map[i][j] != nullptr)
			{
				delete m_map[i][j];
				m_map[i][j] = nullptr;
			}
		}
	}
	// マップの行配列をクリアする
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		m_map[i].clear();
	}
	// マップ配列全体をクリアする
	m_map.clear();
	delete endNode;
	endNode = nullptr;
}

/// <summary>
/// マップの設定
/// </summary>
void AStar::SetMap()
{
	// ①2次元配列マップを生成する
	// マップの[行]を設定する
	m_map.resize(m_attributeMap->GetMapHight());
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		// マップ[列]を設定する
		m_map[i].resize(m_attributeMap->GetMapWidth());
	}

	// 2次元配列マップにNodeオブジェクトを格納する
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
			// ノードオブジェクトを生成する
			m_map[i][j] = new MapNode(i, j, m_attributeMap->GetAttributeMap(i, j));
			// マップに[障害物]をセットする
			if (isWall(i,j))
				m_map[i][j]->SetScore(AttributeMap::WALL);
		}
	}
	// マップに[開始行]をセットする（計算結果を分かりやすくするためマップのスコアに属性値を設定する）
	m_map[m_attributeMap->GetStartX()][m_attributeMap->GetStartY()]->SetScore(AttributeMap::START);
	// マップに[終了行]をセットする（計算結果を分かりやすくするためマップのスコアに属性値を設定する）
	m_map[m_attributeMap->GetEndX()][m_attributeMap->GetEndY()]->SetScore(AttributeMap::END);
}

void AStar::PrintMapAttr()
{
	// ②マップの属性を表示する
	cout << "attribute m_map" << endl;
	cout << "    0  1  2  3  4  5  6  7  8  9" << endl;
	for (int i = 0; i < m_attributeMap->GetMapHight(); i++) {
		cout << setw(2) << i << " ";
		for (int j = 0; j < m_attributeMap->GetMapWidth(); j++) {
			// 属性を表示する
			cout << setw(2) << m_map[i][j]->GetAttribute() << " ";
		}
		cout << endl;
	}
}

void AStar::AddOpenList()
{

}

/// <summary>
/// 探索結果を取得する
/// </summary>
void AStar::CreateResultPath()
{
	MapNode* node = endNode;
	while (node != nullptr) {
		resultPath.insert(resultPath.begin(), Vector2(static_cast<float>(node->GetRow()), static_cast<float>(node->GetColumn())));
		// 次のノードを設定する
		node = node->GetParent();
	}
}

/// <summary>
/// 壁かどうか
/// </summary>
/// <param name="i">何行目</param>
/// <param name="j">何列目</param>
/// <returns></returns>
bool AStar::isWall(int i, int j)
{
	if (m_map[i][j]->GetAttribute() == AttributeMap::WALL)
		return true;
	return false;
}
