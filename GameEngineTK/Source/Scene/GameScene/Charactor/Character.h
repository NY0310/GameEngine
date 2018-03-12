#pragma once
#include <memory>

class Character
{
public:
	//コンストラクタ
	Character(int maxHp);
	//デストラクタ
	virtual ~Character();
	// コピーコンストラクタ禁止
	Character(const Character&) = delete;
	// 代入禁止
	Character& operator=(const Character&) = delete;
protected:
	//体力上限
	const int MAX_HP;
	//残り体力
	int hp;
};