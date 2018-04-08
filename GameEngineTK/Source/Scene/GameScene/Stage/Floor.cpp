#include "Floor.h"
#include "../../../NYLibrary/Input/KeyboardUtil.h"
Floor::Floor()
	:PaintObj(true)
{
	LoadOBJFile("Resources/OBJ/floor.obj");
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	SetScale(3);
	SetTag("stage");
	SetLocalSize(30);
	AddComponent<TrianglePolygonListCollider>();
}

