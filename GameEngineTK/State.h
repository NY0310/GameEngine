//	�w�b�_�t�@�C���̃C���N���[�h
#include "Obj3d.h"

//	���d�C���N���[�h�̖h�~
#pragma once

//	�N���X
class State {
public:
	// ���z�f�X�g���N�^(virtual destructor)
	virtual ~State() {}
	// �������z�֐���錾����(Declare pure virtual function)
	virtual void Execute(Obj3d* pPlayer) = 0;
};