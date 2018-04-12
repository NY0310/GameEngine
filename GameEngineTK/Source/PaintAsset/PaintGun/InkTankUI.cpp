#include "InkTankUI.h"

using namespace NYLibrary;

//ç¿ïWåWêî
const D3DXVECTOR3 InkTankUI::positionCoefficient = D3DXVECTOR3(0.1f,0.3f,0);


InkTankUI::InkTankUI()
	:Sprite(L"Resources/PNG/InkTank.png", Dimension::Dimension2)
{
}

/// <summary>
/// èâä˙âª
/// </summary>
void InkTankUI::Initialize()
{
	auto& devices = Devices::Get();

	SetPosition(D3DXVECTOR3(devices.Width() * positionCoefficient.x, devices.Height() * positionCoefficient.y, positionCoefficient.z));
	SetAnchorPoint(D3DXVECTOR2(0.5f, 0));
}
