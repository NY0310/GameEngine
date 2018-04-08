#include "Map.h"

using namespace DirectX::SimpleMath;
using namespace NYLibrary;



void Map::Initialize()
{
	attributeMap = make_shared<AttributeMap>();
	//aStar = make_unique<AStar>();
	//aStar->SetAttributeMap(attributeMap);
	//aStar->Initialize();
	//�}�b�v�f�[�^�ǂݍ���
	attributeMap->ReadCSV("Resources/CSV/stage.csv");
	//�}�b�v�̃T�C�Y��ύX����
	MapObjResize(/*attributeMap->GetMapHight()*/1, /*attributeMap->GetMapWidth()*/1);
	//�}�b�v�f�[�^����I�u�W�F�N�g�𐶐�
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
	//�^�C���̊Ԋu
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

			//�I�u�W�F�N�g����
			mapObj[i][j] = obj;
		}
}

