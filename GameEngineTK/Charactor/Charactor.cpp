#include "Charactor.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="maxHp">�̗͏��</param>
/// <param name="matrixObject">�I�u�W�F�N�g�s��</param>
Charactor::Charactor(int maxHp, MatrixObject* matrixObject)
	:MAX_HP(maxHp)
{
	this->matrixObject.reset(matrixObject);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Charactor::~Charactor()
{
	matrixObject.release();
}
