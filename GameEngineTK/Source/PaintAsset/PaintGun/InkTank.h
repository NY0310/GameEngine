#pragma once
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "../../NYLibrary/Input/KeyboardUtil.h"
#include "../../NYLibrary/Input/MouseUtil.h"
#include "InkTankUI.h"

class InputInkUI;

class InkTank :public NYLibrary::GameObjectNodeEmpty
{
public:
	//子供を作成し登録
	void CreateAddChild();
	//初期化　
	void Initialize();
	//更新処理
	void Update();
	//インクを出す
	void InkLost();
	//インクの色を取得する
	const D3DXVECTOR4& GetInkColor() { return inkColor; }
	//色の種類
	enum StandardColor
	{
		red,
		green,
		blue,
		total
	};
	//インクが空か
	bool IsEmptyInk() { return colorAmount[total] == 0; }
private:
	//前フレームのホイール総量
	int oldWheel;
	//インク量を計算
	void CalcTotalAmount();
	//色を計算
	void CalcColor();
	//追加する色を変更する
	void ChangeColor();
	//インクの色
	D3DXVECTOR4 inkColor;
	//インク量
	float colorAmount[total + 1];
	//追加する色
	StandardColor inColor;
	//最大インク量
	static int const MAX_INK = 50;
	//入るインクのUI
	std::shared_ptr<InputInkUI> inputInkUI;
	//インクタンクUI
	std::shared_ptr<InkTankUI> inkTankUI;
	//増やすインク量
	static const int ADD_INK = 1;
};