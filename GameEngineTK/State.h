//	ヘッダファイルのインクルード
#include "Obj3d.h"

//	多重インクルードの防止
#pragma once

//	クラス
class State {
public:
	// 仮想デストラクタ(virtual destructor)
	virtual ~State() {}
	// 純粋仮想関数を宣言する(Declare pure virtual function)
	virtual void Execute(Obj3d* pPlayer) = 0;
};