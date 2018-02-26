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
//::GameObjectNode::GameObjectNode(shared_ptr<GameObjectInterface>&& gameobject)
//	:Node()
//{
//	gameObject  = move(gameobject);
//}

void RootGameObjectNode::LoopInitialize()
{
	for (auto& child : children) {
		child->LoopInitialize();
	}
}

void RootGameObjectNode::LoopUpdate()
{
	for (auto& child : children) {
		child->LoopUpdate();
	}

}

void RootGameObjectNode::LoopRender()
{
	for (auto& child : children) {
		child->LoopRender();
	}
}

void RootGameObjectNode::LoopFinalize()
{
	for (auto& child : children) {
		child->LoopFinalize();
	}

}


/// <summary>
/// �m�[�h�𕡐�����(�ċN�֐�)
/// </summary>
/// <returns>���������m�[�h</returns>
shared_ptr<NodeAbstract> RootGameObjectNode::Clone()
{

	auto clone = make_shared<GameObjectNode>(*this);

	// ���X�g���N���A����
	clone->GetChildren().clear();

	for (auto& child : children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;


}







/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>



void GameObjectNode::LoopInitialize()
{
	this->Initialize();
	for (auto& child : children) {
		child->LoopInitialize();
	}
}

void GameObjectNode::LoopUpdate()
{
	this->Update();
	for (auto& child : children) {
		child->LoopUpdate();
	}

}

void GameObjectNode::LoopRender()
{
	this->Render();
	for (auto& child : children) {
		child->LoopRender();
	}

}

void GameObjectNode::LoopFinalize()
{
	this->Finalize();
	for (auto& child : children) {
		child->LoopFinalize();
	}

}


/// <summary>
/// �m�[�h�𕡐�����(�ċN�֐�)
/// </summary>
/// <returns>���������m�[�h</returns>
shared_ptr<NodeAbstract>   GameObjectNode::Clone()
{

	auto clone = make_shared<GameObjectNode>(*this);

	// ���X�g���N���A����
	clone->GetChildren().clear();

	for (auto& child : children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;


}

