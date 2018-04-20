#pragma once
#include "EnemyMove.h"
#include "EnemyState.h"


class EnemyStop :public EnemyState
{
public:
	//Standクラスのインスタンスを取得する
	static EnemyStop* GetInstance();
	//実行する
	void Execute(Enemy* enemy);
	//Standオブジェクトを破棄する
	static void Dispose();
private:
	static EnemyStop* state;
	//移動のインターバル
	static const int MOVE_INTERVAL = 1;
};
