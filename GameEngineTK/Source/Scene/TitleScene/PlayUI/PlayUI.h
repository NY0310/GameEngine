#pragma once
#include "../../GameScene/Charactor/Target.h"


class PlayUI : public Target
{
public:
	PlayUI();
	//������
	void Initialize();
	// �R�s�[�R���X�g���N�^�֎~
	PlayUI(const PlayUI&) = delete;
	// ����֎~
	PlayUI& operator=(const PlayUI&) = delete;
private:
	void OnDied();

};