#pragma once
#include "../Charactor.h"

class Player : public Charactor
{
public:
	//コンストラクタ
	Player(int maxHp, MatrixObject* matrixObject);
	//デストラクタ
	~Player() = default;
	// コピーコンストラクタ禁止
	Player(const Player&) = delete;
	// 代入禁止
	Player& operator=(const Player&) = delete;
private:

};

