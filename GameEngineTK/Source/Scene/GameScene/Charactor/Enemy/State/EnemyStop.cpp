#include "EnemyStop.h"
#include "EnemyDied.h"

EnemyStop* EnemyStop::state = nullptr;

/***************************************************************************
*|	�T�v�@	�C���X�^���X���擾����
*|	�����@�@����
*|�@�߂�l�@�v���C���̏��
****************************************************************************/
EnemyStop* EnemyStop::GetInstance()
{
	if (state == nullptr)
	{
		state = new EnemyStop();
	}

	return state;
}




/***************************************************************************
*|	�T�v�@�@���s����
*|	�����@�@�G
*|�@�߂�l�@����
****************************************************************************/
void EnemyStop::Execute(Enemy * enemy)
{
	//���񂾂Ƃ�		
	if (enemy->GetDied())
	{
		frameCnt = 0;
		enemy->ChangeState(EnemyDied::GetInstance());
	}
	

	if (MOVE_INTERVAL <= frameCnt)
	{
		frameCnt = 0;
		enemy->ChangeState(EnemyMove::GetInstance());
	}


	//��~���͓����蔻����s�Ȃ�
	enemy->SetComponentActive(true);

	frameCnt++;
}



/***************************************************************************
*|	�T�v�@�@�j������
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
void EnemyStop::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
