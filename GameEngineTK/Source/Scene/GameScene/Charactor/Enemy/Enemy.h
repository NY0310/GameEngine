#pragma once
#include "../Target.h"
#include "../../Clear/ClearUI.h"

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
	//蘇生
	void Revival();
	//壊れた時に呼び出し
	void OnBreakEnd();
private:
	//状態
	EnemyState* enemyState;
	//HP上限
	static const int MAX_HP = 15;
	//回復回数
	static const int MAX_DIED = 5;
	//死亡回数
	int diedCnt;
	//クリア
	std::shared_ptr<ClearUI> clearUI;
};

#include "State/EnemyState.h"
#include "State/EnemyStop.h"
