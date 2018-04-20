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
	SetScale(0.3);
	SetPositionY(GetLocalSize().y *GetScaleY() / 2);
	SetTag("Enemy");
	enemyState = EnemyStop::GetInstance();
	hp = MAX_HP;
	isDied = false;
	diedCnt = 0;
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
/// ��Ԃ�ύX����
/// </summary>
/// <param name="state">���</param>
void Enemy::ChangeState(EnemyState * state)
{
	enemyState = state;
}

/// <summary>
/// �h��
/// </summary>
void Enemy::Revival()
{
	hp = MAX_HP;
	isDied = false;
	ChangeWeakColor();
}

void Enemy::OnBreakEnd()
{
	diedCnt++;
	if (MAX_DIED == diedCnt)
	{
		RunWithScene(SceneKind::TitleScene);
	}
}
