#pragma once
#include "../Character.h"
#include "../../../../NYLibrary/SkinMeshAnimation/SkinMesh.h"
#include "../../../../PaintAsset/PaintGun/PaintGun.h"

class PlayerState;
class Player : public Character , public NYLibrary::SkinMesh
{
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();
	// コピーコンストラクタ禁止
	Player(const Player&) = delete;
	// 代入禁止
	Player& operator=(const Player&) = delete;
	//子供を追加し親子関係を構築する
	void CreateAddChild();
	//初期化処理
	void Initialize();
	//更新処理
	void Update();
	//終了処理
	void Finalize();
	//状態変化
	void ChangeState(PlayerState* state);
	//描画
	void Render();
	//回復
	void Recovery();
	//エイムの行列管理
	std::shared_ptr<MatrixObject> aimMatrix;
private:
	//ペイント銃
	std::shared_ptr<PaintGun> paintGun;
	//プレイヤの状態
	PlayerState* playerState;
	//死んだとき
	void OnDied();
	//HP上限
	static const int MAX_HP = 5;
	//HP回復インターバル
	static const int RECOVERY_INTERVAL = 120;
	//回復フレームカウント
	int recoveryFrameCnt;
};

#include "State/PlayerState.h"
