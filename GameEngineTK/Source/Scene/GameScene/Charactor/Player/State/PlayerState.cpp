#include "PlayerState.h"

using namespace NYLibrary;

const float PlayerState::ROTATION = 1.0f;

void PlayerState::MoveUpdate(Player * player, D3DXVECTOR3 speed)
{
	//�L�[�{�[�h
	KeyboardUtil* keyBoard = KeyboardUtil::GetInstance();
	//�ړ����x
	D3DXVECTOR3 saveSpeed(0,0,0);
	//��]��
	D3DXVECTOR2 savaRot(0, 0);
	//�O�i
	if (keyBoard->IsPressed(DirectX::Keyboard::W))
		saveSpeed.z = -speed.z;
	//���
	if (keyBoard->IsPressed(DirectX::Keyboard::S))
		saveSpeed.z = speed.z;
	//������
	if (keyBoard->IsPressed(DirectX::Keyboard::A))
		rot.x -= ROTATION;
	//�E����
	if (keyBoard->IsPressed(DirectX::Keyboard::D))
		rot.x += ROTATION;

	//�ړ�������
	if (saveSpeed.x != 0 || saveSpeed.z != 0)
	{
		Move(player, saveSpeed);
	}

	//��]��
	int width = static_cast<int>(Devices::Get().Width());
	int hight = static_cast<int>(Devices::Get().Height());
	MouseUtil* mouse = MouseUtil::GetInstance();
	DirectX::XMINT2 pos = mouse->GetPos();


	float x = static_cast<float>(pos.x) / width;
	float y = static_cast<float>(pos.y) / hight;
	D3DXVECTOR2 mouseRot = Math::ChangeRegularDevice(D3DXVECTOR2(x,y));
	//mouseRot.x *= -1;
	mouseRot *= 90.0f;

	player->SetQuaternion(Rotation(mouseRot) * Rotation(D3DXVECTOR2(rot.x,rot.y)));
}

/// <summary>
/// �v���C���̈ړ�
/// </summary>
/// <param name="player">�v���C��</param>
/// <param name="speed">�ړ����x</param>
void PlayerState::Move(Player * player, D3DXVECTOR3 speed)
{
	//	�ړ��x�N�g��w
	D3DXVECTOR3 moveV(speed);

	D3DXMATRIX moveRot;
	D3DXMatrixRotationY(&moveRot, player->GetRotationY());
	//	�ړ��x�N�g�������@�̊p�x����]������
	D3DXVec3TransformNormal(&moveV, &moveV,&moveRot);
	//	���@�̍��W���ړ�
	player->SetPosition(player->GetPosition() + moveV);

}

/// <summary>
/// �v���C���̉�]
/// </summary>
/// <param name="player">�v���C��</param>
/// <param name="angle">��]��</param>
D3DXQUATERNION PlayerState::Rotation(D3DXVECTOR2 angle)
{
	D3DXQUATERNION q(0, 0, 0, 1);
	D3DXQUATERNION q2(0, 0, 0, 1);
	D3DXVECTOR3 NAxis(0, 1, 0);
	D3DXVECTOR3 NAxis2(1, 0, 0);

	D3DXQuaternionRotationAxis(&q, &NAxis, D3DXToRadian(angle.x));
	D3DXQuaternionRotationAxis(&q2, &NAxis2, D3DXToRadian(angle.y));
	return  q * q2;
}
