/// <summary>
/// ノード
/// Yusuke Nakata
/// 2017/10/09
/// </summary>

#include <memory> 
#include <vector> 
#include "NodeAbstract.h"


namespace NYLibrary
{
	class VisitorInterface;

	class Node :public NodeAbstract ,public std::enable_shared_from_this<Node>
	{
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Node();

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Node();

		/// <summary>
		/// コピーコンストラクタ禁止
		/// </summary>
		//Node(const Node&) = delete;

		/// <summary>
		/// 代入禁止
		/// </summary>
		/// <param name="">ノード</param>
		/// <returns>ノード</returns>
		Node& operator=(const Node&) = delete;

		/// <summary>
		/// 子を追加する
		/// </summary>
		/// <param name="child">子</param>
		inline void AddChild(std::shared_ptr<NodeAbstract>&& child)
		{
			//親を設定
			auto a = child;
			auto d = shared_from_this();
			child->SetParent(shared_from_this());
			children.emplace_back(child);
		}

		/// <summary>
		/// 子を切り離す(アドレスは削除しない)
		/// </summary>
		/// <param name="child">削除する子</param>
		/// <returns>削除した子またはnullptr</returns>
		std::shared_ptr<NodeAbstract> RemoveChild(const std::shared_ptr<NodeAbstract>& child);

		/// <summary>
		/// 自信を削除(親から切り離したあと)
		/// </summary>
		void RemoveFromParent() {
			parent._Get()->RemoveChild(shared_from_this());
		}

		/// <summary>
		/// 子供を取得する
		/// </summary>
		/// <returns>子供のアドレスのリスト</returns>
		NodeList& GetChildren() { return children; }

		/// <summary>
		/// 子を取得
		/// </summary>
		/// <param name="childrenListIndex">子供リストの要素位置</param>
		/// <returns>子のアドレス</returns>
		std::shared_ptr<NodeAbstract> GetChild(int childrenListIndex)const  { return children[childrenListIndex]; }

		/// <summary>
		/// ビジターの受け入れ
		/// </summary>
		/// <param name="visitor">使用するビジター</param>
		void Accept(std::shared_ptr<VisitorInterface>&& visitor)override;

		/// <summary>
		/// ノードの数をカウントする(再起関数)
		/// </summary>
		/// <param name="cnt">ノードの数</param>
		void Count(int& cnt)override;

		//子供を追加
		virtual void LoopCreateAddChild() = 0;
		//初期化
		virtual void LoopInitialize() = 0;
		//更新
		virtual void LoopUpdate() = 0;
		//描画
		virtual void LoopRender() = 0;
		//終了
		virtual void LoopFinalize() = 0;
		//子供を追加し親子関係を構築する
		virtual void CreateAddChild() = 0;
		//初期化
		virtual void Initialize() = 0;
		//更新
		virtual void Update() = 0;
		//描画
		virtual void Render() = 0;
		//影の描画
		virtual void ShadowRender() = 0;
		//この描画処理後に画面に映すレンダリングターゲットを適応
		virtual void ClearRenderConfig() = 0;
		//終了
		virtual void Finalize() = 0;
		//ノードの数を取得する
		int GetSize();

	protected:
		//子供
		NodeList children;
	};
};