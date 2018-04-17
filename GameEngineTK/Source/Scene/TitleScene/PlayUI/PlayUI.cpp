#include "PlayUI.h"

PlayUI::PlayUI()
	:Target("Resources/OBJMODEL/start.obj")
{

}

/// <summary>
/// èâä˙âª
/// </summary>
void PlayUI::Initialize()
{
	Target::Initialize();
	LoadTextuerFile("Resources/PNG/start.png");
	SetTag("Stage");
	SetScale(3);
}

void PlayUI::OnDied(){
	RunWithScene(SceneKind::GameScene);
}
