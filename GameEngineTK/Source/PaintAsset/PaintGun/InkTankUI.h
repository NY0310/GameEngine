#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"

class InkTankUI : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	InkTankUI();
	void Initialize();
	//�C���N�^���N�̗ʂƐF��ݒ肷��
	void SetInkTankData(float proportion, D3DXVECTOR4 color) { SetScaleY(proportion); SetColor(color); }
private:
};
