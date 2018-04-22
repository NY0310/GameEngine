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
	SetScale(0.3);
	SetPositionY(GetLocalSize().y *GetScaleY() / 2);
	SetTag("Enemy");
	enemyState = EnemyStop::GetInstance();
	hp = MAX_HP;
	isDied = false;
	diedCnt = 0;
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
/// 状態を変更する
/// </summary>
/// <param name="state">状態</param>
void Enemy::ChangeState(EnemyState * state)
{
	enemyState = state;
}

/// <summary>
/// 蘇生
/// </summary>
void Enemy::Revival()
{
	hp = MAX_HP;
	isDied = false;
	ChangeWeakColor();
}

/// <summary>
/// 壊れた時
/// </summary>
void Enemy::OnBreakEnd()
{
	diedCnt++;
	if (MAX_DIED == diedCnt)
	{
		//クリアクラス作成
		clearUI = make_shared<ClearUI>();
		AddChild(clearUI);
		//更新不可能にする
		SetCanUpDate(false);
		//非表示
		SetVisible(false);
		//弱点を非表示
		weakSprite->SetVisible(false);
	}
}
