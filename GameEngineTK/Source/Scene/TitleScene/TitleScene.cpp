/// <summary>
/// タイトルシーン
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#include "TitleScene.h"
using namespace Scene;
using namespace std;
using namespace NYLibrary;

void TitleScene::CreateAddChild()
{
	//オブジェクトを生成し子供として登録
	player = make_shared<Player>();
	AddChild(player);
}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize()
{
	Finalize();
	nextSceneKind = SceneKind::Game;
	RaiseReplaceScene(nextSceneKind);

}

/// <summary>
/// 更新
/// </summary>
void TitleScene::Update()
{
}
