/// <summary>
/// �^�C�g���V�[��
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#include "TitleScene.h"
using namespace Scene;

/// <summary>
/// ������
/// </summary>
void TitleScene::Initialize()
{
	Finalize();
	nextSceneKind = SceneKind::Game;
	RaiseReplaceScene(nextSceneKind);

}

/// <summary>
/// �X�V
/// </summary>
void TitleScene::Update()
{
}
