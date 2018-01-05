#include "Map.h"

using namespace DirectX::SimpleMath;



void Map::Inisialize()
{
	attributeMap = make_shared<AttributeMap>();
	aStar = make_unique<AStar>();
	aStar->SetAttributeMap(attributeMap);
	aStar->Initialize();
	//�}�b�v�f�[�^�ǂݍ���
	attributeMap->ReadCSV("Resources/CSV/stage.csv");
	//�}�b�v�̃T�C�Y��ύX����
	MapObjResize(attributeMap->GetMapHight(), attributeMap->GetMapWidth());
	//�}�b�v�f�[�^����I�u�W�F�N�g�𐶐�
	CreateMap();
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
	for (int i = 0; i < attributeMap->GetMapHight(); i++)
		for (int j = 0; j < attributeMap->GetMapWidth(); j++)
		{
			Obj* obj = new Obj();
			obj->Init();
			Vector3 pos = Vector3(i, 0, j);
			obj->SetPosition(pos);
			obj->LoadOBJFile("Resources/OBJ/Panel.obj");
			int a = attributeMap->GetAttributeMap(i, j);
			obj->LoadTextuerFile(FileNames[attributeMap->GetAttributeMap(i, j) + 3]);
			//�I�u�W�F�N�g����
			mapObj[i][j] = obj;
		}
}

