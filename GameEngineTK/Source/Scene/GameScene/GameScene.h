/// <summary>
/// ゲームシーン
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"
#include "Charactor/Player/MainPlayer.h"
#include "Stage/Stage.h"
#include "Charactor/Enemy/Enemy.h"
#include "../../NYLibrary/Sound/ADX2Le.h"
#include "../../Sounds/CueSheet_0.h"

namespace Scene
{
	class GameScene : public SceneAbstract {
	public:
		//コンストラクタ
		GameScene() = default;
		//子供を追加する
		void CreateAddChild();
		//初期化
		void Initialize()final;
		//更新
		void Update()final {};
		//描画
		void Render()final {};
		//終了
		void Finalize()final;
		//コピーコンストラクタ禁止
		GameScene(const GameScene&) = delete;
		//代入禁止
		GameScene& operator=(const GameScene&) = delete;
	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<Stage> stage;
		std::shared_ptr<Enemy> enemy;
	};
};