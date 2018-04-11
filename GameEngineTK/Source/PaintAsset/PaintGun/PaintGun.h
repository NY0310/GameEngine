#pragma once
#include <memory>
#include "../InkParticle/InkParticle.h"
#include "../../NYLibrary/Collision/Collision.h"
#include "../../NYLibrary/Device/Device.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "Aim.h"
#include "InkTank.h"

class PaintGun : public NYLibrary::GameObjectNode
{
public:
	//コンストラクタ
	PaintGun();
	void CreateAddChild();
	//初期化
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Render();
	//終了処理
	void Finalize();
	//発射位置を設定する
	void SetAimMatirx(const std::shared_ptr<MatrixObject>& aimMatirx) {
		this->aimMatirx = aimMatirx;
	}
private:
	//インク発射
	void Emit();
	//インクのパーティクルマネージャー
	std::shared_ptr<InkParticleManager> inkParticleManager;
	//エイム
	std::shared_ptr<Aim> aim;
	//発射座標
	std::shared_ptr<MatrixObject> aimMatirx;
	//インクのタンク
	std::shared_ptr<InkTank> inkTank;
};



