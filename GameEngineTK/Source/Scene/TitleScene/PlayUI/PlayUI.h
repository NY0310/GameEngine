#pragma once
#include "../../GameScene/Charactor/Enemy/Enemy.h"


class PlayUI : public Enemy
{
public:
	PlayUI() = default;
	// �R�s�[�R���X�g���N�^�֎~
	PlayUI(const PlayUI&) = delete;
	// ����֎~
	PlayUI& operator=(const PlayUI&) = delete;
private:
	void OnDied();

};