/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>
#pragma once
#include "Node.h"

//
//namespace gameobject {
//	class GameObjectInterface;
//}


namespace NYLibrary
{
	/// <summary>
	///	ルートノード
	/// このクラスがプロジェクトのアダムとデネブになるように
	/// 子供を作ってください。
	/// そうすればこのクラスのメンバを呼び出すと全てのクラスのメンバが呼び出されます。
	/// </summary>
	class RootGameObjectNode :  public Node
	{
	public:
		RootGameObjectNode() {}
		//RootGameObjectNode& operator=(const RootGameObjectNode&) = delete;

		~RootGameObjectNode() = default;
		//初期化
		void LoopInitialize()override;

		void LoopCreateAddChild()override;		
		//更新
		void LoopUpdate()override;
		//描画
		void LoopRender()override;
		//終了
		void LoopFinalize()override;
		// ノードを複製する(再起関数)
		std::shared_ptr<NodeAbstract> Clone();

	private:
		//子供を追加し親子関係を構築する
		void CreateAddChild()override {}
		//初期化
		void Initialize()override {}
		//更新
		void Update()override {}
		//描画
		void Render()override {}
		//終了
		void Finalize()override {}

	};


	/// <summary>
	/// ゲームオブジェクトノードエンプティー
	/// このクラスは空オブジェクトに使用してください
	/// 座標、行列機能が備わっていません。
	/// </summary>
	class GameObjectNodeEmpty : public Node
	{
	public:
		GameObjectNodeEmpty() {}
		GameObjectNodeEmpty& operator=(const GameObjectNodeEmpty&) = delete;

		~GameObjectNodeEmpty() = default;
		//子供を追加し親子関係を構築する
		void CreateAddChild()override {}
		//初期化
		void Initialize()override {}
		//更新
		void Update()override {}
		//描画
		void Render()override {}
		//終了
		void Finalize()override {}
		// ノードを複製する(再起関数)
		std::shared_ptr<NodeAbstract> Clone()final;
	private:
		void LoopCreateAddChild()final;
		//初期化
		void LoopInitialize()final;
		//更新
		virtual void LoopUpdate()override;
		//描画
		void LoopRender()final;
		//終了
		void LoopFinalize()final;

	};

	/// <summary>
	/// ゲームオブジェクトノード
	/// 座標、行列機能完備。毎フレーム行列計算します。
	/// </summary>
	class GameObjectNode : public GameObjectNodeEmpty
	{
	public:
		GameObjectNode() {}
		GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
	private:
		void LoopUpdate()final;

	};
};
