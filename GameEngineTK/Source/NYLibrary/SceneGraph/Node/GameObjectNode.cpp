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
	//シャドウマップ取得
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
	//シャドウマップ取得
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
/// ノードを複製する(再起関数)
/// </summary>
/// <returns>複製したノード</returns>
shared_ptr<NodeAbstract> RootGameObjectNode::Clone()
{

	auto clone = make_shared<RootGameObjectNode>(*this);

	// リストをクリアする
	clone->GetChildren().clear();

	for (auto& child : children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;


}






/// <summary>
/// 空オブジェクト(行列計算を行わない)
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
	//スクリーン描画
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
	//子供の数を取得
	int childNum = children.size();

	for (vector<shared_ptr<NodeAbstract>>::iterator child = children.begin(); child != children.end();  child++)
	{
		(*child)->LoopFinalize();
		//子供が削除されたか
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
/// ノードを複製する(再起関数)
/// </summary>
/// <returns>複製したノード</returns>
shared_ptr<NodeAbstract>  GameObjectNodeEmpty::Clone()
{

	auto clone = make_shared<GameObjectNodeEmpty>(*this);

	// リストをクリアする
	clone->GetChildren().clear();

	for (auto& child : children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;


}


/// <summary>
/// ゲームオブジェクト
/// </summary>
void GameObjectNode::LoopUpdate()
{
	//更新可能か
	if (GetCanUpdate())
	{
		this->Update();
	}
	//親行列に影響するか
	if (GetParantMatrixInfluence())
	{
		this->Calc(Getparent()._Get()->GetWorldMatrix());
	}
	else
	{
		this->Calc();
	}

	//コンポーネントの更新処理
	this->ComponentUpdate();

	int childNum = children.size();

	for (vector<shared_ptr<NodeAbstract>>::iterator child = children.begin(); child != children.end(); child++)
	{
		(*child)->LoopUpdate();
		//子供が削除されたか
		if (childNum != children.size())
		{
			child = children.begin();
			childNum = children.size();
		}
	}
}