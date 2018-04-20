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
		//移動中は当たり判定を行わない
		enemy->SetComponentActive(true);
	}

	float time = static_cast<float>(frameCnt) / MOVE_TIME;
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


	//enemy->SetBreakTime(breaktime);
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
