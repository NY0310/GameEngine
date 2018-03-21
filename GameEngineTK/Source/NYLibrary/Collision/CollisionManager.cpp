#include "CollisionManager.h"
#include "../Component/Collider/Collider.h"
#include "../Component/Collider/SegmentCollider/SegmentCollider.h"
#include "../Component/Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"

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
			if (collider == collider_ || collider->GetTag() == collider_->GetTag())
			{
				break;
			}
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
}

bool CollisionManager::TypeCheckSegmentCollider(Collider * collider, Collider * _collider)
{
	SegmentCollider* segmentCollider = dynamic_cast<SegmentCollider*>(_collider);
	if (segmentCollider)
	{
		collider->Collision(segmentCollider);
		return true;
	}
	return false;
}

bool CollisionManager::TypeCheckTrianglePolygonListCollider(Collider * collider, Collider * collider_)
{
	TrianglePolygonListCollider* trianglePolygonListCollider = dynamic_cast<TrianglePolygonListCollider*>(collider);
	if (trianglePolygonListCollider)
	{
		collider->Collision(trianglePolygonListCollider);
		return true;
	}
	return false;
}






//bool CollisionManager::Collision(const Sphere&sphereA, const Sphere& sphereB)
//{
//	//球AとBの中心座標を求める
//	D3DXVECTOR3 sub = sphereB.center - sphereA.center;
//	//AとBの距離を計算
//	float distanceSquare = sub.x * sub.x * +sub.y  * sub.y + sub.z * sub.z;
//
//	//半径の和を計算
//	float radiusSquare = sphereA.radius + sphereB.radius;
//
//	radiusSquare = radiusSquare * radiusSquare;
//
//	//距離を半径の和より大きければ当たっていない
//	if (distanceSquare > radiusSquare)
//	{
//		return false;
//	}
//
//	//当たっている
//	return true;
//}
//
//// 3点から三角形を構築
//void CollisionManager::Collision(const D3DXVECTOR3& _p0, const D3DXVECTOR3& _p1, const D3DXVECTOR3& _p2, Triangle* _Triangle)
//{
//	//座標をコピー
//	_Triangle->p0 = _p0;
//	_Triangle->p1 = _p1;
//	_Triangle->p2 = _p2;
//	//法線ベクトルを計算
//	D3DXVECTOR3 P0_P1 = _p1 - _p0;
//	D3DXVECTOR3 P1_P2 = _p2 - _p1;
//	D3DXVECTOR3 P2_P0 = _p2 - _p0;
//
//
//
//	//2ベクトルに垂直なベクトルを得る
//	D3DXVec3Cross(&_Triangle->normal, &P0_P1, &P1_P2);
//	//ベクトルを長さを1に調整する
//	D3DXVec3Normalize(&_Triangle->normal, &_Triangle->normal);
//}
//
//
//--------------------------------------------------------------------------------------------
// 説　明 : 点と三角形の最近接点を計算
// 引　数 : _point	点
//			_triangle	三角形
//			_closest	最近接点(結果出力用）
// 戻り値 : なし
// メ　モ : 
//--------------------------------------------------------------------------------------------
//void CollisionManager::Collision(const D3DXVECTOR3& _point, const TriangleCollider& _triangle, D3DXVECTOR3* _closest)
//{
//	// _pointがP0の外側の頂点領域の中にあるかどうかチェック
//	D3DXVECTOR3 P0_P1 = _triangle.p1 - _triangle.p0;
//	D3DXVECTOR3 P0_P2 = _triangle.p2 - _triangle.p0;
//	D3DXVECTOR3 P0_PT = _point - _triangle.p0;
//
//	float d1 = D3DXVec3Dot(&P0_P1, &P0_PT);
//	float d2 = D3DXVec3Dot(&P0_P2, &P0_PT);
//
//
//	if (d1 <= 0.0f && d2 <= 0.0f)
//	{
//		// P0が最近傍
//		*_closest = _triangle.p0;
//		return;
//	}
//
//	// _pointがP1の外側の頂点領域の中にあるかどうかチェック
//	D3DXVECTOR3 P1_PT = _point - _triangle.p1;
//
//	float d3 = D3DXVec3Dot(&P0_P1, &P1_PT);
//	float d4 = D3DXVec3Dot(&P0_P2, &P1_PT);
//
//	if (d3 >= 0.0f && d4 <= d3)
//	{
//		// P1が最近傍
//		*_closest = _triangle.p1;
//		return;
//	}
//
//	// _pointがp0_P1の辺領域の中にあるかどうかチェックし、あれば_pointのP0_P1上に対する射影を返す
//	float vc = d1 * d4 - d3 * d2;
//
//	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
//	{
//		float v = d1 / (d1 - d3);
//		*_closest = _triangle.p0 + v * P0_P1;
//		return;
//	}
//
//	// _pointがP2の外側の頂点領域の中にあるかどうかチェック
//	D3DXVECTOR3 P2_PT = _point - _triangle.p2;
//
//	float d5 = D3DXVec3Dot(&P0_P1, &P2_PT);
//	float d6 = D3DXVec3Dot(&P0_P2, &P2_PT);
//
//	if (d6 >= 0.0f && d5 <= d6)
//	{
//		*_closest = _triangle.p2;
//		return;
//	}
//
//	// _pointがP0_P2の辺領域の中にあるかどうかチェックし、あれば_pointのP0_P2上に対する射影を返す
//	float vb = d5 * d2 - d1 * d6;
//
//	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
//	{
//		float w = d2 / (d2 - d6);
//		*_closest = _triangle.p0 + w * P0_P2;
//		return;
//	}
//
//	// _pointがP1_P2の辺領域の中にあるかどうかチェックし、あれば_pointのP1_P2上に対する射影を返す
//	float va = d3 * d6 - d5 * d4;
//
//	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
//	{
//		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
//
//		*_closest = _triangle.p1 + w * (_triangle.p2 - _triangle.p1);
//		return;
//	}
//
//	float denom = 1.0f / (va + vb + vc);
//	float v = vb * denom;
//	float w = vc * denom;
//	*_closest = _triangle.p0 + P0_P1 * v + P0_P2 * w;
//}

//bool CollisionManager::Collision(SegmentCollider* segment, TrianglePolygonListCollider* triangleList)
//{
//	D3DXVECTOR3 inter;
//	for (auto triangle : triangleList->GetTriangleList())
//	{
//		if (Collision(segment, triangle, inter))
//		{
//			triangleList->SetInter(inter);
//			triangleList->RegisterCollider(segment);
//			break;
//		}
//	}
//
//
//
//}

//// 三角形と点の当たり判定
//bool CollisionManager::Collision(const D3DXVECTOR3& _point, const Triangle& _triangle)
//{
//	//点と三角形は同一平面上にあるものとしています。同一平面上に無い場合は正しい結果になりません
//	//線上は外とみなします。
//	//ABCが三角形かどうかのチェックは省略...
//	// 点0→1、 1→2、 2→0 のベクトルをそれぞれ計算
//	D3DXVECTOR3 v01 = _triangle.p1 - _triangle.p0;
//	D3DXVECTOR3 v12 = _triangle.p2 - _triangle.p1;
//	D3DXVECTOR3 v20 = _triangle.p0 - _triangle.p2;
//
//	// 三角形の各頂点から点へのベクトルをそれぞれ計算
//	D3DXVECTOR3 v0p = _point - _triangle.p0;
//	D3DXVECTOR3 v1p = _point - _triangle.p1;
//	D3DXVECTOR3 v2p = _point - _triangle.p2;
//
//	// 各辺ベクトルと、点へのベクトルの外積を計算
//	D3DXVECTOR3 c0;
//	D3DXVECTOR3 c1;
//	D3DXVECTOR3 c2;
//	D3DXVec3Cross(&c0, &v01, &v0p);
//	D3DXVec3Cross(&c0, &v01, &v0p);
//	D3DXVec3Cross(&c0, &v01, &v0p);
//
//	// 内積で同じ方向かどうか調べる
//	float dot01 = D3DXVec3Dot(&c0, &c1);
//	float dot02 = D3DXVec3Dot(&c0, &c2);
//	// 外積ベクトルが全て同じ方向なら、三角形の内側に点がある
//	if (dot01 > 0 && dot02 > 0)
//	{
//		return true;
//	}
//
//	//三角形の外側に点がある
//	return false;
//}

//--------------------------------------------------------------------------------------------
// 説　明 : 球と法線付き三角形の当たりチェック
// 引　数 : _sphere		球
//			_tri		法線付き三角形
//			_inter		交点（省略可）
// 戻り値 : 交差しているか否か
// メ　モ : 裏面の当たりはとらない
//--------------------------------------------------------------------------------------------
//bool CollisionManager::Collision(const SegmentCollider& _sphere, const TrianglePolygonListCollider& _triangle, D3DXVECTOR3 *_inter)
//{
//	D3DXVECTOR3 p;
//
//	// 球の中心に対する最近接点である三角形上にある点pを見つける
//	Collision(_sphere.center, _triangle, &p);
//
//	D3DXVECTOR3 v = p - _sphere.center;
//
//	// 球と三角形が交差するのは、球の中心から点pまでの距離が球の半径よりも小さい場合
//	if (D3DXVec3Dot(&v, &v) <= _sphere.radius * _sphere.radius)
//	{
//		if (_inter)
//		{
//			*_inter = p;
//		}
//
//		return true;
//	}
//
//	return false;
//
//	// 球と平面（三角形が乗っている平面）の当たり判定
//	// 球と平面の距離を計算
//	float ds = D3DXVec3Dot(&_sphere.center, &_triangle.normal);
//	float dt = D3DXVec3Dot(&_triangle.p0, &_triangle.normal);
//	float dist = ds - dt;
//	// 距離が半径以上なら、当たらない
//	if (fabsf(dist) > _sphere.radius)	return false;
//
//	// 中心点を平面に射影したとき、三角形の内側にあれば、当たっている
//	// 射影した座標
//	D3DXVECTOR3 center = -dist * _triangle.normal + _sphere.center;
//
//	// 三角形の外側になければ、当たっていない
//	if (!CheckPoint2Triangle(center, _triangle))	return false;
//
//	if (_inter)
//	{
//		*_inter = center;	// 交点をコピー
//	}
//
//	return true;
//}


