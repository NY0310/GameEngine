#pragma once
#include "../../../PaintAsset/PaintSystem/PaintObj.h"
#include "Floor.h"

class Stage	: public PaintObj
{
public:
	Stage();
	void CreateAddChild();
	void Update();
	void ShadowRender();
	void Finalize();
	//void ShadowRender();
	~Stage();
private:
	std::shared_ptr<Floor> floor;
};

