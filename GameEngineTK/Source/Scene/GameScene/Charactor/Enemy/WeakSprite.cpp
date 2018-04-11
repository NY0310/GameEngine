#include "WeakSprite.h"


const float WeakSprite::WAKE_DISTANCE = 0.4f;


WeakSprite::WeakSprite()
	:Sprite(L"Resources/PNG/weak.png",Dimension3)
{
}


void WeakSprite::Update()
{
	//敵の上に描画する
	D3DXVECTOR3 position = this->Getparent()._Get()->GetPosition();
	position.y += this->Getparent()._Get()->GetScaleY() * this->Getparent()._Get()->GetLocalSize();
	SetPosition(position);
}