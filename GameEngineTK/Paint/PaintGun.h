#pragma once
#include <memory>
#include "../InkParticle.h"
#include "../Input/MouseRay.h"
#include "../CollisionNode.h"
#include "../AStar/Math.h"
#include "../Sprite/Sprite.h"
#include "../Device.h"

class PaintGun
{
public:
	PaintGun();
	//初期化
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Render();
	//終了処理
	void Finalize();
private:
	//インク発射
	void Shoot();
	//エイムの更新処理
	void AimUpdate();
	//インクのパーティクルマネージャー
	std::unique_ptr<InkParticleManager> inkParticleManager;
	//エイム
	std::unique_ptr<Sprite> aim;
};




class InkTank
{
public:
	void Update();
	void Reset();
	void Shoot();
private:
	enum StandardColor
	{
		red,
		green,
		yellow
	};
	void CalcColor();
	void ColorChange();
	D3DXVECTOR4 color;
	int redAmount;
	int greenAmount;
	int yellowAmount;
	int maxInk;
	StandardColor inColor;
};