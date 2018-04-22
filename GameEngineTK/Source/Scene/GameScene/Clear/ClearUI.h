#pragma once
#include <vector>
#include "../../../NYLibrary/Sprite/Sprite.h"
#include "../../../NYLibrary/Sound/ADX2Le.h"
#include "../../../Sounds/CueSheet_0.h"

class ClearUI : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	ClearUI();
	//更新
	void Update();
private:
	//座標係数
	static const float POSITION_COEFFICIENT_Y;
	//時間
	int frameCnt;
	//クリア時間
	static const int CLEAR_TIME = 300;
};
