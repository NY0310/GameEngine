#pragma once
#include "../Charactor.h"

class Player : public Charactor
{
public:
	//�R���X�g���N�^
	Player(int maxHp, MatrixObject* matrixObject);
	//�f�X�g���N�^
	~Player() = default;
	// �R�s�[�R���X�g���N�^�֎~
	Player(const Player&) = delete;
	// ����֎~
	Player& operator=(const Player&) = delete;
private:

};

