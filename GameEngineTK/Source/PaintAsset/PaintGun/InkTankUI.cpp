#include "InkTankUI.h"

using namespace NYLibrary;

//���W�W��
const D3DXVECTOR3 InkTankUI::positionCoefficient = D3DXVECTOR3(0.1f,0.3f,0);


InkTankUI::InkTankUI()
	:Sprite(L"Resources/PNG/InkTank.png", Dimension::Dimension2)
{
}

/// <summary>
/// �I�u�W�F�N�g�𐶐����q���Ƃ��ēo�^
/// </summary>
void InkTankUI::CreateAddChild()
{
	inkContainerUI = make_shared<InkContainerUI>();
	AddChild(inkContainerUI);
}

/// <summary>
/// ������
/// </summary>
void InkTankUI::Initialize()
{
	Sprite::Initialize();
	auto& devices = Devices::Get();
	SetPosition(D3DXVECTOR3(devices.Width() * positionCoefficient.x, devices.Height() * positionCoefficient.y, positionCoefficient.z));
	SetAnchorPoint(D3DXVECTOR2(0.5f, 0));

}
