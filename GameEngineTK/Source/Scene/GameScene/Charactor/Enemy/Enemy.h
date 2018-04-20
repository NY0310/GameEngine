#pragma once
#include "../Target.h"

class EnemyState;
class Enemy : public Target
{
public:
	//�R���X�g���N�^
	Enemy();
	// �R�s�[�R���X�g���N�^�֎~
	Enemy(const Enemy&) = delete;
	// ����֎~
	Enemy& operator=(const Enemy&) = delete;
	//������
	void Initialize();
	//�X�V����
	void Update();
	//��ԕω�
	void ChangeState(EnemyState * state);
	//�h��
	void Revival();
private:
	//���
	EnemyState* enemyState;
	//HP���
	static const int MAX_HP = 10;
};

#include "State/EnemyState.h"
#include "State/EnemyStop.h"
