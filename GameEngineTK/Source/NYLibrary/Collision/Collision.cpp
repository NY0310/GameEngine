#include "Collision.h"
#include "../Component/Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"
#include "../Component/Component.h"

using namespace DirectX::SimpleMath;
using namespace NYLibrary;


bool NYLibrary::CheckSpere2Sphere(const Sphere&sphereA, const Sphere& sphereB)
{
	//球AとBの中心座標を求める
	D3DXVECTOR3 sub = sphereB.center - sphereA.center;
	//AとBの距離を計算
	float distanceSquare = sub.x * sub.x * +sub.y  * sub.y + sub.z * sub.z;

	//半径の和を計算
	float radiusSquare = sphereA.radius + sphereB.radius;

	radiusSquare = radiusSquare * radiusSquare;

	//距離を半径の和より大きければ当たっていない
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	//当たっている
	return true;
}

// 3点から三角形を構築
void NYLibrary::ComputeTriangle(const D3DXVECTOR3& _p0, const D3DXVECTOR3& _p1, const D3DXVECTOR3& _p2, Triangle* _Triangle)
{
	//座標をコピー
	_Triangle->p0 = _p0;
	_Triangle->p1 = _p1;
	_Triangle->p2 = _p2;
	//法線ベクトルを計算
	D3DXVECTOR3 P0_P1 = _p1 - _p0;
	D3DXVECTOR3 P1_P2 = _p2 - _p1;
	D3DXVECTOR3 P2_P0 = _p2 - _p0;



	//2ベクトルに垂直なベクトルを得る
	D3DXVec3Cross(&_Triangle->normal,&P0_P1,&P1_P2);
	//ベクトルを長さを1に調整する
	D3DXVec3Normalize(&_Triangle->normal, &_Triangle->normal);
}


//--------------------------------------------------------------------------------------------
// 説　明 : 点と三角形の最近接点を計算
// 引　数 : _point	点
//			_triangle	三角形
//			_closest	最近接点(結果出力用）
// 戻り値 : なし
// メ　モ : 
//--------------------------------------------------------------------------------------------
void NYLibrary::ClosestPtPoint2Triangle(const D3DXVECTOR3& _point, const Triangle& _triangle, D3DXVECTOR3* _closest)
{
	// _pointがP0の外側の頂点領域の中にあるかどうかチェック
	D3DXVECTOR3 P0_P1 = _triangle.p1 - _triangle.p0;
	D3DXVECTOR3 P0_P2 = _triangle.p2 - _triangle.p0;
	D3DXVECTOR3 P0_PT = _point - _triangle.p0;

	float d1 =  D3DXVec3Dot(&P0_P1, &P0_PT);
	float d2 = D3DXVec3Dot(&P0_P2, &P0_PT);


	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// P0が最近傍
		*_closest = _triangle.p0;
		return;
	}

	// _pointがP1の外側の頂点領域の中にあるかどうかチェック
	D3DXVECTOR3 P1_PT = _point - _triangle.p1;

	float d3 = D3DXVec3Dot(&P0_P1, &P1_PT);
	float d4 = D3DXVec3Dot(&P0_P2,&P1_PT);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// P1が最近傍
		*_closest = _triangle.p1;
		return;
	}

	// _pointがp0_P1の辺領域の中にあるかどうかチェックし、あれば_pointのP0_P1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;

	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*_closest = _triangle.p0 + v * P0_P1;
		return;
	}

	// _pointがP2の外側の頂点領域の中にあるかどうかチェック
	D3DXVECTOR3 P2_PT = _point - _triangle.p2;

	float d5 = D3DXVec3Dot(&P0_P1, &P2_PT);
	float d6 = D3DXVec3Dot(&P0_P2, &P2_PT);

	if (d6 >= 0.0f && d5 <= d6)
	{
		*_closest = _triangle.p2;
		return;
	}

	// _pointがP0_P2の辺領域の中にあるかどうかチェックし、あれば_pointのP0_P2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*_closest = _triangle.p0 + w * P0_P2;
		return;
	}

	// _pointがP1_P2の辺領域の中にあるかどうかチェックし、あれば_pointのP1_P2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

		*_closest = _triangle.p1 + w * (_triangle.p2 - _triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*_closest = _triangle.p0 + P0_P1 * v + P0_P2 * w;
}

// 三角形と点の当たり判定
bool NYLibrary::CheckPoint2Triangle(const D3DXVECTOR3& _point, const Triangle& _triangle)
{
	//点と三角形は同一平面上にあるものとしています。同一平面上に無い場合は正しい結果になりません
	//線上は外とみなします。
	//ABCが三角形かどうかのチェックは省略...
	// 点0→1、 1→2、 2→0 のベクトルをそれぞれ計算
	D3DXVECTOR3 v01 = _triangle.p1 - _triangle.p0;
	D3DXVECTOR3 v12 = _triangle.p2 - _triangle.p1;
	D3DXVECTOR3 v20 = _triangle.p0 - _triangle.p2;

	// 三角形の各頂点から点へのベクトルをそれぞれ計算
	D3DXVECTOR3 v0p = _point - _triangle.p0;
	D3DXVECTOR3 v1p = _point - _triangle.p1;
	D3DXVECTOR3 v2p = _point - _triangle.p2;

	// 各辺ベクトルと、点へのベクトルの外積を計算
	D3DXVECTOR3 c0;
	D3DXVECTOR3 c1;
	D3DXVECTOR3 c2;
	D3DXVec3Cross(&c0, &v01, &v0p);
	D3DXVec3Cross(&c0, &v01, &v0p);
	D3DXVec3Cross(&c0, &v01, &v0p);

	// 内積で同じ方向かどうか調べる
	float dot01 = D3DXVec3Dot(&c0, &c1);
	float dot02 = D3DXVec3Dot(&c0,&c2);
	// 外積ベクトルが全て同じ方向なら、三角形の内側に点がある
	if (dot01 > 0 && dot02 > 0)
	{
		return true;
	}

	//三角形の外側に点がある
	return false;
}



//--------------------------------------------------------------------------------------------
// 説　明 : 球と法線付き三角形の当たりチェック
// 引　数 : _sphere		球
//			_tri		法線付き三角形
//			_inter		交点（省略可）
// 戻り値 : 交差しているか否か
// メ　モ : 裏面の当たりはとらない
//--------------------------------------------------------------------------------------------
bool NYLibrary::CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _triangle, D3DXVECTOR3 *_inter)
{
	D3DXVECTOR3 p;

	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(_sphere.center, _triangle, &p);

	D3DXVECTOR3 v = p - _sphere.center;

	// 球と三角形が交差するのは、球の中心から点pまでの距離が球の半径よりも小さい場合
	if (D3DXVec3Dot(&v,&v) <= _sphere.radius * _sphere.radius)
	{
		if (_inter)
		{
			*_inter = p;
		}

		return true;
	}

	return false;

	// 球と平面（三角形が乗っている平面）の当たり判定
	// 球と平面の距離を計算
	float ds = D3DXVec3Dot(&_sphere.center,&_triangle.normal);
	float dt = D3DXVec3Dot(&_triangle.p0,&_triangle.normal);
	float dist = ds - dt;
	// 距離が半径以上なら、当たらない
	if (fabsf(dist) > _sphere.radius)	return false;

	// 中心点を平面に射影したとき、三角形の内側にあれば、当たっている
	// 射影した座標
	D3DXVECTOR3 center = -dist * _triangle.normal + _sphere.center;

	// 三角形の外側になければ、当たっていない
	if (!CheckPoint2Triangle(center, _triangle))	return false;

	if (_inter)
	{
		*_inter = center;	// 交点をコピー
	}

	return true;
}


//--------------------------------------------------------------------------------------------
// 説　明 : 線分（有向）と法線付き三角形の当たりチェック
// 引　数 : _segment		光線の線分（start→endが正方向）
//			_tri		法線付き三角形
//			_inter		交点（省略可）
// 戻り値 : 交差しているか否か
// メ　モ : 裏面の当たりはとらない
//--------------------------------------------------------------------------------------------
//bool NYLibrary::CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, D3DXVECTOR3 *_inter)
//{
//	const float epsilon = -1.0e-5f;	// 誤差吸収用の微小な値
//	D3DXVECTOR3 	LayV;		// 線分の終点→始点
//	D3DXVECTOR3 	tls;		// 三角形の頂点0→線分の始点
//	D3DXVECTOR3 	tle;		// 三角形の頂点0→線分の終点
//	float 	distl0;
//	float 	distl1;
//	float 	dp;
//	float 	denom;
//	float 	t;
//	D3DXVECTOR3	s;			// 直線と平面との交点
//	D3DXVECTOR3 	st0;		// 交点→三角形の頂点0
//	D3DXVECTOR3 	st1;		// 交点→三角形の頂点1
//	D3DXVECTOR3 	st2;		// 交点→三角形の頂点2
//	D3DXVECTOR3 	t01;		// 三角形の頂点0→頂点1
//	D3DXVECTOR3 	t12;		// 三角形の頂点1→頂点2
//	D3DXVECTOR3 	t20;		// 三角形の頂点2→頂点0
//	D3DXVECTOR3	m;
//
//	// 線分の始点が三角系の裏側にあれば、当たらない
//	tls = _segment.start - _triangle.p0;
//	distl0 = D3DXVec3Dot(&tls,&_triangle.normal);	// 線分の始点と平面の距離
//	if (distl0 <= epsilon) return false;
//
//	// 線分の終点が三角系の表側にあれば、当たらない
//	tle = _segment.end - _triangle.p0;
//	distl1 = D3DXVec3Dot(&tle,&_triangle.normal);	// 線分の終点と平面の距離
//	if (distl1 >= -epsilon) return false;
//
//	// 直線と平面との交点sを取る
//	denom = distl0 - distl1;
//	t = distl0 / denom;
//	LayV = _segment.end - _segment.start;	// 線分の方向ベクトルを取得
//	s = t * LayV + _segment.start;
//
//	// 交点が三角形の内側にあるかどうかを調べる。
//	// 三角形の内側にある場合、交点から各頂点へのベクトルと各辺ベクトルの外積（三組）が、全て法線と同じ方向を向く
//	// 一つでも方向が一致しなければ、当たらない。
//	st0 = _triangle.p0 - s;
//	t01 = _triangle.p1 - _triangle.p0;
//	D3DXVec3Cross(&m,&st0,&t01);
//	dp = D3DXVec3Dot(&m,&_triangle.normal);
//	if (dp <= epsilon) return false;
//
//	st1 = _triangle.p1 - s;
//	t12 = _triangle.p2 - _triangle.p1;
//	D3DXVec3Cross(&m,&st1,&t12);
//	dp = D3DXVec3Dot(&m,&_triangle.normal);
//	if (dp <= epsilon) return false;
//
//	st2 = _triangle.p2 - s;
//	t20 = _triangle.p0 - _triangle.p2;
//	D3DXVec3Cross(&m,&st2,&t20);
//	dp = D3DXVec3Dot(&m,&_triangle.normal);
//	if (dp <= epsilon) return false;
//
//	if (_inter)
//	{
//		*_inter = s;	// 交点をコピー
//	}
//
//	return true;
//}

void NYLibrary::CheckSegment2AllTriangle(SegmentCollider * segment, TrianglePolygonListCollider * trianglePolygonListCollider)
{
	D3DXVECTOR3 inter;
	for (auto& triangle : trianglePolygonListCollider->GetTriangleList())
	{
		if (CheckSegment2Triangle(segment, triangle,&inter))
		{
			//線との交点を設定する
			trianglePolygonListCollider->SetInter(inter);
			//当たった三角形ポリゴンを設定する
			trianglePolygonListCollider->SetCollisionTriangle(triangle);
			//当たったコライダーを登録する
			segment->AddCollisionCollider(trianglePolygonListCollider);
			trianglePolygonListCollider->AddCollisionCollider(segment);
			//当たった通知をする
			segment->OnCollision();
			trianglePolygonListCollider->OnCollision();

		}
	}
}

//--------------------------------------------------------------------------------------------
// 説　明 : 線分（有向）と法線付き三角形の当たりチェック
// 引　数 : segment		光線の線分（start→endが正方向）
//			_tri		法線付き三角形
//			_inter		交点（省略可）
// 戻り値 : 交差しているか否か
// メ　モ : 裏面の当たりはとらない
//--------------------------------------------------------------------------------------------
bool NYLibrary::CheckSegment2Triangle(const SegmentCollider* segment, Triangle* triangle, D3DXVECTOR3* inter)
{
	const float epsilon = -1.0e-5f;	// 誤差吸収用の微小な値
	D3DXVECTOR3 	LayV;		// 線分の終点→始点
	D3DXVECTOR3 	tls;		// 三角形の頂点0→線分の始点
	D3DXVECTOR3 	tle;		// 三角形の頂点0→線分の終点
	float 	distl0;
	float 	distl1;
	float 	dp;
	float 	denom;
	float 	t;
	D3DXVECTOR3	s;			// 直線と平面との交点
	D3DXVECTOR3 	st0;		// 交点→三角形の頂点0
	D3DXVECTOR3 	st1;		// 交点→三角形の頂点1
	D3DXVECTOR3 	st2;		// 交点→三角形の頂点2
	D3DXVECTOR3 	t01;		// 三角形の頂点0→頂点1
	D3DXVECTOR3 	t12;		// 三角形の頂点1→頂点2
	D3DXVECTOR3 	t20;		// 三角形の頂点2→頂点0
	D3DXVECTOR3	m;

	// 線分の始点が三角系の裏側にあれば、当たらない
	tls = segment->start - triangle->p0;
	distl0 = D3DXVec3Dot(&tls, &triangle->normal);	// 線分の始点と平面の距離
	if (distl0 <= epsilon) return false;

	// 線分の終点が三角系の表側にあれば、当たらない
	tle = segment->end - triangle->p0;
	distl1 = D3DXVec3Dot(&tle, &triangle->normal);	// 線分の終点と平面の距離
	if (distl1 >= -epsilon) return false;

	// 直線と平面との交点sを取る
	denom = distl0 - distl1;
	t = distl0 / denom;
	LayV = segment->end - segment->start;	// 線分の方向ベクトルを取得
	s = t * LayV + segment->start;

	// 交点が三角形の内側にあるかどうかを調べる。
	// 三角形の内側にある場合、交点から各頂点へのベクトルと各辺ベクトルの外積（三組）が、全て法線と同じ方向を向く
	// 一つでも方向が一致しなければ、当たらない。
	st0 = triangle->p0 - s;
	t01 = triangle->p1 - triangle->p0;
	D3DXVec3Cross(&m, &st0, &t01);
	dp = D3DXVec3Dot(&m, &triangle->normal);
	if (dp <= epsilon) return false;

	st1 = triangle->p1 - s;
	t12 = triangle->p2 - triangle->p1;
	D3DXVec3Cross(&m, &st1, &t12);
	dp = D3DXVec3Dot(&m, &triangle->normal);
	if (dp <= epsilon) return false;

	st2 = triangle->p2 - s;
	t20 = triangle->p0 - triangle->p2;
	D3DXVec3Cross(&m, &st2, &t20);
	dp = D3DXVec3Dot(&m, &triangle->normal);
	if (dp <= epsilon) return false;

	inter = &s;	// 交点をコピー

	return true;
}
