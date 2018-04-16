#pragma once
#include "../../GameScene/Charactor/Target.h"


class PlayUI : public Target
{
public:
	PlayUI();
	//初期化
	void Initialize();
	// コピーコンストラクタ禁止
	PlayUI(const PlayUI&) = delete;
	// 代入禁止
	PlayUI& operator=(const PlayUI&) = delete;
private:
	void OnDied();

};