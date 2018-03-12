#pragma once
#include <memory>

class Character
{
public:
	//�R���X�g���N�^
	Character(int maxHp);
	//�f�X�g���N�^
	virtual ~Character();
	// �R�s�[�R���X�g���N�^�֎~
	Character(const Character&) = delete;
	// ����֎~
	Character& operator=(const Character&) = delete;
protected:
	//�̗͏��
	const int MAX_HP;
	//�c��̗�
	int hp;
};