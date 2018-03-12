///// <summary>
///// 更新ノード検索ビジター
///// Yusuke Nakata 
///// 2017/10/09
///// </summary>
//
//#include "SearchUpdateVisitor.h"
//#include "../Node/Node.h"
//#include "../Node/Leaf.h"
//
//using namespace std;
//using namespace scenegraph;
//
//
//SearchUpdateVisitor::SearchUpdateVisitor() {
//	updateList.clear();
//}
//
//SearchUpdateVisitor::~SearchUpdateVisitor() {
//	updateList.clear();
//}
//
//void SearchUpdateVisitor::Visit(shared_ptr<Node> node)
//{
//	if (node->CanUpdate()) updateList.emplace_back(node);
//}
//
//void SearchUpdateVisitor::Visit(shared_ptr<Leaf> leaf)
//{
//	if (leaf->CanUpdate()) updateList.emplace_back(leaf);
//}
