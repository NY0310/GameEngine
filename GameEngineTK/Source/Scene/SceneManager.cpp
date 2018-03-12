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
	//現在のシーンとの親子関係を消す
	RemoveChild(scene);
	scene.reset();
	switch (scenekind)
	{
	case SceneKind::Title:
		scene = make_shared<TitleScene>();
		break;
	case SceneKind::Game:
		scene = make_shared<GameScene>();
		break;
	case SceneKind::Result:
		scene = make_shared<ResultScene>();
		break;
	default:
		assert(!"存在しないクラス");
		scene = make_shared<NullScene>();
		break;
	}

	//次のシーンを子供として登録
	AddChild(scene);

	//シーンオブジェクトににシーン変更のオブジェクト関数を登録する
	void SetReplaceSceneObject();
}



