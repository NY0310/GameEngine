#pragma once
#include <memory>

class Character 
{
public:
	//�R���X�g���N�^
	Character();
	//�f�X�g���N�^
	virtual ~Character();
	// �R�s�[�R���X�g���N�^�֎~
	Character(const Character&) = delete;
	// ����֎~
	Character& operator=(const Character&) = delete;
	//�X�V
	void CheckData();
	//�_���[�W���󂯂�
	void HitDamage() { hp--;}
	//�_���[�W���󂯂�
	void WeakHitDamage() { hp -= WEAK_DAMAGE; }
	//��
	void Recovery() { hp++; }
protected:
	//���񂾂Ƃ�
	virtual void OnDied() {};
	//�c��̗�
	int hp;
	//��_�_���[�W
	static const int WEAK_DAMAGE = 2;
};