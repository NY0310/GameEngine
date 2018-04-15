/// <summary>
/// リザルトシーン
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"

namespace Scene
{
	class ResultScene : public SceneAbstract {
	public:
		//コンストラクタ
		ResultScene() = default;
		//初期化
		void Initialize()final {};
		//更新
		void Update()final {};
		//描画
		void Render()final {};
		//終了
		void Finalize()final {};
		//コピーコンストラクタ禁止
		ResultScene(const ResultScene&) = delete;
		//代入禁止
		ResultScene& operator=(const ResultScene&) = delete;
	};

};