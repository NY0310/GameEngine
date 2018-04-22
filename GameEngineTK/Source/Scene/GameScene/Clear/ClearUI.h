#pragma once
#include <vector>
#include "../../../NYLibrary/Sprite/Sprite.h"
#include "../../../NYLibrary/Sound/ADX2Le.h"
#include "../../../Sounds/CueSheet_0.h"

class ClearUI : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	ClearUI();
	//�X�V
	void Update();
private:
	//���W�W��
	static const float POSITION_COEFFICIENT_Y;
	//����
	int frameCnt;
	//�N���A����
	static const int CLEAR_TIME = 300;
};
