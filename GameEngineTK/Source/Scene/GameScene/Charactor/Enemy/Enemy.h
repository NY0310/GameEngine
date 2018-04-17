#pragma once
#include "../Target.h"

class EnemyState;
class Enemy : public Target
{
public:
	//コンストラクタ
	Enemy();
	// コピーコンストラクタ禁止
	Enemy(const Enemy&) = delete;
	// 代入禁止
	Enemy& operator=(const Enemy&) = delete;
	//初期化
	void Initialize();
	//更新する
	void Update();
	//状態変化
	void ChangeState(EnemyState * state);
private:
	//死んだとき
	void OnDied();
	//状態
	EnemyState* enemyState;
};

#include "State/EnemyState.h"
#include "State/EnemyStop.h"
