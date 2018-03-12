#include "AttributeMap.h"

#include <sstream>
#include <fstream>
#include <cassert>

using namespace std;
using namespace NYLibrary;

/// <summary>
/// CSVファイルの読み込み
/// </summary>
/// <param name="filename">ファイル名</param>
void AttributeMap::ReadCSV(std::string filename)
{
	ifstream ifs(filename);
	//ファイルがないとassert
	assert(ifs);

	//行数を取得
	Get1Row(m_mapHight, ifs);
	//列数を取得
	Get1Row(m_mapWidth, ifs);

	//属性マップのサイズを変更する
	m_attributeMap.resize(m_mapHight);
	for (auto& i : m_attributeMap)
		i.resize(m_mapWidth);
	//残りの行を読み込む
	ReadRestRow(ifs);
}



/// <summary>
/// 1行のみ取得
/// </summary>
/// <param name="setNum">変更したい値</param>
/// <param name="ifs">ストリーム</param>
/// <param name="str">文字列</param>
void AttributeMap::Get1Row(int& setNum, std::ifstream &ifs)
{
	string str;
	getline(ifs, str);
	setNum = atoi(str.c_str());
}

/// <summary>
/// 残りの行を読み込む
/// </summary>
/// <param name="ifs">ストリーム</param>
/// <param name="str">文字列</param>
void AttributeMap::ReadRestRow(std::ifstream & ifs)
{

	string str;
	int i = 0;
	//csvファイルを1行ずつ読み込む
	while (getline(ifs, str))
	{
		string token;
		istringstream stream(str);

		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ','))
		{
			//スタートの位置を求める
			FindSpecificPos(START, m_startX, m_startY, i, token);
			//終了の位置を求める
			FindSpecificPos(END, m_endX, m_endY, i, token);
			//マップにcsvの情報を代入
			m_attributeMap[i / m_mapHight][i % m_mapWidth] = atoi(token.c_str());
			i++;
		}
	}
}

/// <summary>
/// 特定の位置を求める
/// </summary>
/// <param name="targetPos">求めたい場所</param>
/// <param name="posX">代入するX座標</param>
/// <param name="posY">代入するY座標</param>
/// <param name="i">何番目</param>
/// <param name="str">文字列</param>
void AttributeMap::FindSpecificPos(int targetPos, int& posX, int& posY, int i , const std::string& str)
{
	if (atoi(str.c_str()) == targetPos)
	{
		posX = i / m_mapHight;
		posY = i % m_mapWidth;
	}
}
