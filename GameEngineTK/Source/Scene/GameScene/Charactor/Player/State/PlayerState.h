#pragma once
#include "../MainPlayer.h"
#include "../../../../../NYLibrary/Input/KeyboardUtil.h"
#include "../../../../../NYLibrary/Input/MouseUtil.h"
#include "../../../../../NYLibrary/Device/Device.h"
#include "../../../../../NYLibrary/Math/Math.h"

//プレイヤの状態
class PlayerState {
public:
	// 仮想デストラクタ
	virtual ~PlayerState() {}
	// 実行する
	virtual void Execute(Player* player) = 0;
protected:
	//プレイヤの移動更新処理
	void MoveUpdate(Player* player, D3DXVECTOR3 speed);
private:
	//プレイヤの移動
	void Move(Player* player, D3DXVECTOR3 speed);
	//プレイヤの回転
	void Rotation(Player* player, D3DXVECTOR2 angle);
	//回転速度
	//static const float ROTATION;
};