#include "SceneManager.h"
using namespace std;
using namespace NYLibrary;
using namespace Scene;


/// <summary>
/// シーンを変更する
/// </summary>
/// <param name="scene">次のシーン</param>
void SceneManager::ReplaceScene(SceneKind scenekind)
{

	if (scenekind == SceneKind::NullScene)
		return;
	int c = 0;
	Count(c);
	if (scene)
	{
		//現在のシーンとの親子関係を消す
		scene->LoopFinalize();
	}
	c = 0;

	Count(c);

	scene = nullptr;
	//scene.reset();
	switch (scenekind)
	{
	case SceneKind::TitleScene:
		scene = make_shared<TitleScene>();
		break;
	case SceneKind::GameScene:
		scene = make_shared<GameScene>();
		break;
	case SceneKind::ResultScene:
		scene = make_shared<ResultScene>();
		break;
	default:
		assert(!"存在しないクラス");
		scene = make_shared<NullScene>();
		break;
	}

	c = 0;

	Count(c);

	//次のシーンをリセット
	nextScene = SceneKind::NullScene;
	//次のシーンを子供として登録
	AddChild(scene);
	LoopCreateAddChild();
	LoopInitialize();

	c = 0;

	Count(c);

}

/// <summary>
/// 更新
/// </summary>
void SceneManager::Update()
{
	ReplaceScene(nextScene);
}



