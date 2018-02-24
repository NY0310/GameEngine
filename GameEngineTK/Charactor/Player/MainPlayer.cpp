#include "MainPlayer.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="maxHp">体力</param>
/// <param name="matrixObject">行列管理</param>
Player::Player(int maxHp, MatrixObject * matrixObject):
	Charactor(maxHp,matrixObject)
{
}
