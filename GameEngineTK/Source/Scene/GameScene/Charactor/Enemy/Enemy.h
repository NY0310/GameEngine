#pragma once
#include "../Target.h"


class Enemy : public Target
{
public:
	Enemy();
	// �R�s�[�R���X�g���N�^�֎~
	Enemy(const Enemy&) = delete;
	// ����֎~
	Enemy& operator=(const Enemy&) = delete;
	//������
	void Initialize();
private:
	void OnDied();

};