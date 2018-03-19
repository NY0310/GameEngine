#pragma once
#include <list>
#include <string.h>
#include <functional>
#include "Collider/Collider.h"
#include "../Object/LocalObject/LocalObject.h"
#include "../Object/MatrixObject/MatrixObject.h"
#include "../Object/ColorObject/ColorObject.h"

namespace NYLibrary {
	class Element : public MatrixObject,public LocalObject,public ColorObject
	{
	private:
		std::string tag;//タグ
		std::list<Component*> componentList;// コンポーネントリスト
		bool isActive;// アクティブ状態
	public:
		//コンストラクタ
		Element()
			: isActive(true)
		{
		}

		// デストラクタ
		virtual ~Element()
		{
			Clear();
		}

		//更新処理
		virtual void Update()
		{
			// 繋げられているコンポーネントの処理
			for (auto& component : componentList)
			{
				component->Update();
			}
		}

		/// <summary>
		/// コンポーネントを追加
		/// </summary>
		template <class C> void AddComponent()
		{ 
			//C* type = dynamic_cast<Collider>(C*);
			////コライダーならMatrixObjectの情報を渡す
			//if (type)
			//{
			//	componentList.emplace_back(new C(this));
			//}
			//else
			//{
			//	componentList.emplace_back(new C());
			//}

			// 重複防止
			componentList.unique();
		}

		/// <summary>
		/// コンポーネントを削除
		/// </summary>
		template <class C> void RemoveComponent()
		{
			for (auto& component : componentList)
			{
				C* type = dynamic_cast<C*>(component);

				if (type)
				{
					componentList.remove(component);
					break;
				}
			}
		}

		/// <summary>
		/// コンポーネントを取得する
		/// </summary>
		/// <returns>コンポーネント</returns>
		template <class C> C* GetComponent()
		{
			for (auto& component : componentList_)
			{
				C* type = dynamic_cast<C*>(component);

				if (type)
				{
					return type;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// コンポーネントリストをクリアする
		/// </summary>
		void Clear()
		{
			for (auto& component : componentList)
			{
				delete component;
			}

			componentList.clear();
		}

		//コライダーが他のコライダーに当たると呼び出される
		virtual void OnCollisiton(Collider* collider) {}

		//シーンオブジェクトににシーン変更のオブジェクト関数を登録する
		void SetReplaceSceneObject(Collider* collider) {
			std::function<void(Collider*)> thisFunction = std::bind(&Element::OnCollisiton, this, std::placeholders::_1);
			collider->addListener(thisFunction);
		}

		// アクティブ状態を設定する
		virtual void SetActive(bool active) { isActive = active; }
		// アクティブを取得する
		inline virtual bool GetActive()	const { return isActive; }

		//タグを設定する
		void SetTag(const std::string& tagName) { tag = tagName; }
		//タグを取得する
		const std::string& GetTag() { return tag; }
	};
};