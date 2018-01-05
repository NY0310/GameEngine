#pragma once

#include <vector>


class AttributeMap
{
public:
	using TiledMap = std::vector<std::vector<int>>;
	//属性
	enum Attribute
	{
		START = -3,	//スタート
		END ,       //ゴール
		WALL,       //壁
		NONE,       //何もなし
		SWAMP		//沼
	};
	//CSVファイルの読み込み
	void ReadCSV(std::string filename);
	//描画
#pragma region Getter
	//属性マップ
	const int GetAttributeMap(int i, int j) const { return m_attributeMap[i][j]; }
	//マップの高さ
	const int GetMapHight    () const { return m_mapHight; }
	//マップの幅
	const int GetMapWidth    () const { return m_mapWidth; }
	//スタート位置のX座標
	const int GetStartX      () const { return m_startX  ; }
	//スタート位置のY座標
	const int GetStartY      () const { return m_startY  ; }
	//ゴール位置のX座標
	const int GetEndX        () const { return m_endX    ; }
	//ゴール位置のY座標
	const int GetEndY        () const { return m_endY    ; }
#pragma endregion

private:
	// 属性マップ
	TiledMap m_attributeMap;
	//マップの高さ
	int m_mapHight;
	//マップの幅
	int m_mapWidth;
	//スタート位置のX座標
	int m_startX;
	//スタート位置のY座標
	int m_startY;
	//ゴール位置のX座標
	int m_endX;
	//ゴール位置のX座標
	int m_endY;

	//1行のみ取得
	void Get1Row(int& setNum, std::ifstream &ifs);
	//残りの行を読み込む
	void ReadRestRow(std::ifstream &ifs);
	//特定の位置を求める
	void FindSpecificPos(int targetPos, int& posX, int& posY, int i ,const std::string& str);
};