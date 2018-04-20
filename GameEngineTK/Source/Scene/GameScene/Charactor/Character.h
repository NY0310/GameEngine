#pragma once
#include <memory>

class Character 
{
public:
	//�R���X�g���N�^
	Character() = default;
	//�f�X�g���N�^
	virtual ~Character() = default;
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
	//���񂾂��擾
	bool GetDied() { return isDied; }
protected:
	//���񂾂Ƃ�
	virtual void OnDied() {}
	//�c��̗�
	int hp;
	//���񂾂�
	bool isDied;
private:
	//��_�_���[�W
	static const int WEAK_DAMAGE = 2;
};