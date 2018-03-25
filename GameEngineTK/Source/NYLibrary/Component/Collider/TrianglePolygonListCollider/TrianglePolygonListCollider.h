#pragma once
#include "../Collider.h"
#include "../../Element.h"


namespace NYLibrary
{
	/// <summary>
	/// 三角形ポリゴン
	/// </summary>
	class TrianglePolygonListCollider : public Collider
	{
	public:
		//コンストラクタ
		TrianglePolygonListCollider(ObjectData* objectData);
		//三角形ポリゴンを取得する
		std::vector<Triangle>& GetTriangleList() { return triangleList; }
		//初期化処理
		void Initialize()final;
		//更新処理
		void Update()final;
		//交点を設定する
		void SetInter(const D3DXVECTOR3& inter) { this->inter = inter; }
		//当たった交点を取得
		const D3DXVECTOR3& GetInter() { return inter; }
		//当たった三角形を設定する
		void SetCollisionTriangle(Triangle triangle) { collisionTriangle = triangle; }
		//線と三角形ポリゴンの当たり判定
		void Collision(SegmentCollider* segment);
		//三角形ポリゴンとの当たり判定
		void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider) {}
		//当たった三角形を取得
		Triangle GetCollisionTriangle() { return collisionTriangle; }
	private:
		//三角形ポリゴン
		std::vector<Triangle> triangleList;
		//当たった三角形
		Triangle collisionTriangle;
		//当たった交点
		D3DXVECTOR3 inter;
	};
}