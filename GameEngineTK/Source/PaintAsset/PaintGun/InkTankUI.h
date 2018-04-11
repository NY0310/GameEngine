#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"

class InkTankUI : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	InkTankUI();
	//インクタンクの量と色を設定する
	void SetInkTankData(float proportion, D3DXVECTOR4 color) { SetScale2D(D3DXVECTOR2(1, proportion)); SetColor(color); }
private:
};
