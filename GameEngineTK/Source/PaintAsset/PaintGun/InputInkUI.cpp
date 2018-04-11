#include "InputInkUI.h"
using namespace NYLibrary;

InputInkUI::InputInkUI()
	:Sprite(L"Resources/PNG/pink1.png", Dimension::Dimension2)
{
	auto& devices = Devices::Get();
	srv[InkTank::StandardColor::red] = GetShaderResourceView();

	D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), L"Resources/PNG/green.png", nullptr, nullptr, srv[InkTank::StandardColor::green].ReleaseAndGetAddressOf(), nullptr);
	D3DX11CreateShaderResourceViewFromFile(devices.Device().Get(), L"Resources/PNG/blue.png", nullptr, nullptr, srv[InkTank::StandardColor::blue].ReleaseAndGetAddressOf(), nullptr);

	Set2DPosition(D3DXVECTOR2(devices.Width() * 0.2, devices.Height() * 0.2));
	SetTransparency(0.5f);

	//Set2DPosition(D3DXVECTOR2(devices.Height() / 3, devices.Width() / 2));
}
