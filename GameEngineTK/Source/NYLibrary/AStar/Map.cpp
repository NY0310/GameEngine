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
	MapObjResize(/*attributeMap->GetMapHight()*/1, /*attributeMap->GetMapWidth()*/1);
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


void Map::ShadowRender()
{
	static float flame = 0;
	flame++;
	for (auto i : mapObj)
		for (auto j : i)
		{
			j->ShadowRender();
			//j->SetRotationX(D3DXToRadian(flame / 10));
		}
}

void Map::MapObjResize(int hight, int windth)
{
	mapObj.resize(hight);
	for (auto& i : mapObj)
		i.resize(windth);
}

void Map::ClearRenderConfig() {
	for (auto i : mapObj)
		for (auto j : i)
		{
			j->ClearRenderConfig();
		}
}

void Map::CreateMap()
{
	//タイルの間隔
	static const LPSTR FileNames[] = { "Resources/PNG/StartPanel.png",
		"Resources/PNG/GoalPanel.png","Resources/PNG/WallPanel.png",
		"Resources/PNG/planePanel.png", "Resources/PNG/SwampPanel.png"
	};
	for (int i = 0; i < /*attributeMap->GetMapHight()*/1; i++)
		for (int j = 0; j < /*attributeMap->GetMapWidth()*/1; j++)
		{
			shared_ptr<PaintObj> obj = make_shared<PaintObj>(true);
			obj->Initialize();
			AddChild(obj);
			obj->CreateAddChild();
			D3DXVECTOR3 pos = D3DXVECTOR3(static_cast<float>(i), 0, static_cast<float>(j));
			obj->SetPosition(pos);
			//obj->LoadOBJFile("Resources/OBJ/Geometry+Normal+UV.obj");
			obj->LoadOBJFile("Resources/OBJ/Enemy.obj");

			int a = attributeMap->GetAttributeMap(i, j);
			obj->LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
			obj->SetTag("stage");
			obj->SetScale(500);
			obj->AddComponent<TrianglePolygonListCollider>();

			//オブジェクト生成
			mapObj[i][j] = obj;
		}
}

