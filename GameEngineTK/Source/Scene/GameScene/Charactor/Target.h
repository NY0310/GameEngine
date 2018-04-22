#pragma once
#include "../../../PaintAsset/PaintSystem/PaintObj.h"
#include "Character.h"
#include "WeakSprite.h"

class Target : public PaintObj , public Character
{
public:
	//コンストラクタ
	Target(LPSTR fileName);
	Target() = delete;
	//デストラクタ
	~Target() = default;
	// コピーコンストラクタ禁止
	Target(const Target&) = delete;
	// 代入禁止
	Target& operator=(const Target&) = delete;
	//生成し子供として登録
	void CreateAddChild();
	//初期化
	void Initialize();
	//更新
	void Update();
	//当たり判定
	void OnCollisiton(Collider * collider);
	//死んだとき
	virtual void OnDied();
	//弱点か
	bool IsWeakColor(const D3DXVECTOR4 & hitColor);
	//弱点色を変更する
	void ChangeWeakColor();
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
		DirectX::Colors::Magenta,
		DirectX::Colors::Yellow,
	};
private:
	//HP上限
	static const int MAX_HP = 3;
	//弱点色
	D3DXVECTOR4 weakColor;
	//誤差上限
	static const float DIFFERENCE_UPPER;
protected:
	//弱点UI
	std::shared_ptr<WeakSprite> weakSprite;
};