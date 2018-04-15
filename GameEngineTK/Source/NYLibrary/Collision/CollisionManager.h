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
	class PlaneCollider;

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
		//当たり判定を行うインターバルを設定
		void SetCollisitonIntervalFrame(int frame) { collisitonIntervalFrame = frame; frameCnt = 0; }

		//コライダーの削除
		void EraseCollider(Collider* collider);
	private:
		//コンストラクタ
		CollisionManager() { collisitonIntervalFrame = 0; }
		//当たり判定を取るか判定する
		bool CheckCallCollision(Collider* collider, Collider* collider_)const;
		//自クラスのインスタンス
		static std::unique_ptr<CollisionManager> collisionManager;
		//コライダーリスト(タグ、コライダー)
		std::vector<Collider*> colliderList;
		//線のコライダーか
		bool TypeCheckSegmentCollider(Collider* collider, Collider * collider_);
		//三角形ポリゴンのコライダーか
		bool TypeCheckTrianglePolygonListCollider(Collider* collider, Collider * collider_);

		bool TypeCheckPlaneCollider(Collider * collider, Collider * collider_);

		//当たり判定を行う
		int collisitonIntervalFrame;
		//フレームカウント
		int frameCnt;
	};
};
