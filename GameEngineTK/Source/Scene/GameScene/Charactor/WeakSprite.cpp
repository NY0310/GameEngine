#include "WeakSprite.h"
#include "Target.h"

const float WeakSprite::WAKE_DISTANCE = 0.4f;

/// <summary>
/// コンストラクタ
/// </summary>
WeakSprite::WeakSprite(Target* target)
	:Sprite(L"Resources/PNG/weak.png",Dimension3),
	target(target)
{
	//ビルボードにする
	SetIsBillBoard(true);
}

/// <summary>
/// 更新
/// </summary>
void WeakSprite::Update()
{
	//敵の上に描画する
	D3DXVECTOR3 position = this->Getparent()._Get()->GetPosition();
	position.y += this->Getparent()._Get()->GetScaleY() * this->Getparent()._Get()->GetLocalSize().y / 2;
	SetPosition(position);
	//表示非表示切り替え
	VisibleUpdate();
}

/// <summary>
/// 表示非表示切り替え
/// </summary>
void WeakSprite::VisibleUpdate()
{
	if (target->GetDied())
	{
		SetVisible(false);
	}
	else
	{
		SetVisible(true);
	}
}
