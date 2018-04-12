#include "Floor.h"
#include "../../../NYLibrary/Input/KeyboardUtil.h"
Floor::Floor()
	:PaintObj(true)
{
	LoadOBJFile("Resources/OBJ/floor.obj");
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	SetTag("stage");
	SetPositionY(-30.3);
	SetScale(30);
	AddComponent<TrianglePolygonListCollider>();
}

