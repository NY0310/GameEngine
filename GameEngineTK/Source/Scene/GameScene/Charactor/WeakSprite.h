#pragma once
#include "../../../NYLibrary/Sprite/Sprite.h"
class WeakSprite : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	WeakSprite();
	void Update();
private:
	//ズレ
	static const float WAKE_DISTANCE;
};
