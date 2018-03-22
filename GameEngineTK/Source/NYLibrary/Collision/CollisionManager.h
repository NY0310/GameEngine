#pragma once
#include <memory>
#include <vector>
#include <algorithm>

namespace NYLibrary
{
	class Collider;
	class SphereCollider;
	class InkSphereCollider;
	class SegmentCollider;
	class InkSegmentCollider;
	class TriangleCollider;
	class TrianglePolygonListCollider;

	class CollisionManager
	{
	public:
		//インスタンスを取得する
		static CollisionManager* GetInstance();
		//更新処理
		void Update();
		void AllTypeCheckAndCollisition(Collider * collider, Collider * _collider);
		//コライダーを追加する
		void AddCollider(Collider* collider) {
			colliderList.emplace_back(collider);
		}
	private:
		//当たり判定を取るか判定する
		bool CheckCallCollision(Collider* collider, Collider* collider_)const;
		//自クラスのインスタンス
		static std::unique_ptr<CollisionManager> collisionManager;
		//コライダーリスト(タグ、コライダー)
		std::vector<Collider*> colliderList;
		//bool Collision(const Sphere & sphereA, const Sphere & sphereB);
		//void Collision(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b, const D3DXVECTOR3 & _p2, Triangle* c);
		//bool Collision(const Sphere & _sphere, const Triangle & _triangle, D3DXVECTOR3 * _inter);
		//bool Collision(const SegmentCollider& segment, const TrianglePolygonListCollider& _triangle, D3DXVECTOR3 *_inter);
		//点と三角形の最近接点を計算
		//void Collision(const D3DXVECTOR3& _point, const TriangleCollider& _triangle, D3DXVECTOR3*);
		////線と三角形ポリゴンの当たり判定
		//bool Collision(SegmentCollider* segment, TrianglePolygonListCollider* _triangle);
		//線と三角形の当たり判定
	//	bool Collision(const SegmentCollider* segment, const Triangle* triangle, D3DXVECTOR3 _inter);

		//bool Collision(const  D3DXVECTOR3& _point, const TriangleCollider& _triangle);
		//void Collision(const D3DXVECTOR3& _point, const TriangleCollider& _triangle, D3DXVECTOR3* _closest);


		//線のコライダーか
		bool TypeCheckSegmentCollider(Collider* collider, Collider * collider_);
		//三角形ポリゴンのコライダーか
		bool TypeCheckTrianglePolygonListCollider(Collider* collider, Collider * collider_);


	};
};
