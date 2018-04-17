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
	frameCnt = 0;
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update()
{
	Target::Update();
	Move();
	frameCnt++;
}

/// <summary>
/// 死んだとき
/// </summary>
void Enemy::OnDied()
{
	BreakStart();

}

/// <summary>
/// 移動
/// </summary>
void Enemy::Move()
{
	if (MOVE_INTERVAL < frameCnt)
	{
		endPosition = GetMovePosition();
		frameCnt = 0;
	}

}

/// <summary>
/// 移動先の座標を取得する
/// </summary>
/// <returns>移動先の座標を取得</returns>
D3DXVECTOR3 Enemy::GetMovePosition()
{
	D3DXVECTOR3 position;
	position.x = RandMovePosition();
	position.y = 0;
	position.z = RandMovePosition();
	return position;
}


/// <summary>
/// ずらす値を乱数で算出し渡す
/// </summary>
/// <returns>ずらす値</returns>
float Enemy::RandMovePosition()
{
	float shift = static_cast<float>(rand() % MAX_MOVE_RADIUS) / 1000;
	shift /= 2;
	if (rand() % 2)
		shift *= -1;
	return shift;
}
