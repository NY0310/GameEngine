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

	if (scene)
	{
		//現在のシーンとの親子関係を消す
		scene->LoopFinalize();
	}

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



	//次のシーンをリセット
	nextScene = SceneKind::NullScene;
	//次のシーンを子供として登録
	AddChild(scene);
	LoopCreateAddChild();
	LoopInitialize();
}

/// <summary>
/// 更新
/// </summary>
void SceneManager::Update()
{
	ReplaceScene(nextScene);
}



