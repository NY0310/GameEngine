#pragma once
#include "../Target.h"


class Enemy : public Target
{
public:
	Enemy();
	// コピーコンストラクタ禁止
	Enemy(const Enemy&) = delete;
	// 代入禁止
	Enemy& operator=(const Enemy&) = delete;
	//初期化
	void Initialize();
private:
	void OnDied();

};