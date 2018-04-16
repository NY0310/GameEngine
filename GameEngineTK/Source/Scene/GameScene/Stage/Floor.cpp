#include "Floor.h"
#include "../../../NYLibrary/Input/KeyboardUtil.h"
Floor::Floor()
	:PaintObj("Resources/OBJMODEL/floor.obj",true)
{
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	SetTag("stage");
	SetPositionY(-30.3);
	SetScale(30);
	AddComponent<TrianglePolygonListCollider>();
}

