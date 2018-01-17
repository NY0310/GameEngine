/// <summary>
/// Node
/// Yusuke Nakata
/// 2017/10/13
/// </summary>
#include "Node.h"
#include "../Visitor/VisitorInterface.h"

using namespace std;
using namespace scenegraph;

Node::Node()
	:NodeAbstract()
{
	children.clear();
}

Node::~Node()
{
	children.clear();
}



/// <summary>
/// 子を切り離す
/// 
/// 切り離した子のアドレスは削除しない
/// </summary>
/// <param name="child"></param>
/// <returns></returns>
shared_ptr<NodeAbstract> Node::RemoveChild(const shared_ptr<NodeAbstract>& child)
{
	
	for (auto& itr = children.begin(); itr != children.end(); itr++)
	{
		if ((*itr) == child) {
			auto node = (*itr);
			//(*itr).reset();
			children.erase(itr);
			return node;
		}
	}
	return shared_ptr<Node>(nullptr);

}

/// <summary>
/// ビジターの受け入れ
/// </summary>
/// <param name="visitor">ビジター</param>
void Node::Accept(shared_ptr<VisitorInterface>&& visitor)
{
	visitor->Visit(shared_from_this());
	for (auto& child : children) {
		child->Accept(move(visitor));
	}
}



/// <summary>
/// ノードの数をカウントする(再起関数)
/// </summary>
/// <param name="cnt">ノードの数</param>
void Node::Count(int& cnt)
{
	cnt++;
	for (auto& child : children) {
		child->Count(cnt);
	}
}

