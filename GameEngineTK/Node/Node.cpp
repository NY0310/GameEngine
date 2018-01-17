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
/// �q��؂藣��
/// 
/// �؂藣�����q�̃A�h���X�͍폜���Ȃ�
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
/// �r�W�^�[�̎󂯓���
/// </summary>
/// <param name="visitor">�r�W�^�[</param>
void Node::Accept(shared_ptr<VisitorInterface>&& visitor)
{
	visitor->Visit(shared_from_this());
	for (auto& child : children) {
		child->Accept(move(visitor));
	}
}



/// <summary>
/// �m�[�h�̐����J�E���g����(�ċN�֐�)
/// </summary>
/// <param name="cnt">�m�[�h�̐�</param>
void Node::Count(int& cnt)
{
	cnt++;
	for (auto& child : children) {
		child->Count(cnt);
	}
}

