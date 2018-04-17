#pragma once
#include "EnemyStop.h"
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
	//�ړ�����擾����
	D3DXVECTOR3 GetMovePosition();
	//���炷�l�𗐐��ŎZ�o���n��
	float RandMovePosition();

	//�ő�ړ����a
	static const int MAX_MOVE_RADIUS = 10000;
	//����
	static const int MOVE_TIME = 300;
	//�ړ��O���W
	D3DXVECTOR3 startPosition;
	//�ړ�����W
	D3DXVECTOR3 endPosition;
	//���g�̃C���X�^���X
	static EnemyMove* state;
};
