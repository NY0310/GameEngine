#include "ClearUI.h"
using namespace NYLibrary;

//���W�W��
const float ClearUI::POSITION_COEFFICIENT_Y = 0.2f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
ClearUI::ClearUI()
	:Sprite(L"Resources/PNG/clear.png", Dimension::Dimension2)
{
	auto& devices = Devices::Get();
	frameCnt = 0;
	SetPosition(D3DXVECTOR3(static_cast<float>(devices.Width() / 2), static_cast<float>(devices.Height() * POSITION_COEFFICIENT_Y), 0));
	
	ADX2Le* adx2 = ADX2Le::GetInstance();
	adx2->Stop();
	adx2->Play(CRI_CUESHEET_0_CLEAR_0);
	SetScale(1.3f);
}

/// <summary>
/// �X�V
/// </summary>
void ClearUI::Update()
{
	if (CLEAR_TIME < frameCnt)
	{
		ADX2Le* adx2 = ADX2Le::GetInstance();
		adx2->Stop(CRI_CUESHEET_0_CLEAR_0);
		RunWithScene(TitleScene);
	}
	frameCnt++;
}
