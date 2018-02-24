#pragma once
#include <memory>
#include "../MatrixObject.h"

class Charactor
{
public:
	//コンストラクタ
	Charactor(int maxHp, MatrixObject* matrixObject);
	Charactor() = delete;
	//デストラクタ
	virtual ~Charactor();
	// コピーコンストラクタ禁止
	Charactor(const Charactor&) = delete;
	// 代入禁止
	Charactor& operator=(const Charactor&) = delete;

protected:
	//体力上限
	const int MAX_HP;
	//残り体力
	int hp;
	//行列管理
	std::unique_ptr<MatrixObject> matrixObject;
};