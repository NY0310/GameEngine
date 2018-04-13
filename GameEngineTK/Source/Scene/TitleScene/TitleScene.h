/// <summary>
/// タイトルシーン
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"
#include "../GameScene/Charactor/Player/MainPlayer.h"

namespace Scene
{

	class TitleScene : public SceneAbstract {
	public:
		TitleScene() = default;
		//オブジェクトを生成し子供として登録
		void CreateAddChild();
		//初期化
		void Initialize()final;
		//更新
		void Update()final;
		//描画
		void Render()final {};
		//終了
		void Finalize()final {};
		//コピーコンストラクタ禁止
		TitleScene(const TitleScene&) = delete;
		//代入禁止
		TitleScene& operator=(const TitleScene&) = delete;
	private:
		std::shared_ptr<Player> player;
	};
};