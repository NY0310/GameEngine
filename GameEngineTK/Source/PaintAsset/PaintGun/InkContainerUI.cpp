#include "InkContainerUI.h"

using namespace NYLibrary;


//親からのずらし量	
const float InkContainerUI::PARENT_DISTANCE = 100.0f;

InkContainerUI::InkContainerUI()
	:Sprite(L"Resources/PNG/InkContainerUI.png", Dimension::Dimension2)
{
}

/// <summary>
/// 初期化
/// </summary>
void InkContainerUI::Initialize()
{
	Sprite::Initialize();
	auto& devices = Devices::Get();
	D3DXVECTOR3 parentPosition = Getparent()._Get()->GetPosition();
//	parentPosition.y += PARENT_DISTANCE;
	SetAnchorPoint(D3DXVECTOR2(0.5f, 0));
	SetPosition(parentPosition);
	SetScale(1.2f);
}
