#include "InkTankUI.h"

using namespace NYLibrary;

InkTankUI::InkTankUI()
	:Sprite(L"Resources/PNG/InkTank.png", Dimension::Dimension2)
{
}

void InkTankUI::Initialize()
{
	auto& devices = Devices::Get();

	SetPosition(D3DXVECTOR3(devices.Width() * 0.5, devices.Height() * 0.5, 0));
	SetAnchor(D3DXVECTOR2(0.5f, 0));
}
