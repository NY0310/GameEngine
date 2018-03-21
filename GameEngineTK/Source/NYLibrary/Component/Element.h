#pragma once
#include <list>
#include <string.h>
#include <functional>
#include "../Object/LocalObject/LocalObject.h"
#include "../Object/MatrixObject/MatrixObject.h"
#include "../Object/ColorObject/ColorObject.h"
#include "Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"
#include "../../PaintAsset/InkSegmentCollider/InkSegmentCollider.h"

namespace NYLibrary {
	class ObjectData:  public MatrixObject, public LocalObject, public ColorObject
	{};
	class Element : public ObjectData
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
			//Collider* coll = new Collider();
			//C* type = dynamic_cast<C*>(coll);
			//delete coll;
			//coll = nullptr;
			//////コライダーならMatrixObjectの情報を渡す
			//if (type)
			//{
				Collider* collider = new C(tag, this);
				collider->Initialize();
				componentList.emplace_back(collider);
				
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
			for (auto& component : componentList)
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

		//コライダーのデータを更新する
		void ColliderDateUpdate()
		{
			for (auto& component : componentList)
			{
				Collider* type = dynamic_cast<Collider*>(component);

				if (type)
				{
					type->Initialize();
					break;
				}
			}
		}
			
		//コライダーが他のコライダーに当たると呼び出される(オーバーライドして任意の処理を追加してください。)
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
		void SetTag(std::string tagName) { tag = tagName; }
		//タグを取得する
		const std::string& GetTag() { return tag; }
	};
};