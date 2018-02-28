#pragma once
#include "../Charactor.h"
#include "../../SkinMesh.h"
#include "../../Paint/PaintGun.h"

class Player : public Charactor
{
public:
	//コンストラクタ
	Player(int maxHp);
	//デストラクタ
	~Player() = default;
	// コピーコンストラクタ禁止
	Player(const Player&) = delete;
	// 代入禁止
	Player& operator=(const Player&) = delete;
	//子供を追加し親子関係を構築する
	void CreateAddChild()final;
	//初期化処理
	void Initialize()final;
	//更新処理
	void Update()final;
	//描画処理　
	void Render()final;
	//終了処理
	void Finalize()final;
private:
	std::unique_ptr<SkinMesh> animetionSkin;
	std::shared_ptr<PaintGun> paintGun;
};

