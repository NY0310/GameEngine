#include "PlayerRun.h"

PlayerState* PlayerRun::state = nullptr;
const D3DXVECTOR3 PlayerRun::SPEED = D3DXVECTOR3(0.5f,0,1.0f);

/***************************************************************************
*|	概要　	PlayerRunクラスのインスタンスを取得する
*|	引数　　無し
*|　戻り値　プレイヤの状態
****************************************************************************/
PlayerState* PlayerRun::GetInstance()
{
	if (state == nullptr)
	{
		state = new PlayerRun();
	}

	return state;
}




/***************************************************************************
*|	概要　　更新処理
*|	引数　　プレイヤ
*|　戻り値　無し
****************************************************************************/
void PlayerRun::Execute(Player * player)
{
	//プレイヤの移動
	MoveUpdate(player, SPEED);
}



/***************************************************************************
*|	概要　　破棄する
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
void PlayerRun::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
