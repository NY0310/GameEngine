#pragma once
#include <memory>

class Character 
{
public:
	//コンストラクタ
	Character();
	//デストラクタ
	virtual ~Character();
	// コピーコンストラクタ禁止
	Character(const Character&) = delete;
	// 代入禁止
	Character& operator=(const Character&) = delete;
	//更新
	void CheckData();
	//ダメージを受ける
	void HitDamage() { hp--;}
	//ダメージを受ける
	void WeakHitDamage() { hp -= WEAK_DAMAGE; }
	//回復
	void Recovery() { hp++; }
protected:
	//死んだとき
	virtual void OnDied() {};
	//残り体力
	int hp;
	//弱点ダメージ
	static const int WEAK_DAMAGE = 2;
};