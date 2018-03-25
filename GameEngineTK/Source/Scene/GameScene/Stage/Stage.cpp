#include "Stage.h"

Stage::Stage()
	:PaintObj(true)
{
	Obj::LoadOBJFile("Resources/OBJ/floor.obj");
	GetComponent<PlaneCollider>();
}


Stage::~Stage()
{
}

