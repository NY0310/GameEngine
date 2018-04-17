#pragma once
#include "EnemyStop.h"
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
	//移動先を取得する
	D3DXVECTOR3 GetMovePosition();
	//ずらす値を乱数で算出し渡す
	float RandMovePosition();

	//最大移動半径
	static const int MAX_MOVE_RADIUS = 10000;
	//時間
	static const int MOVE_TIME = 300;
	//移動前座標
	D3DXVECTOR3 startPosition;
	//移動後座標
	D3DXVECTOR3 endPosition;
	//自身のインスタンス
	static EnemyMove* state;
};
