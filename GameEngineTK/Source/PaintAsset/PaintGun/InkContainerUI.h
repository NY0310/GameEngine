#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"

class InkContainerUI : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	InkContainerUI();
	//������
	void Initialize();
private:
	//�e����̂��炵��	
	static const float PARENT_DISTANCE;
};
