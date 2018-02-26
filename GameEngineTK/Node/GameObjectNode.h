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


namespace scenegraph
{

	class RootGameObjectNode :  public Node
	{
	public:
		//explicit GameObjectNode(tInterface>&& gameobject);
		RootGameObjectNode() = default;

		RootGameObjectNode& operator=(const RootGameObjectNode&) = delete;

		~RootGameObjectNode() = default;
		//初期化
		void LoopInitialize()override;
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
		//std::shared_ptr<gameobject::GameObjectInterface> gameObject;
	};
	class GameObjectNode : public Node
	{
	public:
		//explicit GameObjectNode();
		GameObjectNode() = default;

		//GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
		//初期化
		void Initialize()override {}
		//更新
		void Update()override {}
		//描画
		void Render()override {}
		//終了
		void Finalize()override {}

		/// <summary>
		/// ノードを複製する(再起関数)
		/// </summary>
		/// <returns>複製したノード</returns>
		std::shared_ptr<NodeAbstract> Clone()override;

	private:
		//初期化
		void LoopInitialize()override;
		//更新
		void LoopUpdate()override;
		//描画
		void LoopRender()override;
		//終了
		void LoopFinalize()override;

	};
}
