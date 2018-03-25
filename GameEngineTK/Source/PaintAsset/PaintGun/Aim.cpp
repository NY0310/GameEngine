#include "Aim.h"
using namespace NYLibrary;

Aim::Aim():Sprite(L"Resources/PNG/Aim.png",Sprite::Dimension2)
{
}

void Aim::Initialize()
{
	Sprite::Initialize();
	auto& devices = Devices::Get();
	Set2DPosition(D3DXVECTOR2(devices.Width() / 2.0f, devices.Height() / 2.0f));
	SetTransparency(0.5f);
}


