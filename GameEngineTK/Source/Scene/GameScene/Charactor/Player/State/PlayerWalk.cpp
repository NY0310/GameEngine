#include "PlayerWalk.h"
//pPlayerState�ÓI�ϐ�������������
PlayerState* PlayerWalk::state = nullptr;
const D3DXVECTOR3 PlayerWalk::SPEED = D3DXVECTOR3(0.1f,0,0.5f);

/***************************************************************************
*|	�T�v�@	Advance�N���X�̃C���X�^���X���擾����
*|	�����@�@����
*|�@�߂�l�@�v���C���̏��
****************************************************************************/
PlayerState* PlayerWalk::GetInstance()
{
	if (state == nullptr)
	{
		state = new PlayerWalk();
	}

	return state;
}




/***************************************************************************
*|	�T�v�@�@����
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
void PlayerWalk::Execute(Player * player)
{
	//�v���C���̈ړ�
	MoveUpdate(player, SPEED);

}



/***************************************************************************
*|	�T�v�@�@�j������
*|	�����@�@����
*|�@�߂�l�@����
****************************************************************************/
void PlayerWalk::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
