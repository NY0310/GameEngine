#include "Enemy.h"

Enemy::Enemy()
	:Target("Resources/OBJMODEL/Enemy.obj")
{
}

void Enemy::Initialize()
{
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	Target::Initialize();
	SetScale(0.2f);
	SetPositionY(GetLocalSize().y *GetScaleY() / 2);
	SetTag("Enemy");
}

void Enemy::OnDied()
{
	BreakStart();
}
