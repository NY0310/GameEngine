/// <summary>
/// �m�[�h�̒��ۃN���X
/// Yusuke Nakata
/// </summary>
#include "NodeAbstract.h"
using namespace std;
using namespace NYLibrary;

NodeAbstract::NodeAbstract()
	:parent(shared_ptr<NodeAbstract>(nullptr))
{
}

/// <summary>
/// �m�[�h�̐����擾����
/// </summary>																																																																																																																																																																																wde
/// <returns>�m�[�h�̐�</returns>
int NodeAbstract::GetSize()
{
	int Cnt = 0;
	this->Count(Cnt);
	return Cnt;
}
