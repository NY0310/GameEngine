#pragma once
#include "../MainPlayer.h"
//	クラス
class PlayerState {
public:
	// 仮想デストラクタ(virtual destructor)
	virtual ~PlayerState() {}
	// 純粋仮想関数を宣言する(Declare pure virtual function)
	virtual void Execute(Player* player) = 0;
};