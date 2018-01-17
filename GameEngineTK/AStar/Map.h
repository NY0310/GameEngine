#pragma once


#include <d3d11_1.h>
#include <DirectXMath.h>
#include "AttributeMap.h"
#include "../Paint/Obj.h"
#include "AStar.h"

class Map {
public:
	Map() = default;
	~Map() = default;
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
	//初期化
	void Inisialize();
	//描画
	void Render();
	//最短経路取得
	std::vector<DirectX::SimpleMath::Vector2> GetShortestPath() { aStar->SearchRoute(); return aStar->GetResultPath(); }
private:
	//マップのリストサイズ変更
	void MapObjResize(const int Hight, const int windth);
	//マップ生成
	void CreateMap();
	//配置するオブジェクト
	std::vector<std::vector<Obj*>> mapObj;
	//アトリビュートマップ
	std::shared_ptr<AttributeMap> attributeMap;
	//A*
	std::unique_ptr<AStar>aStar;
};

