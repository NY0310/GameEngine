#pragma once
#include "EnemyStop.h"
#include "EnemyDied.h"
#include "EnemyState.h"

class EnemyMove :public EnemyState
{
public:
	//Stand�N���X�̃C���X�^���X���擾����
	static EnemyMove* GetInstance();
	//���s����
	void Execute(Enemy* enemy);
	//Stand�I�u�W�F�N�g��j������
	static void Dispose();
private:
	//����
	static const int MOVE_TIME = 180;
	//�ړ��O���W
	D3DXVECTOR3 startPosition;
	//�ړ�����W
	D3DXVECTOR3 endPosition;
	//���g�̃C���X�^���X
	static EnemyMove* state;
};
