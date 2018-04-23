#include "EnemyDied.h"

EnemyDied* EnemyDied::state = nullptr;
//�ŏ���
bool EnemyDied::isFirst = true;


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
	isFirst = true;
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
		//�I�u�W�F�N�g��j�󂷂�
		enemy->BreakStart();
		//��~���͓����蔻����s��Ȃ�
		enemy->SetComponentActive(false);
		isFirst = false;
	}


	//�j�󂳂�I�������
	if (enemy->IsBreakEnd())
	{
		isFirst = true;
		//�G�ɂ����C���N���N���A
		enemy->ClearCampus();
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
