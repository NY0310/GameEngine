#pragma once
#include <memory>
#include "../MatrixObject.h"
#include "../Node/GameObjectNode.h"

class Charactor : public scenegraph::GameObjectNode
{
public:
	//コンストラクタ
	Charactor(int maxHp);
	//デストラクタ
	virtual ~Charactor();
	// コピーコンストラクタ禁止
	Charactor(const Charactor&) = delete;
	// 代入禁止
	Charactor& operator=(const Charactor&) = delete;
	//初期化
	virtual void Initialize();
	//更新処理
	virtual void Update() = 0;
	//描画処理　
	virtual void Render() = 0;
	//終了処理
	virtual void Finalize();

protected:
	//体力上限
	const int MAX_HP;
	//残り体力
	int hp;

	//行列管理
	std::unique_ptr<MatrixObject> matrixObject;
};