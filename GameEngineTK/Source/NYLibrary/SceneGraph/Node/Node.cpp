/// <summary>
/// Node
/// Yusuke Nakata
/// 2017/10/13
/// </summary>
#include "Node.h"
#include "../Visitor/VisitorInterface.h"

using namespace std;
using namespace NYLibrary;

Node::Node()
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
			children.erase(itr);
			return node;
		}
	}
	return shared_ptr<NodeAbstract>(nullptr);

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


/// <summary>
/// �m�[�h�̐����擾����
/// </summary>
/// <returns>�m�[�h�̐�</returns>
int Node::GetSize()
{
	int Cnt = 0;
	this->Count(Cnt);
	return Cnt;
}


