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
	Update();
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
	RemoveFromParent();
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
	if (GetCanUpdate())
	{
		this->Update();
	}
	for (auto& child : children) {
		child->LoopUpdate();
	}

}



void GameObjectNodeEmpty::LoopRender()
{
	//�X�N���[���`��
	if (GetVisible())
	{
		this->Render();
	}
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
	//�q���̐����擾
	int childNum = children.size();

	for (vector<shared_ptr<NodeAbstract>>::iterator child = children.begin(); child != children.end();  child++)
	{
		(*child)->LoopFinalize();
		//�q�����폜���ꂽ��
		if (childNum != static_cast<int>(children.size()))
		{
			child = children.begin();
			childNum = children.size();
		}
		if (childNum == 0)
			break;

	}

	RemoveFromParent();
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
	//�X�V�\��
	if (GetCanUpdate())
	{
		this->Update();
	}
	//�e�s��ɉe�����邩
	if (GetParantMatrixInfluence())
	{
		this->Calc(Getparent()._Get()->GetWorldMatrix());
	}
	else
	{
		this->Calc();
	}

	//�R���|�[�l���g�̍X�V����
	this->ComponentUpdate();

	int childNum = children.size();

	for (vector<shared_ptr<NodeAbstract>>::iterator child = children.begin(); child != children.end(); child++)
	{
		(*child)->LoopUpdate();
		//�q�����폜���ꂽ��
		if (childNum != children.size())
		{
			child = children.begin();
			childNum = children.size();
		}
	}
}