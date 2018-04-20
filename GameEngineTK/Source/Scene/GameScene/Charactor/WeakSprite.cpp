#include "WeakSprite.h"
#include "Target.h"

const float WeakSprite::WAKE_DISTANCE = 0.4f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
WeakSprite::WeakSprite(Target* target)
	:Sprite(L"Resources/PNG/weak.png",Dimension3),
	target(target)
{
	//�r���{�[�h�ɂ���
	SetIsBillBoard(true);
}

/// <summary>
/// �X�V
/// </summary>
void WeakSprite::Update()
{
	//�G�̏�ɕ`�悷��
	D3DXVECTOR3 position = this->Getparent()._Get()->GetPosition();
	position.y += this->Getparent()._Get()->GetScaleY() * this->Getparent()._Get()->GetLocalSize().y / 2;
	SetPosition(position);
	//�\����\���؂�ւ�
	VisibleUpdate();
}

/// <summary>
/// �\����\���؂�ւ�
/// </summary>
void WeakSprite::VisibleUpdate()
{
	if (target->GetDied())
	{
		SetVisible(false);
	}
	else
	{
		SetVisible(true);
	}
}
