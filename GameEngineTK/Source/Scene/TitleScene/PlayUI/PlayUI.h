#pragma once
#include "../../GameScene/Charactor/Enemy/Enemy.h"


class PlayUI : public Enemy
{
public:
	PlayUI() = default;
	// コピーコンストラクタ禁止
	PlayUI(const PlayUI&) = delete;
	// 代入禁止
	PlayUI& operator=(const PlayUI&) = delete;
private:
	void OnDied();

};