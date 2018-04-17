#include "Enemy.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Enemy::Enemy()
	:Target("Resources/OBJMODEL/Enemy.obj")
{
}

/// <summary>
/// ������
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
/// �X�V
/// </summary>
void Enemy::Update()
{
	Target::Update();
	enemyState->Execute(this);
}

/// <summary>
/// ���񂾂Ƃ�
/// </summary>
void Enemy::OnDied()
{
	BreakStart();

}



/// <summary>
/// ��Ԃ�ύX����
/// </summary>
/// <param name="state">���</param>
void Enemy::ChangeState(EnemyState * state)
{
	enemyState = state;
}
