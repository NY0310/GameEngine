#pragma once
#include "EnemyMove.h"
#include "EnemyState.h"


class EnemyStop :public EnemyState
{
public:
	//Stand�N���X�̃C���X�^���X���擾����
	static EnemyStop* GetInstance();
	//���s����
	void Execute(Enemy* enemy);
	//Stand�I�u�W�F�N�g��j������
	static void Dispose();
private:
	static EnemyStop* state;
	//�ړ��̃C���^�[�o��
	static const int MOVE_INTERVAL = 1;
};
