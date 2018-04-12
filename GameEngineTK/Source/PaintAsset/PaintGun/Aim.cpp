#include "Aim.h"
using namespace NYLibrary;

Aim::Aim():Sprite(L"Resources/PNG/Aim.png",Sprite::Dimension2)
{
}


void Aim::Initialize()
{
	Sprite::Initialize();
	auto& devices = Devices::Get();
	SetPosition(D3DXVECTOR3(devices.Width() / 2.0f, devices.Height() / 2.0f,0));
	SetTransparency(0.5f);
}


void Aim::Update()
{


}

void Aim::Render()
{

}