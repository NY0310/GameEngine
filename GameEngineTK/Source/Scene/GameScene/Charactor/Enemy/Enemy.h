#pragma once
#include "../Target.h"


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
private:
	//死んだとき
	void OnDied();
	//移動
	void Move();
	//移動先を取得する
	D3DXVECTOR3 GetMovePosition();
	//ずらす値を乱数で算出し渡す
	float RandMovePosition();
	//フレームカウント
	int frameCnt;
	//移動のインターバル
	static const int MOVE_INTERVAL = 360;
	//最大移動半径
	static const int MAX_MOVE_RADIUS = 10000;
	//移動後座標
	D3DXVECTOR3 endPosition;
};