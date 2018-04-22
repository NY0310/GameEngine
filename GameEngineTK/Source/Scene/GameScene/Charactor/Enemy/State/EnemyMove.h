#pragma once
#include "EnemyStop.h"
#include "EnemyDied.h"
#include "EnemyState.h"

class EnemyMove :public EnemyState
{
public:
	//Standクラスのインスタンスを取得する
	static EnemyMove* GetInstance();
	//実行する
	void Execute(Enemy* enemy);
	//Standオブジェクトを破棄する
	static void Dispose();
private:
	//時間
	static const int MOVE_TIME = 180;
	//移動前座標
	D3DXVECTOR3 startPosition;
	//移動後座標
	D3DXVECTOR3 endPosition;
	//自身のインスタンス
	static EnemyMove* state;
};
