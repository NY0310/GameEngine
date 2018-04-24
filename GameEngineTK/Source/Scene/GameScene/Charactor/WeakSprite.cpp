#include "WeakSprite.h"
#include "Target.h"

const float WeakSprite::PARENT_DISTANCE = 0.5f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
WeakSprite::WeakSprite(Target* target)
	:Sprite(L"Resources/PNG/weak.png",Dimension3),
	target(target)
{
	//�r���{�[�h�ɂ���
	SetIsBillBoard(true);
	//�e�s��̉e�����󂯂�
	SetParantMatrixInfluence(true);
	//�e�̓��ɐݒu����
	SetPositionY(PARENT_DISTANCE);
}

/// <summary>
/// �X�V
/// </summary>
void WeakSprite::Update()
{
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
