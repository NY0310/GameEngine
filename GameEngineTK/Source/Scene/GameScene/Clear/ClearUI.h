#pragma once
#include <vector>
#include "../../../NYLibrary/Sprite/Sprite.h"

class ClearUI : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	ClearUI();
private:
	//座標係数
	static const D3DXVECTOR3 positionCoefficient;
};
