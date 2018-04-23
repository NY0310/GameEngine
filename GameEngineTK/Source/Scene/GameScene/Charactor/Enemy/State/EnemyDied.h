#pragma once
#include "EnemyStop.h"
#include "EnemyState.h"

class EnemyDied :public EnemyState
{
public:
	//Standクラスのインスタンスを取得する
	static EnemyDied* GetInstance();
	//実行する
	void Execute(Enemy* enemy);
	//Standオブジェクトを破棄する
	static void Dispose();
private:
	//自身のインスタンス
	static EnemyDied* state;
	//最初か
	static bool isFirst;
};
