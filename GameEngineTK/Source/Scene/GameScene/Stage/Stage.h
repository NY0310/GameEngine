#pragma once
#include "../../../PaintAsset/PaintSystem/PaintObj.h"


class Stage	: public PaintObj
{
public:
	Stage();
	void Update();
	void ShadowRender();
	//void ShadowRender();
	~Stage();
private:
};

