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
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"
#include "ResultScene/ResultScene.h"


namespace Scene
{

	class SceneManager : public NYLibrary::RootGameObjectNode
	{
	public:
		//コンストラクタ
		SceneManager() { SetReplaceSceneObject(); }
		//デストラクタ
		~SceneManager() = default;
		//コピーコンストラクタ禁止
		SceneManager(const SceneManager&) = delete;
		//代入禁止
		SceneManager& operator=(const SceneManager&) = delete;
		//シーン変更
		void ReplaceScene(SceneKind scenekind);
		//更新
		void Update();
		//次のシーンを登録
		void RegisterNextScene(SceneKind nextScene) {
			this->nextScene = nextScene;
		}
	private:
		//シーン
		std::shared_ptr<SceneAbstract> scene;
		//シーンオブジェクトにシーン変更のオブジェクト関数を登録する
		void SetReplaceSceneObject(){
			std::function<void(SceneKind)> thisFunction = std::bind(&SceneManager::RegisterNextScene, this, std::placeholders::_1);
			AddRunWithScene(thisFunction);
		}
		//次のシーン
		SceneKind nextScene;
	};
};


