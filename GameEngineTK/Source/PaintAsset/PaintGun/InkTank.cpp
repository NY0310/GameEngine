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

	//�C���N���^���N�ɓ����
	if (mouse->IsPressed(MouseUtil::Right) && MAX_INK >= colorAmount[total])
	{
		colorAmount[inColor] += ADD_INK;
		//colorAmount[inColor]  = max( MAX_INK, colorAmount[inColor]);
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
		if (colorAmount[i] < 0)
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
	inkColor = D3DXVECTOR4(static_cast<float>(colorAmount[red]) / colorAmount[total], static_cast<float>(colorAmount[green]) / colorAmount[total], static_cast<float>(colorAmount[blue]) / colorAmount[total], 1);
	//�񐔉��
	Math::ValidateNan(inkColor);
}

void InkTank::ChangeColor()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	if (mouse->GetWheelValue() > oldWheel)
	{
		//���₷�C���N��
		inColor = static_cast<StandardColor>(inColor + 1);
	}

	if (mouse->GetWheelValue() < oldWheel)
	{
		inColor = static_cast<StandardColor>(inColor - 1);
	}
	//mouse->ResetWheelValue();
	inColor = static_cast<StandardColor>(Math::Clamp(2, 0, inColor));

	//inColor = static_cast<StandardColor>(wheel);
	//if (inColor != 0)
	//{
	//	int ab = 0;
	//}



	oldWheel = mouse->GetWheelValue();
}
