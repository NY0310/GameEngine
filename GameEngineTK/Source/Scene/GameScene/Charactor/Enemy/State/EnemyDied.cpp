#include "EnemyDied.h"

EnemyDied* EnemyDied::state = nullptr;

/***************************************************************************
*|	概要　	インスタンスを取得する
*|	引数　　無し
*|　戻り値　敵の状態
****************************************************************************/
EnemyDied* EnemyDied::GetInstance()
{
	if (state == nullptr)
	{
		state = new EnemyDied();
	}

	return state;
}




/***************************************************************************
*|	概要　　実行する
*|	引数　　敵
*|　戻り値　無し
****************************************************************************/
void EnemyDied::Execute(Enemy * enemy)
{
	if (isFirst)
	{
		//敵についたインクをクリア
		enemy->ClearCampus();
		//オブジェクトを破壊する
		enemy->BreakStart();
		isFirst = false;
	}

	//破壊され終わったか
	if (enemy->IsBreakEnd())
	{
		isFirst = true;
	//	enemy->SetPosition(GetMovePosition());
		enemy->BreakClear();
		enemy->Revival();
		enemy->ChangeState(EnemyMove::GetInstance());
	}
}



/***************************************************************************
*|	概要　　破棄する
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
void EnemyDied::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
