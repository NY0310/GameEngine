#include "Charactor.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="maxHp">体力上限</param>
/// <param name="matrixObject">オブジェクト行列</param>
Charactor::Charactor(int maxHp, MatrixObject* matrixObject)
	:MAX_HP(maxHp)
{
	this->matrixObject.reset(matrixObject);
}

/// <summary>
/// デストラクタ
/// </summary>
Charactor::~Charactor()
{
	matrixObject.release();
}
