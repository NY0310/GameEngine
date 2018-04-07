#include "CollisionManager.h"
#include "../Component/Collider/Collider.h"
#include "../Component/Collider/SegmentCollider/SegmentCollider.h"
#include "../Component/Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"
#include "../Component/Collider/PlaneCollider/PlaneCollider.h"

using namespace std;
using namespace NYLibrary;

unique_ptr<CollisionManager> CollisionManager::collisionManager;


/// <summary>
/// CollisionManager���擾����
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
/// �����蔻�����邩���肷��
/// </summary>
/// <param name="collider">�R���C�_�[</param>
/// <param name="collider_">�R���C�_�[</param>
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
/// ����1�������R���C�_�[�������瓖���蔻����s��	
/// </summary>
/// <param name="collider">�R���C�_�[</param>
/// <param name="collider_">�R���C�_�[2</param>
/// <returns>����1�������R���C�_�[��</returns>
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
/// ����1���O�p�`�|���S���R���C�_�[������true�Ȃ瓖���蔻����s��
/// </summary>
/// <param name="collider">�R���C�_�[</param>
/// <param name="collider_">�R���C�_�[2</param>
/// <returns>����1���O�p�`�|���S����������</returns>
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
/// ����1���v���[���R���C�_�[�����ׂ�true�Ȃ瓖���蔻����s��
/// </summary>
/// <param name="collider">�R���C�_�[</param>
/// <param name="collider_">�R���C�_�[2</param>
/// <returns>����1���v���[���R���C�_�[��</returns>
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





