#pragma once
#include <memory>
#include "../InkParticle/InkParticle.h"
#include "../../NYLibrary/Collision/Collision.h"
#include "../../NYLibrary/Device/Device.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "../../NYLibrary/Input/MouseRay.h"
#include "Aim.h"

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
	void SetEmitPosition(const D3DXVECTOR3& emitPosition) {
		this->emitPosition = emitPosition;
	}
private:
	//インク発射
	void Emit();
	//インクのパーティクルマネージャー
	std::shared_ptr<InkParticleManager> inkParticleManager;
	//エイム
	std::shared_ptr<Aim> aim;
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