#include "EnemyDied.h"

EnemyDied* EnemyDied::state = nullptr;
//最初か
bool EnemyDied::isFirst = true;


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
	isFirst = true;
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
		//オブジェクトを破壊する
		enemy->BreakStart();
		//停止中は当たり判定を行わない
		enemy->SetComponentActive(false);
		isFirst = false;
	}


	//破壊され終わったか
	if (enemy->IsBreakEnd())
	{
		isFirst = true;
		//敵についたインクをクリア
		enemy->ClearCampus();
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
