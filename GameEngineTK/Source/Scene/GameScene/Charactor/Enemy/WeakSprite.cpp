#include "WeakSprite.h"

WeakSprite::WeakSprite()
	:Sprite(L"Resources/PNG/weak.png",Dimension3)
{
}


void WeakSprite::Update()
{
	SetPosition(this->Getparent()._Get()->GetPosition());
}