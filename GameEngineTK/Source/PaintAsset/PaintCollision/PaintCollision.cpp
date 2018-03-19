#include "PaintCollision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace NYLibrary;

//--------------------------------------------------------------------------------------
// 地形と線分の交差判定
// segment : 線分
// （出力）inter : 交点（ポリゴンの平面上で、点との再接近点の座標を返す）
//--------------------------------------------------------------------------------------
bool PaintCollision::IntersectSegment(Segment* segment, D3DXVECTOR2& uv)
{

////	if (mesh.pIndexBuffer == nullptr) return false;
//
//	// 大きい数字で初期化
//	float distance = 1.0e5;
//	// 角度判定用に地面とみなす角度の限界値<度>
//	const float limit_angle = XMConvertToRadians(30.0f);
//	Vector3 l_inter;
//
//
//	// 逆行列を計算
//	D3DXMATRIX d3dWorldLocal;
//	D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldMatrix);
//	Matrix WorldLocal = Math::D3DXMATRIXToMatrix(d3dWorldLocal);
//
//
//	// コピー
//	Segment localSegment = *segment;
//	// 線分をワールド座標からモデル座標系に引き込む
//	localSegment.Start = Vector3::Transform(localSegment.Start, WorldLocal);
//	localSegment.End = Vector3::Transform(localSegment.End, WorldLocal);
//	// 線分の方向ベクトルを取得
//	Vector3 segmentNormal = localSegment.End - localSegment.Start;
//	segmentNormal.Normalize();
//
//	// 全ての三角形について
//	for (auto triangle : triangles)
//	{
//		float temp_distance;
//		Vector3 temp_inter;
//		if (triangle.Normal == Vector3(0, 0, 0))
//			continue;
//		// 上方向ベクトルと法線の内積
//		// 長さが１のベクトル２同士の内積は、コサイン（ベクトルの内積の定義より）
//		float cosine = -segmentNormal.Dot(triangle.Normal);
//		//// コサイン値から、上方向との角度差を計算
//		//float angle = acosf(cosine);
//		//// 上方向との角度が限界角より大きければ、面の傾きが大きいので、地面とみなさずスキップ
//		//if ( angle > limit_angle ) continue;
//
//		//--高速版--
//		const float limit_cosine = cosf(limit_angle);
//		// コサインが１のときにベクトル間の角度は0度であり、ベクトルの角度差が大きいほど、コサインは小さいので、
//		// コサイン値のまま比較すると、角度の比較の場合と大小関係が逆である
//		// つまり、コサイン値が一定値より小さければ、面の傾きが大きいので、地面とみなさずスキップ
//		//if (cosine < limit_cosine) continue;
//		//--高速版ここまで--
//		// 線分と三角形（ポリゴン）の交差判定
//		if (CheckSegment2Triangle(localSegment, triangle, &temp_inter))
//		{
//			// 線分の始点と衝突点の距離を計算（めりこみ距離）
//			temp_distance = Vector3::Distance(localSegment.start, temp_inter);
//			// めりこみ具合がここまでで最小なら
//			{
//				// 衝突点の座標、めりこみ距離を記録
//				l_inter = temp_inter;
//				distance = temp_distance;
//
//
//				Vector3 p1 = triangle.P0;
//				Vector3 p2 = triangle.P1;
//				Vector3 p3 = triangle.P2;
//				Vector3 p = l_inter;
//
//				Vector2 uv1 = triangle.Uv0;
//				Vector2 uv2 = triangle.Uv1;
//				Vector2 uv3 = triangle.Uv2;
//
//				//塗られるオブジェクトのワールド座標をかける
//				//PerspectiveCollect(透視投影を考慮したUV補間)
//				Matrix mvp = Math::D3DXMATRIXToMatrix(worldMatrix * camera->GetView()* camera->GetProjection());
//				//各点をProjectionSpaceへの変換
//				Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 0));
//				Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 0));
//				Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 0));
//				Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 0));
//
//
//				//通常座標への変換(ProjectionSpace)
//				Vector2 p1_n = Vector2(p1_p.x, p1_p.y) / p1_p.w;
//				Vector2 p2_n = Vector2(p2_p.x, p2_p.y) / p2_p.w;
//				Vector2 p3_n = Vector2(p3_p.x, p3_p.y) / p3_p.w;
//				Vector2 p_n = Vector2(p_p.x, p_p.y) / p_p.w;
//
//
//				//頂点のなす三角形を点pにより3分割し、必要になる面積を計算
//				float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
//				float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
//				float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
//				//面積比からuvを補間
//				float u = s1 / s;
//				float v = s2 / s;
//				float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
//				Vector2 _uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);
//				uv = Math::VectorToD3DXVECTOR2(_uv);
//				return true;
//			}
//		}
//	}
//
//
//
	return false;
}



//--------------------------------------------------------------------------------------
// 三角形と円の交差判定
// segment : 線分
// （出力）inter : 交点（ポリゴンの平面上で、点との再接近点の座標を返す）
//--------------------------------------------------------------------------------------
bool PaintCollision::IntersectSphere(Sphere* sphere, D3DXVECTOR2 & uv)
{

	//// 逆行列を計算
	//D3DXMATRIX d3dWorldLocal;
	//D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldMatrix);
	//Matrix WorldLocal = Math::D3DXMATRIXToMatrix(d3dWorldLocal);

	//// ヒットフラグを初期化
	//bool hit = false;
	//// 大きい数字で初期化
	//float over_length = 1.0e5;
	//Vector3 l_inter;
	//Vector3 l_normal;
	////	Vector3 l_down;
	//// スケールを取得
	//float scale = sphere->Radius * 2;

	//// 球をコピー
	//Sphere* localsphere = sphere;

	//// スケール0の場合、判定しない
	//if (scale <= 1.0e-10) return false;

	//// 球の中心点をワールド座標からモデル座標系に引き込む
	//localsphere->Center = Vector3::Transform(sphere->Center, WorldLocal);
	//// 半径をワールドをワールド座標系からモデル座標系に変換
	//localsphere->radius = sphere->radius / scale;
	//for (auto triangle : triangles)
	//{
	//	{
	//		float temp_over_length;
	//		Vector3 temp_inter;

	//		Triangle& tri = triangle;

	//		// 三角形と球の当たり判定
	//		if (CheckSphere2Triangle(*localsphere, tri, &temp_inter))
	//		{

	//			Vector3 p1 = triangle.P0;
	//			Vector3 p2 = triangle.P1;
	//			Vector3 p3 = triangle.P2;
	//			Vector3 p = temp_inter;

	//			Vector2 uv1 = triangle.Uv0;
	//			Vector2 uv2 = triangle.Uv1;
	//			Vector2 uv3 = triangle.Uv2;

	//			//塗られるオブジェクトのワールド座標をかける
	//			//PerspectiveCollect(透視投影を考慮したUV補間)
	//			Matrix mvp = Math::D3DXMATRIXToMatrix(worldMatrix * camera->GetView()* camera->GetProjection());
	//			//各点をProjectionSpaceへの変換
	//			Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 1));
	//			Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 1));
	//			Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 1));
	//			Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 1));


	//			//通常座標への変換(ProjectionSpace)
	//			Vector2 p1_n = Vector2(p1_p.x, p1_p.y) / p1_p.w;
	//			Vector2 p2_n = Vector2(p2_p.x, p2_p.y) / p2_p.w;
	//			Vector2 p3_n = Vector2(p3_p.x, p3_p.y) / p3_p.w;
	//			Vector2 p_n = Vector2(p_p.x, p_p.y) / p_p.w;


	//			//頂点のなす三角形を点pにより3分割し、必要になる面積を計算
	//			float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
	//			float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
	//			float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
	//			//面積比からuvを補間
	//			float u = s1 / s;
	//			float v = s2 / s;
	//			float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
	//			Vector2 _uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);

	//			uv = Math::VectorToD3DXVECTOR2(_uv);
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	return false;
}

Vector4 PaintCollision::MatrixTimes(const Matrix& matrix, const Vector4& vector)
{
	Vector4 vec;
	vec.x = matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * vector.w;
	vec.y = matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * vector.w;
	vec.z = matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * vector.w;
	vec.w = matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * vector.w;
	return vec;
}



