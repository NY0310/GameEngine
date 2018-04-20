#include "EnemyDied.h"

EnemyDied* EnemyDied::state = nullptr;

/***************************************************************************
*|	�T�v�@	�C���X�^���X���擾����
*|	�����@�@����
*|�@�߂�l�@�G�̏��
****************************************************************************/
EnemyDied* EnemyDied::GetInstance()
{
	if (state == nullptr)
	{
		state = new EnemyDied();
	}

	return state;
}




/***************************************************************************
*|	�T�v�@�@���s����
*|	�����@�@�G
*|�@�߂�l�@����
****************************************************************************/
void EnemyDied::Execute(Enemy * enemy)
{
	if (isFirst)
	{
		//�G�ɂ����C���N���N���A
		enemy->ClearCampus();
		//�I�u�W�F�N�g��j�󂷂�
		enemy->BreakStart();
		isFirst = false;
	}

	//�j�󂳂�I�������
	if (enemy->IsBreakEnd())
	{
		isFirst = true;
	//	enemy->SetPosition(GetMovePosition());
		enemy->BreakClear();
		enemy->Revival();
		enemy->ChangeState(EnemyMove::GetInstance());
	}
}



/***************************************************************************
*|	�T�v�@�@�j������
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
void EnemyDied::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
