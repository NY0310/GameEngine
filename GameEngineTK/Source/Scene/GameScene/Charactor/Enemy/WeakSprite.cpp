#include "WeakSprite.h"


const float WeakSprite::WAKE_DISTANCE = 0.4f;


WeakSprite::WeakSprite()
	:Sprite(L"Resources/PNG/weak.png",Dimension3)
{
	//ƒrƒ‹ƒ{[ƒh‚É‚·‚é
	SetIsBillBoard(true);
}


void WeakSprite::Update()
{
	//“G‚Ìã‚É•`‰æ‚·‚é
	D3DXVECTOR3 position = this->Getparent()._Get()->GetPosition();
	position.y += this->Getparent()._Get()->GetScaleY() * this->Getparent()._Get()->GetLocalSize() * 4;
	SetPosition(position);
	SetScale(3);

}