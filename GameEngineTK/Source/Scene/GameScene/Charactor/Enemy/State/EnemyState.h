#pragma once
#include "../Enemy.h"


//プレイヤの状態
class EnemyState {
public:
	//コンストラクタ
	EnemyState() { frameCnt = 0; isFirst = true; };
	// 仮想デストラクタ
	virtual ~EnemyState() {}
	// 実行する
	virtual void Execute(Enemy* enemy) = 0;
protected:
	//フレームカウント
	int frameCnt;
	//最初か
	bool isFirst;
	/// <summary>
	/// 移動先の座標を取得する
	/// </summary>
	/// <returns>移動先の座標を取得</returns>
	D3DXVECTOR3 EnemyState::GetMovePosition()
	{
		D3DXVECTOR3 position;
		position.x = RandMovePosition();
		position.y = 0;
		position.z = RandMovePosition();
		return position;
	}
private:
	//最大移動半径
	static const int MAX_MOVE_RADIUS = 10000;

	/// <summary>
	/// ずらす値を乱数で算出し渡す
	/// </summary>
	/// <returns>ずらす値</returns>
	float EnemyState::RandMovePosition()
	{
		int radius = MAX_MOVE_RADIUS;
		float shift = static_cast<float>(rand() % MAX_MOVE_RADIUS) / 1000;
		shift /= 2;
		if (rand() % 2)
			shift *= -1;
		return shift;
	}

};