#pragma once
#include "../MainPlayer.h"
#include "../../../../../NYLibrary/Input/KeyboardUtil.h"
#include "../../../../../NYLibrary/Input/MouseUtil.h"
#include "../../../../../NYLibrary/Device/Device.h"
#include "../../../../../NYLibrary/Math/Math.h"

//プレイヤの状態
class PlayerState {
public:
	//コンストラクタ
	PlayerState();
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
	D3DXQUATERNION Rotation(D3DXVECTOR2 angle);
	//回転 Yを最大値をクランプする
	void Clamp(float& rotY,int max) {
		if (max >= rotY)
		{
			rotY = max;
			
		}
		else if (max * -1 >= rotY)
		{
			rotY = max * -1;
		}
	}
	//回転速度
	static const int ROTATION_COEFFICIENT;
	//マウスの相移動量
	D3DXVECTOR2 mouseTotalTrans;
	//マウスの初期位置
	D3DXVECTOR2 initialMousePosition;
	//回転量Yの最大値
	static const D3DXVECTOR2 MAX_ROTATION;
};