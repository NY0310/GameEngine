/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>

#include "GameObjectNode.h"

using namespace std;
using namespace NYLibrary;

void RootGameObjectNode::LoopInitialize()
{
	//�V���h�E�}�b�v�擾
	ShadowMap* shadowMap = ShadowMap::GetInstance();
	shadowMap->Initilize();
	for (auto& child : children) {
		child->LoopInitialize();
	}
}

void RootGameObjectNode::LoopCreateAddChild()
{
	for (auto& child : children) {
		child->LoopCreateAddChild();
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

void RootGameObjectNode::LoopShadowRender()
{
	//�V���h�E�}�b�v�擾
	ShadowMap* shadowMap = ShadowMap::GetInstance();
	shadowMap->OMSetRenderTargets();
	shadowMap->ClearRTVAndDSV();
	shadowMap->SetShader();

	for (auto& child : children) {
		child->LoopShadowRender();
	}
}

void RootGameObjectNode::LoopClearRenderConfig()
{
	for (auto& child : children) {
		child->LoopClearRenderConfig();
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

	auto clone = make_shared<RootGameObjectNode>(*this);

	// ���X�g���N���A����
	clone->GetChildren().clear();

	for (auto& child : children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;


}






/// <summary>
/// ��I�u�W�F�N�g(�s��v�Z���s��Ȃ�)
/// </summary>


void GameObjectNodeEmpty::LoopCreateAddChild()
{
	this->CreateAddChild();
	for (auto& child : children) {
		child->LoopCreateAddChild();
	}
}

void GameObjectNodeEmpty::LoopInitialize()
{
	this->Initialize();
	for (auto& child : children) {
		child->LoopInitialize();
	}
}



void GameObjectNodeEmpty::LoopUpdate()
{
	this->Update();
	for (auto& child : children) {
		child->LoopUpdate();
	}

}



void GameObjectNodeEmpty::LoopRender()
{
	//�X�N���[���`��
	this->Render();
	for (auto& child : children) {
		child->LoopRender();
	}

}

void GameObjectNodeEmpty::LoopShadowRender()
{
	this->ShadowRender();
	for (auto& child : children) {
		child->LoopShadowRender();
	}
}

void GameObjectNodeEmpty::LoopClearRenderConfig()
{
	this->ClearRenderConfig();
	for (auto& child : children) {
		child->LoopClearRenderConfig();
	}
}

void GameObjectNodeEmpty::LoopFinalize()
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
shared_ptr<NodeAbstract>  GameObjectNodeEmpty::Clone()
{

	auto clone = make_shared<GameObjectNodeEmpty>(*this);

	// ���X�g���N���A����
	clone->GetChildren().clear();

	for (auto& child : children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;


}


/// <summary>
/// �Q�[���I�u�W�F�N�g
/// </summary>
void GameObjectNode::LoopUpdate()
{
	this->Update();
	this->Calc();
	for (auto& child : children) {
		child->LoopUpdate();
	}
	this->ComponentUpdate();
}