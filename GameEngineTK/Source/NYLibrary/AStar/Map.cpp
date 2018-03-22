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
	MapObjResize(1,1);
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

void Map::MapObjResize(int hight, int windth)
{
	mapObj.resize(hight);
	for (auto& i : mapObj)
		i.resize(windth);
}


void Map::CreateMap()
{
	//�^�C���̊Ԋu
	static const LPSTR FileNames[] = { "Resources/PNG/StartPanel.png",
		"Resources/PNG/GoalPanel.png","Resources/PNG/WallPanel.png",
		"Resources/PNG/planePanel.png", "Resources/PNG/SwampPanel.png"
	};
	for (int i = 0; i < 1; i++)
		for (int j = 0; j < 1; j++)
		{
			shared_ptr<PaintObj> obj = make_shared<PaintObj>();
			obj->Initialize();
			AddChild(obj);
			D3DXVECTOR3 pos = D3DXVECTOR3(i, 0, j);
			obj->SetPosition(pos);
			obj->LoadOBJFile("Resources/OBJ/Geometry+Normal+UV.obj");
			obj->SetScale(10);
			int a = attributeMap->GetAttributeMap(i, j);
			obj->LoadTextuerFile(FileNames[attributeMap->GetAttributeMap(i, j) + 3]);
			obj->AddComponent<TrianglePolygonListCollider>();

			//�I�u�W�F�N�g����
			mapObj[i][j] = obj;
		}
}

