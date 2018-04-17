#pragma once
#include "../Enemy.h"


//�v���C���̏��
class EnemyState {
public:
	//�R���X�g���N�^
	EnemyState() { frameCnt = 0; isFirst = true; };
	// ���z�f�X�g���N�^
	virtual ~EnemyState() {}
	// ���s����
	virtual void Execute(Enemy* enemy) = 0;
protected:
	//�t���[���J�E���g
	int frameCnt;
	//�ŏ���
	bool isFirst;
};