#include "EnemyMove.h"

EnemyMove* EnemyMove::state = nullptr;

/***************************************************************************
*|	�T�v�@	�C���X�^���X���擾����
*|	�����@�@����
*|�@�߂�l�@�G�̏��
****************************************************************************/
EnemyMove* EnemyMove::GetInstance()
{
	if (state == nullptr)
	{
		state = new EnemyMove();
	}

	return state;
}




/***************************************************************************
*|	�T�v�@�@���s����
*|	�����@�@�G
*|�@�߂�l�@����
****************************************************************************/
void EnemyMove::Execute(Enemy * enemy)
{ 
	if (isFirst)
	{
		startPosition = enemy->GetPosition();
		endPosition = GetMovePosition();
		frameCnt = 0;
		isFirst = false;
	}

	float time = static_cast<float>(frameCnt) / MOVE_TIME;
	float breaktime = time;
	if (breaktime <= 0.5f)
	{
		breaktime *= 2;
	}
	else
	{
		breaktime -= 1.0f;
		breaktime *= -2.0f;
	}


	enemy->SetBreakTime(breaktime);
	//���`��Ԃňړ�������
	enemy->SetPosition(Math::Lerp(startPosition, endPosition, time));
	frameCnt++;
	if (time > 1)
	{
		enemy->ChangeState(EnemyStop::GetInstance());
		frameCnt = 0;
		isFirst = true;
	}



}

/// <summary>
/// �ړ���̍��W���擾����
/// </summary>
/// <returns>�ړ���̍��W���擾</returns>
D3DXVECTOR3 EnemyMove::GetMovePosition()
{
	D3DXVECTOR3 position;
	position.x = RandMovePosition();
	position.y = 0;
	position.z = RandMovePosition();
	return position;
}


/// <summary>
/// ���炷�l�𗐐��ŎZ�o���n��
/// </summary>
/// <returns>���炷�l</returns>
float EnemyMove::RandMovePosition()
{
	float shift = static_cast<float>(rand() % MAX_MOVE_RADIUS) / 1000;
	shift /= 2;
	if (rand() % 2)
		shift *= -1;
	return shift;
}


/***************************************************************************
*|	�T�v�@�@�j������
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
void EnemyMove::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
