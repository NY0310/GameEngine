#pragma once
#include "../../../NYLibrary/Sprite/Sprite.h"
class Target;
class WeakSprite : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	WeakSprite(Target* target);
	//更新
	void Update();
private:
	//デフォルトコンストラクタ禁止
	WeakSprite() = default;
	//表示非表示切り替え
	void VisibleUpdate();
	//ターゲット
	Target* target;
	//ズレ
	static const float PARENT_DISTANCE;
};
