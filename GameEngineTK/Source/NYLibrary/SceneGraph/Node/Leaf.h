/// <summary>
/// 葉(子を持たない)
/// Nakata Yusuke
/// 2017/10/10
/// </summary>
#pragma once

#include "NodeAbstract.h"
#include "../Visitor/VisitorInterface.h"


namespace NYLibrary
{
	class Leaf : public virtual NodeAbstract ,public std::enable_shared_from_this<Leaf>
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Leaf() :NodeAbstract() {}
		/// <summary>
		/// デストラクタ
		/// l子供を開放
		/// </summary>
		virtual ~Leaf()= default;
		Leaf& operator= (const Leaf&) = delete;
		/// <summary>
		/// 自身を削除する
		/// 
		/// 親から切り離したあとに削除
		/// </summary>
		inline void RemoveFromParent()override {
			parent._Get()->RemoveChild(shared_from_this());
			delete this;
		}

		/// <summary>
		/// ビジターの受け入れ
		/// </summary>
		/// <param name="visitor">ビジター</param>
		inline void Accept(std::shared_ptr<VisitorInterface>&& visitor)override {
			visitor->Visit(shared_from_this());
		}


		/// <summary>
		/// ノードの数をカウントする
		/// </summary>
		/// <param name="cnt">ノードの数</param>
		inline void Count(int& cnt)override{ cnt++;}

	private:
		/// <summary>
		/// 以下のメンバは子供を追加しない関係上
		/// 処理を書かない
		/// </summary>
		

		/// <summary>
		/// 子を追加する
		/// </summary>
		/// <param name="child">子</param>
		inline void AddChild(std::shared_ptr<NodeAbstract>&& child)override {};

		/// <summary>
		/// 子を切り離す
		/// </summary>
		inline std::shared_ptr<NodeAbstract> RemoveChild(const std::shared_ptr<NodeAbstract>& child) override { return std::shared_ptr<NodeAbstract>(nullptr);  };


		/// <summary>
		/// 子供を取得
		/// </summary>
		inline  NodeList& GetChildren()override { NodeList nodelist;  return nodelist; }

		/// <summary>
		/// 子を取得
		/// </summary>
		inline std::shared_ptr<NodeAbstract> GetChild(int childrenListIndex) const override { return std::shared_ptr<NodeAbstract>(nullptr); }

		/// <summary>
		/// 葉を複製する
		/// </summary>
		/// <returns>複製した葉</returns>
		inline std::shared_ptr<NodeAbstract> Clone()override {
			return shared_from_this();
		}

	};
}