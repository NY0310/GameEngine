#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>
#include <list>
#include <memory>

#include "Node.h"
#include "AttributeMap.h"

class AStar
{
public:
	//コンストラクタ
	AStar();
	//初期化
	void Initialize();
	//経路探索
	void SearchRoute();
	//メモリの削除
	void Clear();
	//アトリビュートマップを設定
	void SetAttributeMap(const std::shared_ptr<AttributeMap>& attributemap) { m_attributeMap = attributemap; }
	//探索結果を取得する
	const std::vector<DirectX::SimpleMath::Vector2>& GetResultPath() { return resultPath; }
private:
	//マップ
	std::vector<std::vector<Node*>> m_map;
	//オープンリスト
	std::list<Node*> m_openList;
	//オープンリストイテレータ
	std::list<Node*>::iterator m_openListIterator;
	//クローズドリスト
	std::list<Node*> m_closedList;
	//最少スコアノードイテレータ
	std::list<Node*>::iterator m_minNodeIterator;
	//アトリビュートマップ
	std::shared_ptr<AttributeMap> m_attributeMap;
	//終了ノード
	Node* endNode;
	//結果経路
	std::vector<DirectX::SimpleMath::Vector2>resultPath;
	//マップの設定
	void SetMap();
	//マップの属性の表示
	void PrintMapAttr();
	//オープンリストに追加
	void AddOpenList();
	//結果経路作成
	void CreateResultPath();

	//壁かどうか
	bool isWall(int i, int j);
};