#pragma once
#include "../Target.h"


class Enemy : public Target
{
public:
	//�R���X�g���N�^
	Enemy();
	// �R�s�[�R���X�g���N�^�֎~
	Enemy(const Enemy&) = delete;
	// ����֎~
	Enemy& operator=(const Enemy&) = delete;
	//������
	void Initialize();
	//�X�V����
	void Update();
private:
	//���񂾂Ƃ�
	void OnDied();
	//�ړ�
	void Move();
	//�ړ�����擾����
	D3DXVECTOR3 GetMovePosition();
	//���炷�l�𗐐��ŎZ�o���n��
	float RandMovePosition();
	//�t���[���J�E���g
	int frameCnt;
	//�ړ��̃C���^�[�o��
	static const int MOVE_INTERVAL = 360;
	//�ő�ړ����a
	static const int MAX_MOVE_RADIUS = 10000;
	//�ړ�����W
	D3DXVECTOR3 endPosition;
};