/// <summary>
/// シーン管理クラス
/// Yusuke Nakata
/// 2018/01/10
/// </summary>
#pragma once
#include <memory>
#include <assert.h>
#include <functional>
#include "../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "Scene.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"
#include "ResultScene/ResultScene.h"


namespace Scene
{

	class SceneManager : public NYLibrary::RootGameObjectNode
	{
	public:
		//static SceneManager* GetInstance()
		//{
		//	if (!sceneManager.get())
		//	{
		//		sceneManager.reset(new SceneManager);
		//	}
		//	return sceneManager.get();
		//}
		//デストラクタ
		~SceneManager() = default;
		//コピーコンストラクタ禁止
		SceneManager(const SceneManager&) = delete;
		//代入禁止
		SceneManager& operator=(const SceneManager&) = delete;
		//シーン変更
		void ReplaceScene(SceneKind scenekind);
		//void CreateAddChild();
		//コンストラクタ
		SceneManager() {};
	private:
		//シーン
		std::shared_ptr<SceneAbstract> scene;
		//シーンオブジェクトににシーン変更のオブジェクト関数を登録する
		void SetReplaceSceneObject(SceneKind scenekind){
			std::function<void(SceneKind)> thisFunction = std::bind(&SceneManager::ReplaceScene, this, std::placeholders::_1);
			scene->addListener(thisFunction);
		}
	//	static std::unique_ptr<SceneManager> sceneManager;
	};
};


