#pragma once
#include "PlayerState.h"
#include "PlayerStop.h"
#include "PlayerWalk.h"

class PlayerRun :public PlayerState
{
public:
	//Standクラスのインスタンスを取得する
	static PlayerState* GetInstance();
	//実行する
	void Execute(Player* player);
	//Standオブジェクトを破棄する
	static void Dispose();
private:
	static PlayerState* state;
	static const D3DXVECTOR3 SPEED;
};
