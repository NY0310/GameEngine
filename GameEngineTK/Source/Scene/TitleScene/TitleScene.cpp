/// <summary>
/// �^�C�g���V�[��
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#include "TitleScene.h"
using namespace Scene;
using namespace std;
using namespace NYLibrary;

void TitleScene::CreateAddChild()
{
	//�I�u�W�F�N�g�𐶐����q���Ƃ��ēo�^
	player = make_shared<Player>();
	AddChild(player);
}

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
