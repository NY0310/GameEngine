#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"

class InkContainerUI : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	InkContainerUI();
	//初期化
	void Initialize();
private:
	//親からのずらし量	
	static const float PARENT_DISTANCE;
};
