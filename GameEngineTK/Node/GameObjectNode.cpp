/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>

#include "GameObjectNode.h"
#include "../GameObject/GameObjectInterface.h"

using namespace std;
using namespace scenegraph;
using namespace gameobject;
::GameObjectNode::GameObjectNode(shared_ptr<GameObjectInterface>&& gameobject)
	:Node()
{
	gameObject  = move(gameobject);
}

void GameObjectNode::Initialize()
{
	gameObject->Initialize();
	for (auto& child : children) {
		child->Initialize();
	}
}

void GameObjectNode::Update()
{
	gameObject->Update();
	for (auto& child : children) {
		child->Update();
	}

}

void GameObjectNode::Render()
{
	gameObject->Render();
	for (auto& child : children) {
		child->Render();
	}

}

void GameObjectNode::Finalize()
{
	gameObject->Finalize();
	for (auto& child : children) {
		child->Finalize();
	}

}


/// <summary>
/// ノードを複製する(再起関数)
/// </summary>
/// <returns>複製したノード</returns>
shared_ptr<NodeAbstract>   GameObjectNode::Clone()
{

	auto clone = make_shared<GameObjectNode>(*this);

	// リストをクリアする
	clone->GetChildren().clear();

	for (auto& child : children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;


}

