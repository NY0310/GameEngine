#pragma once


#include <d3d11_1.h>
#include <DirectXMath.h>
#include "AttributeMap.h"
#include "../Obj/Obj.h"
#include "AStar.h"
#include "../SceneGraph/Node/GameObjectNode.h"

namespace NYLibrary
{
	class Map :public GameObjectNodeEmpty
	{
	public:
		Map() = default;
		~Map() = default;
		Map(const Map&) = delete;
		Map& operator=(const Map&) = delete;
		//初期化
		void Initialize();
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
		std::vector<std::vector<std::shared_ptr<Obj>>> mapObj;
		//アトリビュートマップ
		std::shared_ptr<AttributeMap> attributeMap;
		//A*
		std::unique_ptr<AStar>aStar;
	};
};
