#include "Enemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
	:Target("Resources/OBJMODEL/Enemy.obj")
{
}

/// <summary>
/// 初期化
/// </summary>
void Enemy::Initialize()
{
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	Target::Initialize();
	SetScale(0.2f);
	SetPositionY(GetLocalSize().y *GetScaleY() / 2);
	SetTag("Enemy");
	enemyState = EnemyMove::GetInstance();
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{
	Target::Update();
	enemyState->Execute(this);
}

/// <summary>
/// 死んだとき
/// </summary>
void Enemy::OnDied()
{
	BreakStart();

}



/// <summary>
/// 状態を変更する
/// </summary>
/// <param name="state">状態</param>
void Enemy::ChangeState(EnemyState * state)
{
	enemyState = state;
}
