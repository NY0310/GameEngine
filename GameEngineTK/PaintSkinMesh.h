#pragma once
#include "SkinMesh.h"
#include "Input/MouseRay.h"
#include "Paint/PaintCollision.h"
#include "Paint/Paint.h"


class PaintSkinMesh : public SkinMesh
{
public:
	PaintSkinMesh();
	~PaintSkinMesh();
	HRESULT Initialize(bool isPlane = false);
	void UpDate();
	void Render();
	//ƒCƒ“ƒN•`‰æ
	void InkRender() { campus->Render(); }

private:
	std::unique_ptr<Paint>campus;
	std::unique_ptr<PaintCollision>paintCollision;
	MouseRay* mouseRay;

};

