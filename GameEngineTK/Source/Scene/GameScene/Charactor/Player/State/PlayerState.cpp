#include "PlayerState.h"
#include <Winuser.h>

using namespace NYLibrary;

const int PlayerState::ROTATION_COEFFICIENT = 35;

PlayerState::PlayerState()
{
	initialMousePosition.x = Devices::Get().Width();
	initialMousePosition.y = Devices::Get().Height();
}

void PlayerState::MoveUpdate(Player * player, D3DXVECTOR3 speed)
{
	//�f�o�C�X
	auto& devices = Devices::Get();
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
		saveSpeed.x -= speed.x;
	//�E����
	if (keyBoard->IsPressed(DirectX::Keyboard::D))
		saveSpeed.x += speed.x;

	//�ړ�������
	if (saveSpeed.x != 0 || saveSpeed.z != 0)
	{
		Move(player, saveSpeed);
	}


	//�}�E�X�J�[�\���̍��W���擾����
	LPPOINT  mousePos = new POINT();
	GetCursorPos(mousePos);

	//���t���[���ł̃}�E�X�̈ړ���
	D3DXVECTOR2 mouseTrans;
	mouseTrans.x = static_cast<float>(mousePos->x) - initialMousePosition.x;
	mouseTrans.y = static_cast<float>(mousePos->y) - initialMousePosition.y;

	//���̂܂܂̐��l�ŃN�H�[�^�j�I�����쐬����Ɖ�]��������̂ŌW���ŏ��Z
	mouseTotalTrans += mouseTrans / ROTATION_COEFFICIENT;
	//�N�H�[�^�j�I�����쐬���v���C���̂���]������
	player->SetQuaternion(Rotation(D3DXVECTOR2(mouseTotalTrans.x, -mouseTotalTrans.y)));


	delete mousePos;
	mousePos = nullptr;
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

	//	�ړ��x�N�g�������@�̊p�x����]������
	D3DXVec3TransformNormal(&moveV, &moveV,&player->GetRotationMatrix());
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
