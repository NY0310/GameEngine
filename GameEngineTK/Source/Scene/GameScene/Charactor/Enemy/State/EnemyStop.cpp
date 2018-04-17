#include "EnemyStop.h"

EnemyStop* EnemyStop::state = nullptr;

/***************************************************************************
*|	概要　	インスタンスを取得する
*|	引数　　無し
*|　戻り値　プレイヤの状態
****************************************************************************/
EnemyStop* EnemyStop::GetInstance()
{
	if (state == nullptr)
	{
		state = new EnemyStop();
	}

	return state;
}




/***************************************************************************
*|	概要　　実行する
*|	引数　　プレイヤ
*|　戻り値　無し
****************************************************************************/
void EnemyStop::Execute(Enemy * enemy)
{
	if (MOVE_INTERVAL <= frameCnt)
	{
		frameCnt = 0;
		enemy->ChangeState(EnemyMove::GetInstance());
	}
	frameCnt++;
}



/***************************************************************************
*|	概要　　破棄する
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
void EnemyStop::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
