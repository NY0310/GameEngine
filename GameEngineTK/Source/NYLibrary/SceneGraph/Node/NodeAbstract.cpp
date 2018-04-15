/// <summary>
/// ノードの抽象クラス
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
/// ノードの数を取得する
/// </summary>																																																																																																																																																																																wde
/// <returns>ノードの数</returns>
int NodeAbstract::GetSize()
{
	int Cnt = 0;
	this->Count(Cnt);
	return Cnt;
}
