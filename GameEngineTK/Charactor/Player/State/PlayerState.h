#pragma once
#include "../MainPlayer.h"
//	�N���X
class PlayerState {
public:
	// ���z�f�X�g���N�^(virtual destructor)
	virtual ~PlayerState() {}
	// �������z�֐���錾����(Declare pure virtual function)
	virtual void Execute(Player* player) = 0;
};