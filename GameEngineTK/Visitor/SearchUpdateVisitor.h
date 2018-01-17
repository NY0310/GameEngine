/// <summary>
/// 更新ノード検索ビジター
/// Yusuke Nakata 
/// 2017/10/09
/// </summary>


#include "VisitorInterface.h"

#include <list>


namespace scenegraph {
	class NodeAbstract;

	class SearchUpdateVisitor : public VisitorInterface, public std::enable_shared_from_this<SearchUpdateVisitor>
	{
	public:
		//更新可能なノードのリスト
		using UpdateList = std::list<std::shared_ptr<NodeAbstract>>;
		//コンストラクタ
		SearchUpdateVisitor();
		//デストラクタ
		~SearchUpdateVisitor();
		//コピーコンストラクタ禁止
		SearchUpdateVisitor(const SearchUpdateVisitor&) = delete;
		//代入禁止
		SearchUpdateVisitor& operator=(const SearchUpdateVisitor&) = delete;
		//ノード用の検索関数
		void Visit(std::shared_ptr<scenegraph::Node> node) override;
		//リーフ用の検索関数
		void Visit(std::shared_ptr<scenegraph::Leaf> leaf) override;
		//更新リスト取得
		inline UpdateList GetUpdataList() { return updateList; }
		//更新リストを空にする
		inline void ClearUpdateList() { updateList.clear(); }
	private:
		//更新可能なノードのリスト
		UpdateList updateList;
	};
}
