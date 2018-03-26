#include "Stage.h"

Stage::Stage()
	:PaintObj(true)
{
	LoadOBJFile("Resources/OBJ/floor.obj");
	//LoadOBJFile("Resources/OBJ/Geometry+Normal+UV.obj");
	LoadTextuerFile("Resources/PNG/GoalPanel.png");
	//SetScale(D3DXVECTOR3(0.1, 0.1, 0.1));
	SetTag("stage");
	SetLocalSize(10);
	AddComponent<PlaneCollider>();
}


Stage::~Stage()
{
}

