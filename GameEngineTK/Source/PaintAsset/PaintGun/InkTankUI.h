#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"

class InkTankUI : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	InkTankUI();
	void Initialize();
	//インクタンクの量と色を設定する
	void SetInkTankData(float proportion, D3DXVECTOR4 color) { SetScaleY(proportion); SetColor(color); }
private:
};
