#include "PlayerWalk.h"
//pPlayerState静的変数を初期化する
PlayerState* PlayerWalk::state = nullptr;
const D3DXVECTOR3 PlayerWalk::SPEED = D3DXVECTOR3(0.1f,0,0.5f);

/***************************************************************************
*|	概要　	Advanceクラスのインスタンスを取得する
*|	引数　　無し
*|　戻り値　プレイヤの状態
****************************************************************************/
PlayerState* PlayerWalk::GetInstance()
{
	if (state == nullptr)
	{
		state = new PlayerWalk();
	}

	return state;
}




/***************************************************************************
*|	概要　　動く
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
void PlayerWalk::Execute(Player * player)
{
	//プレイヤの移動
	MoveUpdate(player, SPEED);

}



/***************************************************************************
*|	概要　　破棄する
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
void PlayerWalk::Dispose()
{
	if (state != nullptr)
	{
		delete state;
	}
}
