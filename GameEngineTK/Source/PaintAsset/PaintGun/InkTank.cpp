#include "InkTank.h"
#include "InputInkUI.h"

using namespace NYLibrary;


/// <summary>
/// �q�𐶐����o�^
/// </summary>
void InkTank::CreateAddChild()
{
	inputInkUI = make_shared<InputInkUI>();
	AddChild(inputInkUI);
	inkTankUI = make_shared<InkTankUI>();
	AddChild(inkTankUI);
}

/// <summary>
/// �^���N�̃C���N�����Z�b�g
/// </summary>
void InkTank::Initialize()
{
	for (int i = 0; i < total; i++)
	{
		colorAmount[i] = 0;
	}
}


/// <summary>
/// �^���N�N���X
/// </summary>

void InkTank::Update()
{

	MouseUtil* mouse = MouseUtil::GetInstance();
	ADX2Le* adx2 = ADX2Le::GetInstance();


	auto key = KeyboardUtil::GetInstance();
	//�����ꂽ�L�[�ɂ��^���N�ɓ����C���N�����肷��
	if (key->IsPressed(Keyboard::Z))
	{
		inColor = red;
	}
	if (key->IsPressed(Keyboard::X))
	{
		inColor = green;
	}
	if (key->IsPressed(Keyboard::C))
	{
		inColor = blue;
	}
	if (key->IsPressed(Keyboard::R))
	{
		TankReset();
		adx2->Play(CRI_CUESHEET_0_LOST_0);
	}

	//�C���N���^���N�ɓ����
	if (mouse->IsPressed(MouseUtil::Right) && MAX_INK >= colorAmount[total])
	{
		colorAmount[inColor] += ADD_INK;

		if (!adx2->IsPauseByID(CRI_CUESHEET_0_CHAGE))
		{
			adx2->Play(CRI_CUESHEET_0_CHAGE);
		}

	}


	//�^���N����r�o�����C���N���v�Z����
	CalcColor();
	//RGB�̃C���N�̍��v(�^���N�̃C���N��)���v�Z
	CalcTotalAmount();
	//�C���N�ɓ����F��UI��ݒ�
	inputInkUI->SetRenderTexture(inColor);
	ChangeColor();
	//�C���N�^���N�̃f�[�^��ݒ�		
	inkTankUI->SetInkTankData(static_cast<float>(colorAmount[total]) / MAX_INK,inkColor);
}


/// <summary>
/// �C���N���o��
/// </summary>
void InkTank::InkLost()
{
	for (int i = red; i < total; i++)
	{
		if (colorAmount[i] == 0)
		{
			continue;
		}
		const float LOST_COEFFICIENT = 0.3f;
		float coefficient  = static_cast<float>(colorAmount[i]) / colorAmount[total];
		colorAmount[i] -= coefficient * LOST_COEFFICIENT;
		if (colorAmount[i] < 0.3f)
		{
			colorAmount[i] = 0;
		}
	}
}

/// <summary>
/// //RGB�̃C���N�̍��v(�^���N�̃C���N��)���v�Z
/// </summary>
void InkTank::CalcTotalAmount()
{
	colorAmount[total] = 0;
	for (int i = 0; i < total; i++)
	{
		colorAmount[total] += colorAmount[i];
	}
}

/// <summary>
/// �^���N�ɓ����Ă���RGB�̃C���N�̊�������F������
/// </summary>
void InkTank::CalcColor()
{
	inkColor = Math::Clamp(1,0,D3DXVECTOR4(colorAmount[red] , colorAmount[green] , colorAmount[blue], 1));

	//�񐔉��
	Math::ValidateNan(inkColor);
}

/// <summary>
/// �����C���N�̐F��ω�������
/// </summary>
void InkTank::ChangeColor()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	const int ONE_WHEEL_VALUE = 120;
	if (mouse->GetWheelValue() > oldWheel + ONE_WHEEL_VALUE)
	{
		//���₷�C���N��
		inColor = static_cast<StandardColor>(inColor + 1);
		oldWheel = oldWheel + ONE_WHEEL_VALUE * 2;
	}

	if (mouse->GetWheelValue() < oldWheel - ONE_WHEEL_VALUE)
	{
		inColor = static_cast<StandardColor>(inColor - 1);
		oldWheel = oldWheel - ONE_WHEEL_VALUE * 2;
	}

	inColor = static_cast<StandardColor>(Math::Clamp(blue, red, inColor));
	
}


/// <summary>
/// �^���N�����Z�b�g
/// </summary>
void InkTank::TankReset()
{
	for (int i = red; i < total; i++)
	{
		colorAmount[i] = 0;
	}
}