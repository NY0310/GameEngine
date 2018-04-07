#include "CollisionManager.h"
#include "../Component/Collider/Collider.h"
#include "../Component/Collider/SegmentCollider/SegmentCollider.h"
#include "../Component/Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"
#include "../Component/Collider/PlaneCollider/PlaneCollider.h"

using namespace std;
using namespace NYLibrary;

unique_ptr<CollisionManager> CollisionManager::collisionManager;


/// <summary>
/// CollisionManagerを取得する
/// </summary>
/// <returns>CollisionManager</returns>
CollisionManager * CollisionManager::GetInstance()
{
	if (!collisionManager)
	{
		collisionManager = make_unique<CollisionManager>();
	}
	return collisionManager.get();
}

void CollisionManager::Update()
{

	for (auto collider : colliderList)
	{
		for (auto collider_ : colliderList)
		{
			if (!CheckCallCollision(collider, collider_))continue;
			AllTypeCheckAndCollisition(collider, collider_);

		}

	}
}

void CollisionManager::AllTypeCheckAndCollisition(Collider* collider, Collider* _collider)
{
	if (TypeCheckSegmentCollider(collider, _collider))
		return;
	if (TypeCheckTrianglePolygonListCollider(collider, _collider))
		return;
	if (TypeCheckPlaneCollider(collider, _collider))
		return;

	
}


/// <summary>
/// 当たり判定を取るか判定する
/// </summary>
/// <param name="collider">コライダー</param>
/// <param name="collider_">コライダー</param>
/// <returns></returns>
bool CollisionManager::CheckCallCollision(Collider * collider, Collider * collider_)const
{
	if (collider == collider_)
	{
		return false;
	}
	if (collider->GetTag() == collider_->GetTag())
	{
		return false;
	}
	if (!collider->GetActive() || !collider_->GetActive())
	{
		return false;
	}
	//if (!CheckSpere2Sphere(collider,collider_))
	//{
	//	return false;
	//}

	return true;
}

/// <summary>
/// 引数1が線分コライダーだったら当たり判定を行う	
/// </summary>
/// <param name="collider">コライダー</param>
/// <param name="collider_">コライダー2</param>
/// <returns>引数1が線分コライダーか</returns>
bool CollisionManager::TypeCheckSegmentCollider(Collider * collider, Collider * collider_)
{
	SegmentCollider* segmentCollider = dynamic_cast<SegmentCollider*>(collider_);
	if (segmentCollider)
	{
		collider->Collision(segmentCollider);
		return true;
	}
	return false;
}

/// <summary>
/// 引数1が三角形ポリゴンコライダーか調べtrueなら当たり判定を行う
/// </summary>
/// <param name="collider">コライダー</param>
/// <param name="collider_">コライダー2</param>
/// <returns>引数1が三角形ポリゴンだったか</returns>
bool CollisionManager::TypeCheckTrianglePolygonListCollider(Collider * collider, Collider * collider_)
{
	TrianglePolygonListCollider* trianglePolygonListCollider = dynamic_cast<TrianglePolygonListCollider*>(collider_);
	if (trianglePolygonListCollider)
	{
		collider->Collision(trianglePolygonListCollider);
		return true;
	}
	return false;
}


/// <summary>
/// 引数1がプレーンコライダーか調べてtrueなら当たり判定を行う
/// </summary>
/// <param name="collider">コライダー</param>
/// <param name="collider_">コライダー2</param>
/// <returns>引数1がプレーンコライダーか</returns>
bool CollisionManager::TypeCheckPlaneCollider(Collider * collider, Collider * collider_)
{
	PlaneCollider* planeCollider = dynamic_cast<PlaneCollider*>(collider_);
	if (planeCollider)
	{
		collider->Collision(planeCollider);
		return true;
	}
	return false;
}





