#pragma once
#include "PlayerState.h"
#include "PlayerStop.h"
#include "PlayerWalk.h"

class PlayerRun :public PlayerState
{
public:
	//Stand�N���X�̃C���X�^���X���擾����
	static PlayerState* GetInstance();
	//���s����
	void Execute(Player* player);
	//Stand�I�u�W�F�N�g��j������
	static void Dispose();
private:
	static PlayerState* state;
	static const D3DXVECTOR3 SPEED;
};
