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
	/// <summary>
	/// �ړ���̍��W���擾����
	/// </summary>
	/// <returns>�ړ���̍��W���擾</returns>
	D3DXVECTOR3 EnemyState::GetMovePosition()
	{
		D3DXVECTOR3 position;
		position.x = RandMovePosition();
		position.y = 0;
		position.z = RandMovePosition();
		return position;
	}
private:
	//�ő�ړ����a
	static const int MAX_MOVE_RADIUS = 10000;

	/// <summary>
	/// ���炷�l�𗐐��ŎZ�o���n��
	/// </summary>
	/// <returns>���炷�l</returns>
	float EnemyState::RandMovePosition()
	{
		int radius = MAX_MOVE_RADIUS;
		float shift = static_cast<float>(rand() % MAX_MOVE_RADIUS) / 1000;
		shift /= 2;
		if (rand() % 2)
			shift *= -1;
		return shift;
	}

};