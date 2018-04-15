/// <summary>
/// シーンデータ
/// Yusuke Nakata
/// 2018/01/10
/// </summary>
#pragma once
#include "../NYLibrary/SceneGraph/Node/GameObjectNode.h"
//#include "SceneManager.h"
using namespace NYLibrary;

namespace Scene
{
	/// <summary>
	/// シーンクラス
	/// </summary>
	class SceneAbstract : public NYLibrary::GameObjectNode
	{
	public:
		//コンストラクタ
		SceneAbstract() = default;
		//デストラクタ
		virtual ~SceneAbstract() = default;
		//初期化
		virtual void Initialize() = 0;
		//更新
		virtual void Update() = 0;
		//描画
		virtual void Render() = 0;
		//終了
		virtual void Finalize() = 0;
		//コピーコンストラクタ禁止
		SceneAbstract(const SceneAbstract&) = delete;
		//代入禁止
		SceneAbstract& operator=(const SceneAbstract&) = delete;
	};




	/// <summary>
	/// Nullシーン
	/// </summary>
	class NullScene : public SceneAbstract {
	public:
		//コンストラクタ
		NullScene() = default;
		//初期化
		void Initialize()final{};
		//更新
		void Update()final {};
		//描画
		void Render()final {};
		//終了
		void Finalize()final {};
		//コピーコンストラクタ禁止
		NullScene(const NullScene&) = delete;
		//代入禁止
		NullScene& operator=(const NullScene&) = delete;
	};
};