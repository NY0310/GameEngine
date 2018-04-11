#include "InkTankUI.h"

using namespace NYLibrary;

InkTankUI::InkTankUI()
	:Sprite(L"Resources/PNG/InkTank.png", Dimension::Dimension2)
{
	auto& devices = Devices::Get();

	Set2DPosition(D3DXVECTOR2(devices.Width() * 0.2, devices.Height() * 0.2));
}