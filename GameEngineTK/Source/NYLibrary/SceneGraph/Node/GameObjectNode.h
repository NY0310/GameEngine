/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>
#pragma once
#include "Node.h"
#include "../../MatrixObject/MatrixObject.h"

//
//namespace gameobject {
//	class GameObjectInterface;
//}


namespace NYLibrary
{

	class RootGameObjectNode :  public Node
	{
	public:
		//explicit GameObjectNode(tInterface>&& gameobject);

		RootGameObjectNode& operator=(const RootGameObjectNode&) = delete;

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

		/// <summary>
		/// ノードを複製する(再起関数)
		/// </summary>
		/// <returns>複製したノード</returns>
		std::shared_ptr<NodeAbstract> Clone()override;

	private:
		//子供を追加し親子関係を構築する
		void CreateAddChild()override {};
		//初期化
		void Initialize()override {}
		//更新
		void Update()override {}
		//描画
		void Render()override {}
		//終了
		void Finalize()override {}

	};
	class GameObjectNodeEmpty : public NYLibrary::Node
	{
	public:
		//explicit GameObjectNode();
		GameObjectNodeEmpty() {}
		GameObjectNodeEmpty& operator=(const GameObjectNodeEmpty&) = delete;

		~GameObjectNodeEmpty() = default;
		//子供を追加し親子関係を構築する
		void CreateAddChild()override {};
		//初期化
		void Initialize()override {}
		//更新
		void Update()override {}
		//描画
		void Render()override {}
		//終了
		void Finalize()override {}
		void LoopCreateAddChild()override;
		//初期化
		void LoopInitialize()override;
		//更新
		virtual void LoopUpdate()override;
		//描画
		void LoopRender()override;
		//終了
		void LoopFinalize()override;

		/// <summary>
		/// ノードを複製する(再起関数)
		/// </summary>
		/// <returns>複製したノード</returns>
		std::shared_ptr<NodeAbstract> Clone()override;

	private:

	};

	class GameObjectNode : public NYLibrary::GameObjectNodeEmpty,public NYLibrary::MatrixObject
	{
	public:
		GameObjectNode() {}
		//explicit GameObjectNode();

		GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
		//子供を追加し親子関係を構築する
		void CreateAddChild()override {};
		//更新
		void LoopUpdate()override;

	private:

	};

};
