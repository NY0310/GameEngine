#pragma once
#include <vector>
#include <D3DX9math.h>
#include "../Component.h"
#include "../../Object/MatrixObject/MatrixObject.h"
#include "../../Collision/Collision.h"
#include "../../Collision/CollisionManager.h"



namespace NYLibrary
{
	class ObjectData;

	


	class Collider : public Component
	{
	public: 
		//コンストラクタ
		Collider(std::string tag, ObjectData* objectData) : tag(tag) {
			this->objectData.reset(objectData); }
		Collider() = delete;
		//デストラクタ
		~Collider() { objectData.release(); };
		//初期化
		virtual void Initialize() {}
		//更新
		virtual void Update() {}
		//線分との当たり判定
		virtual void Collision(SegmentCollider* segmentCollider) {}
		//三角形ポリゴンとの当たり判定
		virtual void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider) {}
		//当たり判定管理クラスに自信の情報を送る
		void RegisterCollider() {		
			CollisionManager* collisionManager = CollisionManager::GetInstance();
			collisionManager->AddCollider(this);
		}
		//処理を受け取る
		void addListener(std::function<void(Collider* collider)> listener) {
			this->listener = listener;
		}
		//タグを取得する
		const std::string& GetTag() { return tag; }
		// アクティブ状態を設定する
		void SetActive(bool active) { isActive = active; }
		// アクティブを取得する
		bool GetActive()const { return isActive; }
		//オブジェクトデータを取得
		ObjectData* GetObjectData() { return objectData.get(); }
		//当たった時にElement関数を呼び出す
		void OnCollision() { listener(this); }
		void AddCollisionCollider(Collider* collider) { collisitonColliderListNow.emplace_back(collider); }
	protected:
		//オブジェクトのタグ(このタグが同じものは当たり判定を行わない)
		std::string tag;
		//座標、行列管理のアドレス
		std::unique_ptr<ObjectData> objectData;
		//アクティブ状態か
		bool isActive;
		//現フレームにてコライダーに当たったコライダーリスト
		std::vector<Collider*> collisitonColliderListNow;
	private:
		//ラムダ式を受け取る
		std::function<void(Collider* collider)> listener = [&](Collider* collider) {};
	};



	/// <summary>
	/// 球体
	/// </summary>
	//class SphereCollider : public Sphere, public Collider
	//{
	//public:
	//	//コンストラクタ
	//	SphereCollider(const std::string& tag, MatrixObject* matrixObject) : Collider(tag, matrixObject) {};
	//	//更新処理
	//	void Update() { center = matrixObject->GetPosition(); }
	//};

	//class InkSphereCollider : public SphereCollider
	//{
	//public:
	//	InkSphereCollider(const std::string& tag, MatrixObject* matrixObject): SphereCollider(tag, matrixObject) {};
	//	D3DXVECTOR4 color;
	//};




	////class InkSegmentCollider : public SegmentCollider
	////{
	////public:
	////	//コンストラクタ
	////	InkSegmentCollider(const std::string& tag, MatrixObject* matrixObject) : SegmentCollider(tag, matrixObject) {};
	////	//更新
	////	void Update() {
	////		SegmentCollider::Update();
	////	}
	////	D3DXVECTOR4 color;
	////	int index;
	////};


	////三角形
	//class TriangleCollider : public Triangle, public Collider
	//{
	//public:
	//	//コンストラクタ
	//	TriangleCollider(const std::string& tag, MatrixObject* matrixObject) : Collider(tag, matrixObject) {}
	//	//更新
	//	void Update(){}
	//};


};