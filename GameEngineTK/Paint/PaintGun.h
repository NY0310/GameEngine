#pragma once
#include <memory>
#include "../InkParticle.h"
#include "../Input/MouseRay.h"
#include "../CollisionNode.h"
#include "../AStar/Math.h"
#include "../Sprite/Sprite.h"
#include "../Device.h"
#include "../Node/GameObjectNode.h"

class PaintGun : public scenegraph::GameObjectNode
{
public:
	//コンストラクタ
	PaintGun();
	//初期化
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Render();
	//終了処理
	void Finalize();
	//発射位置を設定する
	void SetEmitPosition(const D3DXVECTOR3& emitPosition) {
		this->emitPosition = emitPosition
			;
	}

private:
	//インク発射
	void Emit();
	//エイムの更新処理
	void AimUpdate();
	//インクのパーティクルマネージャー
	std::unique_ptr<InkParticleManager> inkParticleManager;
	//エイム
	std::unique_ptr<Sprite> aim;
	//発射座標
	D3DXVECTOR3 emitPosition;
};




class InkTank
{
public:
	//初期化　
	void Initialize();
	//更新処理
	void Update();
	//発射
	void Emit();
private:
	enum StandardColor
	{
		red,
		green,
		blue,
		total
	};
	//色を計算
	void CalcColor();
	//追加する色を変更する
	void ChangeColor();
	D3DXVECTOR4 color;
	//インク量
	int colorAmount[total + 1];
	//追加する色
	StandardColor inColor;
};