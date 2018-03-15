#pragma once
#include <list>
#include <string.h>
#include "Component.h"

namespace NYLibrary {
	class Element
	{
	protected:
		std::string tag;
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
				//component->Update(this);
			}
		}

		/// <summary>
		/// コンポーネントを追加
		/// </summary>
		template <class C> void AddComponent()
		{ 
			componentList.emplace_back(new C);

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