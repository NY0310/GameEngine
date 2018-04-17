#pragma once
#include "../Enemy.h"


//プレイヤの状態
class EnemyState {
public:
	//コンストラクタ
	EnemyState() { frameCnt = 0; isFirst = true; };
	// 仮想デストラクタ
	virtual ~EnemyState() {}
	// 実行する
	virtual void Execute(Enemy* enemy) = 0;
protected:
	//フレームカウント
	int frameCnt;
	//最初か
	bool isFirst;
};