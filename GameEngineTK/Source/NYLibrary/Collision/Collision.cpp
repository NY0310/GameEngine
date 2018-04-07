#include "Collision.h"
#include "../Component/Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"
#include "../Component/Component.h"
#include "../Component/Collider/SegmentCollider/SegmentCollider.h"
#include "../Component/Collider/PlaneCollider/PlaneCollider.h"

using namespace NYLibrary;


bool NYLibrary::CheckSpere2Sphere(const Sphere * sphereA, const Sphere* sphereB)
{
	//球AとBの中心座標を求める
	D3DXVECTOR3 sub = sphereB->center - sphereA->center;
	//AとBの距離を計算
	float distanceSquare = sub.x * sub.x * +sub.y  * sub.y + sub.z * sub.z;

	//半径の和を計算
	float radiusSquare = sphereA->radius + sphereB->radius;

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
	//交点
	D3DXVECTOR3 inter;
	//
	float temp_distance = 0;

	// 角度判定用に地面とみなす角度の限界値<度>
	const float limit_angle = XMConvertToRadians(30.0f);

	// 逆行列を計算
	D3DXMATRIX startWorldInverse;
	D3DXMATRIX endWorldInverse;

	D3DXMatrixInverse(&startWorldInverse, nullptr, &trianglePolygonListCollider->GetObjectData()->GetWorldMatrix());
	D3DXMatrixInverse(&endWorldInverse, nullptr, &segment->GetEndWorldMatrix());

	
	// コピー
	D3DXVECTOR4 start;
	D3DXVECTOR4 end;
	// 線分をワールド座標からモデル座標系に引き込む
	D3DXVec3Transform(&start,&segment->start,&startWorldInverse);
	D3DXVec3Transform(&end, &segment->end, &startWorldInverse);



	// 線分の方向ベクトルを取得
	D3DXVECTOR4 segmentNormal = end - start;
	D3DXVec4Normalize(&segmentNormal, &segmentNormal);

	Segment* localSegment = new Segment;
	localSegment->start = D3DXVECTOR3(start.x, start.y, start.z);
	localSegment->end = D3DXVECTOR3(end.x, end.y, end.z);


	for (auto& triangle : trianglePolygonListCollider->GetTriangleList())
	{
		//ポリゴンの法線が無効な場合
		if (triangle.normal == D3DXVECTOR3(0, 0, 0))
			continue;


		// 上方向ベクトルと法線の内積
		// 長さが１のベクトル２同士の内積は、コサイン（ベクトルの内積の定義より）
		D3DXVECTOR3 vec3SegmentNormal = D3DXVECTOR3(segmentNormal.x, segmentNormal.y, segmentNormal.z);
		float cosine = D3DXVec3Dot(&-vec3SegmentNormal,&triangle.normal);
		// コサイン値から、上方向との角度差を計算
		float angle = acosf(cosine);
		// 上方向との角度が限界角より大きければ、面の傾きが大きいので、地面とみなさずスキップ


		if (CheckSegment2Triangle(localSegment, &triangle,inter))
		{
			// 線分の始点と衝突点の距離を計算（めりこみ距離）
			float nowTempDistance = D3DXVec3Length(&(inter-localSegment->start ));
			if (temp_distance < nowTempDistance)
			{
				temp_distance = nowTempDistance;
				//線との交点を設定する
				trianglePolygonListCollider->SetInter(inter);
				//当たった三角形ポリゴンを設定する
				trianglePolygonListCollider->SetCollisionTriangle(triangle);
				//当たったコライダーを登録する
				segment->AddCollisionCollider(trianglePolygonListCollider);
				trianglePolygonListCollider->AddCollisionCollider(segment);
				//当たった通知をする
				segment->OnCollision(trianglePolygonListCollider);
				trianglePolygonListCollider->OnCollision(segment);
			}
		}
	}
	delete localSegment;
	localSegment = nullptr;
}

//--------------------------------------------------------------------------------------------
// 説　明 : 線分（有向）と法線付き三角形の当たりチェック
// 引　数 : segment		光線の線分（start→endが正方向）
//			_tri		法線付き三角形
//			_inter		交点（省略可）
// 戻り値 : 交差しているか否か
// メ　モ : 裏面の当たりはとらない
//--------------------------------------------------------------------------------------------
bool NYLibrary::CheckSegment2Triangle(const Segment* segment, Triangle* triangle, D3DXVECTOR3& inter)
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

	inter = s;	// 交点をコピー

	return true;
}




void NYLibrary::IntersectPlane2Segment(SegmentCollider* inkSegmentColslider, PlaneCollider* planeCollider)
{
	D3DXVECTOR3 inter;
	if (IntersectPlane2Segment(&inter, inkSegmentColslider->start, inkSegmentColslider->end,*planeCollider))
	{
		planeCollider->SetInter(inter);
		//当たったコライダーを登録する
		inkSegmentColslider->AddCollisionCollider(planeCollider);
		planeCollider->AddCollisionCollider(inkSegmentColslider);
		//当たった通知をする
		inkSegmentColslider->OnCollision(planeCollider);
		planeCollider->OnCollision(inkSegmentColslider);
	}
}

//線分ABと平面の交点を計算する
bool NYLibrary::IntersectPlane2Segment(
	D3DXVECTOR3* inter, //戻り値　交点が見つかれば格納される
	D3DXVECTOR3  A,   //線分始点
	D3DXVECTOR3  B,   //線分終点
	Plane     PL) //平面
{
	////平面上の点P
	//D3DXVECTOR3 P = D3DXVECTOR3(PL.nX * PL.r, PL.nY * PL.r, PL.nZ * PL.r);

	////Pp0 PBベクトル
	//D3DXVECTOR3 PA = D3DXVECTOR3(P.x - A.x, P.y - A.y, P.z - A.z);
	//D3DXVECTOR3 PB = D3DXVECTOR3(P.x - B.x, P.y - B.y, P.z - B.z);

	////PA PBそれぞれを平面法線と内積
	//double dot_PA = PA.x * PL.nX + PA.y * PL.nY + PA.z * PL.nZ;
	//double dot_PB = PB.x * PL.nX + PB.y * PL.nY + PB.z * PL.nZ;

	////これは線端が平面上にあった時の計算の誤差を吸収しています。調整して使ってください。
	//if (abs(dot_PA) < 0.000001) { dot_PA = 0.0; }
	//if (abs(dot_PB) < 0.000001) { dot_PB = 0.0; }

	////交差判定
	//if (dot_PA == 0.0 && dot_PB == 0.0) {
	//	//両端が平面上にあり、交点を計算できない。
	//	//return false;
	//}
	//else
	//	if ((dot_PA >= 0.0 && dot_PB <= 0.0) ||
	//		(dot_PA <= 0.0 && dot_PB >= 0.0)) {
	//		//内積の片方がプラスで片方がマイナスなので、交差している

	//	}
	//	else {
	//		//交差していない
	//		return false;
	//	}

	//	//以下、交点を求める 

	//	D3DXVECTOR3 AB = D3DXVECTOR3(B.x - A.x, B.y - A.y, B.z - A.z);

	//	//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB
	//	double hiritu = abs(dot_PA) / (abs(dot_PA) + abs(dot_PB));

	//	inter->x = A.x + (AB.x * hiritu);
	//	inter->y = A.y + (AB.y * hiritu);
	//	inter->z = A.z + (AB.z * hiritu);

	//	return true;

	D3DXVECTOR3 AB = A - B;
	float t = (PL.h - D3DXVec3Dot(&PL.normal, &A)) / D3DXVec3Dot(&PL.normal,&AB);

	if (t >= 0.0f && t <= 1.0f) {
		*inter = A + t * AB;
		return true;
	}
	return false;
}
