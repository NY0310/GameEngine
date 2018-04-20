#include "PlayUI.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayUI::PlayUI()
	:Target("Resources/OBJMODEL/start.obj")
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayUI::Initialize()
{
	Target::Initialize();
	LoadTextuerFile("Resources/PNG/start.png");
	SetTag("Stage");
	SetScale(3);
	hp = MAX_HP;
}

/// <summary>
/// 更新
/// </summary>
void PlayUI::Update()
{
	Target::Update();
}

/// <summary>
/// 破壊されたとき
/// </summary>
void PlayUI::OnBreakEnd()
{
	RunWithScene(SceneKind::GameScene);
}

/// <summary>
/// 死んだとき
/// </summary>
void PlayUI::OnDied(){
	BreakStart();
}
