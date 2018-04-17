#include "EnemyMove.h"

EnemyMove* EnemyMove::state = nullptr;

/***************************************************************************
*|	概要　	インスタンスを取得する
*|	引数　　無し
*|　戻り値　敵の状態
****************************************************************************/
EnemyMove* EnemyMove::GetInstance()
{
	if (state == nullptr)
	{
		state = new EnemyMove();
	}

	return state;
}




/***************************************************************************
*|	概要　　実行する
*|	引数　　敵
*|　戻り値　無し
****************************************************************************/
void EnemyMove::Execute(Enemy * enemy)
{ 
	if (isFirst)
	{
		startPosition = enemy->GetPosition();
		endPosition = GetMovePosition();
		frameCnt = 0;
		isFirst = false;
	}

	float time = static_cast<float>(frameCnt) / MOVE_TIME;
	float breaktime = time;
	if (breaktime <= 0.5f)
	{
		breaktime *= 2;
	}
	else
	{
		breaktime -= 1.0f;
		breaktime *= -2.0f;
	}


	enemy->SetBreakTime(breaktime);
	//線形補間で移動させる
	enemy->SetPosition(Math::Lerp(startPosition, endPosition, time));
	frameCnt++;
	if (time > 1)
	{
		enemy->ChangeState(EnemyStop::GetInstance());
		frameCnt = 0;
		isFirst = true;
	}



}

/// <summary>
/// 移動先の座標を取得する
/// </summary>
/// <returns>移動先の座標を取得</returns>
D3DXVECTOR3 EnemyMove::GetMovePosition()
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
float EnemyMove::RandMovePosition()
{
	float shift = static_cast<float>(rand() % MAX_MOVE_RADIUS) / 1000;
	shift /= 2;
	if (rand() % 2)
		shift *= -1;
	return shift;
}


/***************************************************************************
*|	概要　　破棄する
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
void EnemyMove::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
