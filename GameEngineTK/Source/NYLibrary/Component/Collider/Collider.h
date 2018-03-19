#pragma once
#include <vector>
#include <D3DX9math.h>
#include "../Component.h"
#include "../../Object/MatrixObject/MatrixObject.h"
#include "../../Collision/Collision.h"
#include "../../Collision/CollisionManager.h"



namespace NYLibrary
{
	class Collider : public Component
	{
	public: 
		//コンストラクタ
		Collider( std::string tag, MatrixObject* matrixObject) : tag(tag) { this->matrixObject.reset(matrixObject); }
		Collider() = default;
		//デストラクタ
		~Collider() { matrixObject.release(); };
		//更新処理
		virtual void Update() = 0;
		//当たった時にElement関数を呼び出す
		void OnCollision() { listener(this); }
		//当たり判定管理クラスに自信の情報を送る
		void RegisterCollider() {		
			CollisionManager* collisionManager = CollisionManager::GetInstance();
			collisionManager->AddCollider(this);
		}
		//当たったコライダーを追加する
		void AddCollisionCollider(Collider* collider) { collisitonColliderList.emplace_back(collider); }
		//処理を受け取る
		void addListener(std::function<void(Collider* collider)> listener) {
			this->listener = listener;
		}
	protected:
		//このコライダーに当たったコライダー
		std::vector<Collider*> collisitonColliderList;
		//オブジェクトのタグ(このタグが同じものは当たり判定を行わない)
		std::string tag;
		//座標、行列管理のアドレス
		std::unique_ptr<MatrixObject> matrixObject;
		//ラムダ式を受け取る
		std::function<void(Collider* collider)> listener = [&](Collider* collider) {};

	};



	/// <summary>
	/// 球体
	/// </summary>
	class SphereCollider : public Sphere, public Collider
	{
	public:
		//コンストラクタ
		SphereCollider(const std::string& tag, MatrixObject* matrixObject) : Collider(tag, matrixObject) {};
		//更新処理
		void Update() { center = matrixObject->GetPosition(); }
	};

	class InkSphereCollider : public SphereCollider
	{
	public:
		InkSphereCollider(const std::string& tag, MatrixObject* matrixObject): SphereCollider(tag, matrixObject) {};
		D3DXVECTOR4 color;
	};


	//線分
	class SegmentCollider : public Collider, public Segment
	{
	public:
		//コンストラクタ
		SegmentCollider(const std::string& tag, MatrixObject* matrixObject) : Collider(tag, matrixObject) {};
		//更新処理
		void Update() {  }
	};

	class InkSegmentCollider : public SegmentCollider
	{
	public:
		//コンストラクタ
		InkSegmentCollider(const std::string& tag, MatrixObject* matrixObject) : SegmentCollider(tag, matrixObject) {};
		//更新
		void Update() {
			SegmentCollider::Update();
		}
		D3DXVECTOR4 color;
		int index;
	};


	//三角形
	class TriangleCollider : public Triangle, public Collider
	{
	public:
		//コンストラクタ
		TriangleCollider(const std::string& tag, MatrixObject* matrixObject) : Collider(tag, matrixObject) {}
		//更新
		void Update(){}
	};


};