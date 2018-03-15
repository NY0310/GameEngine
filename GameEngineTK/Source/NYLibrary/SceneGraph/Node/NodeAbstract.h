/// <summary>
/// NodeAbstract
/// NodeとLeafが継承する抽象クラス
/// Yusuke Nakata
/// 2017/10/13
/// </summary>
#pragma once

#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include "../../MatrixObject/MatrixObject.h"

namespace NYLibrary
{
	class Object : public MatrixObject{

	};

	class VisitorInterface;
	class NodeAbstract: public Object
	{
	public:
		//ノードのリスト
		using NodeList = std::vector<std::shared_ptr<NodeAbstract>>;
		//コンストラクタ
		NodeAbstract();
		//デストラクタ
		virtual ~NodeAbstract() = default;
		//コピーコンストラクタ禁止
		//NodeAbstract(const NodeAbstract&) = delete;
		//代入禁止
		NodeAbstract& operator=(const NodeAbstract&) = delete;
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
		//終了
		virtual void Finalize() = 0;

		//子を追加
		virtual void AddChild(std::shared_ptr<NodeAbstract>&& child) = 0;
		//子を切り離す
		virtual std::shared_ptr<NodeAbstract> RemoveChild(const std::shared_ptr<NodeAbstract>& child) = 0;
		//親から切り離したあと自信を削除
		virtual void RemoveFromParent() = 0;
		//子供を取得する
		virtual NodeList& GetChildren() = 0;
		//子を取得する
		virtual std::shared_ptr<NodeAbstract> GetChild(int children)const =0;
		//ビジターの受け入れ
		virtual void Accept(std::shared_ptr<VisitorInterface>&& visitor) = 0;
		//親のアドレスを取得
		std::weak_ptr<NodeAbstract> Getparent()const { return parent; }
		//親のアドレスを設定する
		inline void SetParent(const std::shared_ptr<NodeAbstract>& parent) { this->parent = parent; }
		//更新可能か
		bool CanUpdate() { return canUpdate;}
		//更新可能にする
		inline void EnableUpdate() { canUpdate = true; }
		//更新不可にする
		inline void DisableUpdate() { canUpdate = false;}
		//ノードの数を取得する
		int GetSize();
		//ノードの数をカウントする(再起関数)
		virtual void Count(int& cnt) = 0;
		//ノードを複製する
		virtual std::shared_ptr<NodeAbstract> Clone() = 0;
	protected:
		//親へのポインタ
		std::weak_ptr<NodeAbstract> parent;
		//更新できるか
		bool canUpdate;
	};
};

