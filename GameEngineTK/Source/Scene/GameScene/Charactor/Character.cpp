#include "Character.h"

using namespace std;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="maxHp">�̗͏��</param>
/// <param name="matrixObject">�I�u�W�F�N�g�s��</param>
Character::Character()
{
}



/// <summary>
/// �f�X�g���N�^
/// </summary>
Character::~Character()
{
}




void Character::CheckData()
{
	if (hp <= 0)
		OnDied();
}

