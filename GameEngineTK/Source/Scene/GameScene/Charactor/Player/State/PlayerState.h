#pragma once
#include "../MainPlayer.h"
#include "../../../../../NYLibrary/Input/KeyboardUtil.h"
#include "../../../../../NYLibrary/Input/MouseUtil.h"
#include "../../../../../NYLibrary/Device/Device.h"
#include "../../../../../NYLibrary/Math/Math.h"

//�v���C���̏��
class PlayerState {
public:
	// ���z�f�X�g���N�^
	virtual ~PlayerState() {}
	// ���s����
	virtual void Execute(Player* player) = 0;
protected:
	//�v���C���̈ړ��X�V����
	void MoveUpdate(Player* player, D3DXVECTOR3 speed);
private:
	//�v���C���̈ړ�
	void Move(Player* player, D3DXVECTOR3 speed);
	//�v���C���̉�]
	void Rotation(Player* player, D3DXVECTOR2 angle);
	//��]���x
	//static const float ROTATION;
};