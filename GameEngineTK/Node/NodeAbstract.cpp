/// <summary>
/// ノードの抽象クラス
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
/// ノードの数を取得する
/// </summary>
/// <returns>ノードの数</returns>
int NodeAbstract::GetSize()
{
	int Cnt = 0;
	this->Count(Cnt);
	return Cnt;
}
