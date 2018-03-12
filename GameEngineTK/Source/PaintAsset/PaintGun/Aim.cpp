#include "Aim.h"
using namespace NYLibrary;

Aim::Aim():Sprite(L"Resources/PNG/Aim.png",Sprite::Dimension2)
{
}

void Aim::Initialize()
{
	Sprite::Initialize();
	auto& devices = Devices::Get();
	Set2DPosition(D3DXVECTOR2((int)devices.Width() / 2, (int)devices.Height() / 2));
	SetTransparency(0.5f);
}


