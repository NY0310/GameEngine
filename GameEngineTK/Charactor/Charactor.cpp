#include "Charactor.h"

using namespace std;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="maxHp">�̗͏��</param>
/// <param name="matrixObject">�I�u�W�F�N�g�s��</param>
Charactor::Charactor(int maxHp)
	:MAX_HP(maxHp)
{
}



/// <summary>
/// �f�X�g���N�^
/// </summary>
Charactor::~Charactor()
{
	Finalize();
}

/// <summary>
/// �������֐�
/// </summary>
void Charactor::Initialize()
{
	matrixObject = make_unique<MatrixObject>();
}

/// <summary>
/// �I���֐�
/// </summary>
void Charactor::Finalize()
{
	matrixObject.release();
}
