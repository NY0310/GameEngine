#include "ClearUI.h"
using namespace NYLibrary;

//ç¿ïWåWêî
const D3DXVECTOR3 ClearUI::positionCoefficient = D3DXVECTOR3(0.1f, 0.1f, 0);


ClearUI::ClearUI()
	:Sprite(L"Resources/PNG/clear.png", Dimension::Dimension2)
{
	auto& devices = Devices::Get();

	//SetPosition(D3DXVECTOR3(devices.Width() * positionCoefficient.x, devices.Height() * positionCoefficient.y, positionCoefficient.z));
}
