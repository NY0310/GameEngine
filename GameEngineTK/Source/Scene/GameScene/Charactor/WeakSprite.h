#pragma once
#include "../../../NYLibrary/Sprite/Sprite.h"
class Target;
class WeakSprite : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	WeakSprite(Target* target);
	//�X�V
	void Update();
private:
	//�f�t�H���g�R���X�g���N�^�֎~
	WeakSprite() = default;
	//�\����\���؂�ւ�
	void VisibleUpdate();
	//�^�[�Q�b�g
	Target* target;
	//�Y��
	static const float PARENT_DISTANCE;
};
