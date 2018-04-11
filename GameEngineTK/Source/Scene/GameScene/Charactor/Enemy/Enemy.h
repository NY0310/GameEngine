#pragma once
#include "../../../../PaintAsset/PaintSystem/PaintObj.h"
#include "../Character.h"
#include "../../../../PaintAsset/PaintGun/PaintGun.h"
#include "WeakSprite.h"

class Enemy : public PaintObj , public Character
{
public:
	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy() = default;
	// コピーコンストラクタ禁止
	Enemy(const Enemy&) = delete;
	// 代入禁止
	Enemy& operator=(const Enemy&) = delete;
	//子を生成し子供として登録
	void CreateAddChild();

	//初期化処理
	void Initialize();
	//
	void Update();
	//当たり判定
	void OnCollisiton(Collider * collider);
	//死んだとき
	void OnDied();

	enum WeakColorKind
	{
		red,
		green,
		blue,
		aqua,
		pink,
		yellow,
		total
	};

	//弱点リスト
	D3DXVECTOR4  WeakColorList[total]
	{
		DirectX::Colors::Red,
		DirectX::Colors::Green,
		DirectX::Colors::Blue,
		DirectX::Colors::Aqua,
		DirectX::Colors::Pink,
		DirectX::Colors::Yellow,
	};
private:
	//HP上限
	static const int MAX_HP = 3;
	//ペイント銃
	std::shared_ptr<PaintGun> paintGun;
	//弱点色
	D3DXVECTOR4 weakColor;
	//誤差上限
	static const float DIFFERENCE_UPPER;
	//弱点UI
	std::shared_ptr<WeakSprite> weakSprite;
};