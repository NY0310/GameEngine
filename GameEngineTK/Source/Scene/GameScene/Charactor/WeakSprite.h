#pragma once
#include "../../../NYLibrary/Sprite/Sprite.h"
class WeakSprite : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	WeakSprite();
	void Update();
private:
	//�Y��
	static const float WAKE_DISTANCE;
};
