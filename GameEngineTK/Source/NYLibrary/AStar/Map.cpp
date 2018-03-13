#include "Map.h"

using namespace DirectX::SimpleMath;
using namespace NYLibrary;



void Map::Initialize()
{
	attributeMap = make_shared<AttributeMap>();
	//aStar = make_unique<AStar>();
	//aStar->SetAttributeMap(attributeMap);
	//aStar->Initialize();
	//マップデータ読み込み
	attributeMap->ReadCSV("Resources/CSV/stage.csv");
	//マップのサイズを変更する
	MapObjResize(attributeMap->GetMapHight(), attributeMap->GetMapWidth());
	//マップデータからオブジェクトを生成
	CreateMap();
	int a = 0;
}

void Map::Render()
{
	for (auto i : mapObj)
		for (auto j : i)
			j->Render();
}

void Map::MapObjResize(int hight, int windth)
{
	mapObj.resize(hight);
	for (auto& i : mapObj)
		i.resize(windth);
}


void Map::CreateMap()
{
	//タイルの間隔
	static const LPSTR FileNames[] = { "Resources/PNG/StartPanel.png",
		"Resources/PNG/GoalPanel.png","Resources/PNG/WallPanel.png",
		"Resources/PNG/planePanel.png", "Resources/PNG/SwampPanel.png"
	};
	for (int i = 0; i < attributeMap->GetMapHight(); i++)
		for (int j = 0; j < attributeMap->GetMapWidth(); j++)
		{
			shared_ptr<Obj> obj = make_shared<Obj>();
			obj->Initialize();
			AddChild(obj);
			D3DXVECTOR3 pos = D3DXVECTOR3(i, 0, j);
			obj->SetPosition(pos);
			obj->LoadOBJFile("Resources/OBJ/Panel.obj");
			obj->SetScale(0.3);
			int a = attributeMap->GetAttributeMap(i, j);
			obj->LoadTextuerFile(FileNames[attributeMap->GetAttributeMap(i, j) + 3]);
			//オブジェクト生成
			mapObj[i][j] = obj;
		}
}

