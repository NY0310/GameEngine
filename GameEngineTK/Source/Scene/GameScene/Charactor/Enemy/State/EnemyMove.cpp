#include "EnemyMove.h"

EnemyMove* EnemyMove::state = nullptr;
//最初か
bool EnemyMove::isFirst = true;

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
	isFirst = true;

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
		//移動中は当たり判定を行う
		enemy->SetComponentActive(true);
	}

	//float breaktime = time;
	//if (breaktime <= 0.5f)
	//{
	//	breaktime *= 2;
	//}
	//else
	//{
	//	breaktime -= 1.0f;
	//	breaktime *= -2.0f;
	//}
	float time = static_cast<float>(frameCnt) / MOVE_TIME;
	//線形補間で移動させる
	enemy->SetPosition(Math::Lerp(startPosition, endPosition, time));
	frameCnt++;

	if (time >= 1)
	{
		isFirst = true;
	}

	//死んだとき		
	if (enemy->GetDied())
	{
		frameCnt = 0;
		isFirst = true;
		enemy->ChangeState(EnemyDied::GetInstance());
	}


	//enemy->SetBreakTime(breaktime);
	//if (time > 1)
	//{
	//	enemy->ChangeState(EnemyStop::GetInstance());
	//	frameCnt = 0;
	//	isFirst = true;
	//}



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
