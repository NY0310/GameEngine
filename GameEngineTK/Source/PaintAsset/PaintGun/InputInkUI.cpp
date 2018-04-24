#include "InputInkUI.h"
using namespace NYLibrary;

//ç¿ïWåWêî
const D3DXVECTOR3 InputInkUI::positionCoefficient = D3DXVECTOR3(0.1f, 0.1f, 0);


InputInkUI::InputInkUI()
	:Sprite(L"Resources/PNG/pink1.png", Dimension::Dimension2)
{
	auto& devices = Devices::Get();

	SetTransparency(0.5f);

	SetPosition(D3DXVECTOR3(devices.Width() * positionCoefficient.x, devices.Height() * positionCoefficient.y, positionCoefficient.z));
}

void InputInkUI::SetRenderTexture(InkTank::StandardColor standardColor)
{
	D3DXVECTOR4 color;
	srv[InkTank::StandardColor::red] = GetShaderResourceView();

	switch (standardColor)
	{
	case InkTank::red:
		color = static_cast<D3DXVECTOR4>(Colors::Red);
		break;
	case InkTank::green:
		color = static_cast<D3DXVECTOR4>(Colors::Green);
		break;
	case InkTank::blue:
		color = static_cast<D3DXVECTOR4>(Colors::Blue);
		break;
	case InkTank::total:
		break;
	default:
		break;
	}

	SetColor(color);

}
