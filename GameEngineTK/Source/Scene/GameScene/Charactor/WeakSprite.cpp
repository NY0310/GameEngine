#include "WeakSprite.h"
#include "Target.h"

const float WeakSprite::PARENT_DISTANCE = 0.5f;

/// <summary>
/// コンストラクタ
/// </summary>
WeakSprite::WeakSprite(Target* target)
	:Sprite(L"Resources/PNG/weak.png",Dimension3),
	target(target)
{
	//ビルボードにする
	SetIsBillBoard(true);
	//親行列の影響を受ける
	SetParantMatrixInfluence(true);
	//親の頭に設置する
	SetPositionY(PARENT_DISTANCE);
}

/// <summary>
/// 更新
/// </summary>
void WeakSprite::Update()
{
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
