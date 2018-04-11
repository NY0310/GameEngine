#include "InkTank.h"
#include "InputInkUI.h"

using namespace NYLibrary;


/// <summary>
/// 子を生成し登録
/// </summary>
void InkTank::CreateAddChild()
{
	inputInkUI = make_shared<InputInkUI>();
	AddChild(inputInkUI);
	inkTankUI = make_shared<InkTankUI>();
	AddChild(inkTankUI);
}

/// <summary>
/// タンクのインクをリセット
/// </summary>
void InkTank::Initialize()
{
	for (int i = 0; i < total; i++)
	{
		colorAmount[i] = 0;
	}
}


/// <summary>
/// タンククラス
/// </summary>

void InkTank::Update()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	auto key = KeyboardUtil::GetInstance();
	//押されたキーによりタンクに入れるインクを決定する
	if (key->IsPressed(Keyboard::Z))
	{
		inColor = red;
	}
	if (key->IsPressed(Keyboard::X))
	{
		inColor = green;
	}
	if (key->IsPressed(Keyboard::C))
	{
		inColor = blue;
	}

	//インクをタンクに入れる
	if (mouse->IsPressed(MouseUtil::Right) && MAX_INK >= colorAmount[total])
	{
		colorAmount[inColor] += ADD_INK;
		//colorAmount[inColor]  = max( MAX_INK, colorAmount[inColor]);
	}


	//タンクから排出されるインクを計算する
	CalcColor();
	//RGBのインクの合計(タンクのインク量)を計算
	CalcTotalAmount();
	//インクに入れる色のUIを設定
	inputInkUI->SetRenderTexture(inColor);
	ChangeColor();
	//インクタンクのデータを設定		
	inkTankUI->SetInkTankData(static_cast<float>(colorAmount[total]) / MAX_INK,inkColor);
}


/// <summary>
/// インクを出す
/// </summary>
void InkTank::InkLost()
{
	for (int i = red; i < total; i++)
	{
		if (colorAmount[i] == 0)
		{
			continue;
		}
		const float LOST_COEFFICIENT = 0.3f;
		float coefficient  = static_cast<float>(colorAmount[i]) / colorAmount[total];
		colorAmount[i] -= coefficient * LOST_COEFFICIENT;
		if (colorAmount[i] < 0)
		{
			colorAmount[i] = 0;
		}
	}
}

/// <summary>
/// //RGBのインクの合計(タンクのインク量)を計算
/// </summary>
void InkTank::CalcTotalAmount()
{
	colorAmount[total] = 0;
	for (int i = 0; i < total; i++)
	{
		colorAmount[total] += colorAmount[i];
	}
}

/// <summary>
/// タンクに入っているRGBのインクの割合から色を決定
/// </summary>
void InkTank::CalcColor()
{
	inkColor = D3DXVECTOR4(static_cast<float>(colorAmount[red]) / colorAmount[total], static_cast<float>(colorAmount[green]) / colorAmount[total], static_cast<float>(colorAmount[blue]) / colorAmount[total], 1);
	//非数回避
	Math::ValidateNan(inkColor);
}

void InkTank::ChangeColor()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	if (mouse->GetWheelValue() > oldWheel)
	{
		//増やすインク量
		inColor = static_cast<StandardColor>(inColor + 1);
	}

	if (mouse->GetWheelValue() < oldWheel)
	{
		inColor = static_cast<StandardColor>(inColor - 1);
	}
	//mouse->ResetWheelValue();
	inColor = static_cast<StandardColor>(Math::Clamp(2, 0, inColor));

	//inColor = static_cast<StandardColor>(wheel);
	//if (inColor != 0)
	//{
	//	int ab = 0;
	//}



	oldWheel = mouse->GetWheelValue();
}
