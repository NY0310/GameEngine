#include "Character.h"

using namespace std;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="maxHp">体力上限</param>
/// <param name="matrixObject">オブジェクト行列</param>
Character::Character()
{
}



/// <summary>
/// デストラクタ
/// </summary>
Character::~Character()
{
}




void Character::CheckData()
{
	if (hp <= 0)
		OnDied();
}

