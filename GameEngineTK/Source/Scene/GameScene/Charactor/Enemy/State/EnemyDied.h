#pragma once
#include "EnemyStop.h"
#include "EnemyState.h"

class EnemyDied :public EnemyState
{
public:
	//Stand�N���X�̃C���X�^���X���擾����
	static EnemyDied* GetInstance();
	//���s����
	void Execute(Enemy* enemy);
	//Stand�I�u�W�F�N�g��j������
	static void Dispose();
private:
	//���g�̃C���X�^���X
	static EnemyDied* state;
	//�ŏ���
	static bool isFirst;
};
