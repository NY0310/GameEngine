#include "WeakSprite.h"


const float WeakSprite::WAKE_DISTANCE = 0.4f;


WeakSprite::WeakSprite()
	:Sprite(L"Resources/PNG/weak.png",Dimension3)
{
	//�r���{�[�h�ɂ���
	SetIsBillBoard(true);
}


void WeakSprite::Update()
{
	//�G�̏�ɕ`�悷��
	D3DXVECTOR3 position = this->Getparent()._Get()->GetPosition();
	position.y += this->Getparent()._Get()->GetScaleY() * this->Getparent()._Get()->GetLocalSize() * 4;
	SetPosition(position);
	SetScale(3);

}