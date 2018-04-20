#include "PlayUI.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayUI::PlayUI()
	:Target("Resources/OBJMODEL/start.obj")
{

}

/// <summary>
/// ������
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
/// �X�V
/// </summary>
void PlayUI::Update()
{
	Target::Update();
}

/// <summary>
/// �j�󂳂ꂽ�Ƃ�
/// </summary>
void PlayUI::OnBreakEnd()
{
	RunWithScene(SceneKind::GameScene);
}

/// <summary>
/// ���񂾂Ƃ�
/// </summary>
void PlayUI::OnDied(){
	BreakStart();
}
