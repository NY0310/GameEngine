/// <summary>
/// �m�[�h�̒��ۃN���X
/// Yusuke Nakata
/// </summary>
#include "NodeAbstract.h"
using namespace std;
using namespace scenegraph;

NodeAbstract::NodeAbstract()
	:parent(shared_ptr<NodeAbstract>(nullptr))
	/*:parent(make_shared<NodeAbstract>(nullptr))*/
{
}

/// <summary>
/// �m�[�h�̐����擾����
/// </summary>
/// <returns>�m�[�h�̐�</returns>
int NodeAbstract::GetSize()
{
	int Cnt = 0;
	this->Count(Cnt);
	return Cnt;
}
