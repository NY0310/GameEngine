#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"
#include "InkContainerUI.h"

class InkTankUI : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	InkTankUI();
	//オブジェクトを生成し子供として登録
	void CreateAddChild();
	//初期化
	void Initialize();
	//インクタンクの量と色を設定する
	void SetInkTankData(float proportion, D3DXVECTOR4 color) { SetScaleY(proportion); SetColor(color); }
private:
	//座標係数
	static const D3DXVECTOR3 positionCoefficient;
	//インクの容器のスプライト	
	std::shared_ptr<InkContainerUI> inkContainerUI;
};
