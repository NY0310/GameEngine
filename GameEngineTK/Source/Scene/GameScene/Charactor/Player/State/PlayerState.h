#pragma once
#include "../MainPlayer.h"
#include "../../../../../NYLibrary/Input/KeyboardUtil.h"
#include "../../../../../NYLibrary/Input/MouseUtil.h"
#include "../../../../../NYLibrary/Device/Device.h"
#include "../../../../../NYLibrary/Math/Math.h"

//�v���C���̏��
class PlayerState {
public:
	//�R���X�g���N�^
	PlayerState();
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
	D3DXQUATERNION Rotation(D3DXVECTOR2 angle);
	//��] Y���ő�l���N�����v����
	void Clamp(float& rotY,int max) {
		if (max >= rotY)
		{
			rotY = max;
			
		}
		else if (max * -1 >= rotY)
		{
			rotY = max * -1;
		}
	}
	//��]���x
	static const int ROTATION_COEFFICIENT;
	//�}�E�X�̑��ړ���
	D3DXVECTOR2 mouseTotalTrans;
	//�}�E�X�̏����ʒu
	D3DXVECTOR2 initialMousePosition;
	//��]��Y�̍ő�l
	static const D3DXVECTOR2 MAX_ROTATION;
};