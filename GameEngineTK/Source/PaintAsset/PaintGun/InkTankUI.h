#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"

class InkTankUI : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	InkTankUI();
	//�C���N�^���N�̗ʂƐF��ݒ肷��
	void SetInkTankData(float proportion, D3DXVECTOR4 color) { SetScale2D(D3DXVECTOR2(1, proportion)); SetColor(color); }
private:
};
