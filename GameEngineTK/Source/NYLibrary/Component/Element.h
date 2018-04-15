#pragma once
#include <list>
#include <string.h>
#include <functional>
#include "../Object/LocalObject/LocalObject.h"
#include "../Object/MatrixObject/MatrixObject.h"
#include "../Object/ColorObject/ColorObject.h"
#include "Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"
#include "../../PaintAsset/InkSegmentCollider/InkSegmentCollider.h"
#include "Collider/PlaneCollider/PlaneCollider.h"

namespace NYLibrary {
	class ObjectData:  public MatrixObject, public LocalObject, public ColorObject
	{
	protected:
		std::string tag;//タグ

	public:
		//タグを設定する
		void SetTag(std::string tagName) { tag = tagName; }
		//タグを取得する
		const std::string& GetTag() { return tag; }
	
	};
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
			ClearComponent();
		}

		//更新処理
		virtual void ComponentUpdate()
		{
			if(isActive)
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
			////s//コライダーならMatrixObjectの情報を渡す
			//if (type)
			//{
				Collider * callCollision = new C(this);
				SetReplaceSceneObject(callCollision);
				componentList.emplace_back(callCollision);
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
		void ClearComponent()
		{
			for (auto& component : componentList)
			{
				component->Finalize();
				delete component;
				component = nullptr;
			}

			componentList.clear();
		}

		////コライダーのデータを更新する
		//void ColliderDateUpdate()
		//{
		//	for (auto& component : componentList)
		//	{
		//		Collider* type = dynamic_cast<Collider*>(component);

		//		if (type)
		//		{
		//			type->Initialize();
		//			break;
		//		}
		//	}
		//}

		/// <summary>
		/// 
		/// </summary>
		void SetComponentActive()
		{
			for (auto& component : componentList)
			{
				Collider* type = dynamic_cast<Collider*>(component);

				if (type)
				{
					type->SetActive(isActive);
					break;
				}
			}

		}
			
		//コライダーが他のコライダーに当たると呼び出される(オーバーライドして任意の処理を追加してください。)
		virtual void OnCollisiton(Collider* collider) {}

		//衝突時に呼び出す関数オブジェクトを登録する
		void SetReplaceSceneObject(Collider* collider) {
			std::function<void(Collider*)> thisFunction = std::bind(&Element::OnCollisiton, this, std::placeholders::_1);
			collider->addListener(thisFunction);
		}

		// アクティブ状態を設定する
		void SetComponentActive(bool active) { isActive = active; SetComponentActive(); }
		// アクティブを取得する
		bool GetComponentActive()	const { return isActive; }

	};
};