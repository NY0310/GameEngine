/// <summary>
/// タイトルシーン
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"

namespace Scene
{

	class TitleScene : public SceneAbstract {
	public:
		TitleScene() = default;
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
	};
};