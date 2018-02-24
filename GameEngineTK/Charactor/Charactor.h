#pragma once
#include <memory>
#include "../MatrixObject.h"

class Charactor
{
public:
	//�R���X�g���N�^
	Charactor(int maxHp, MatrixObject* matrixObject);
	Charactor() = delete;
	//�f�X�g���N�^
	virtual ~Charactor();
	// �R�s�[�R���X�g���N�^�֎~
	Charactor(const Charactor&) = delete;
	// ����֎~
	Charactor& operator=(const Charactor&) = delete;

protected:
	//�̗͏��
	const int MAX_HP;
	//�c��̗�
	int hp;
	//�s��Ǘ�
	std::unique_ptr<MatrixObject> matrixObject;
};