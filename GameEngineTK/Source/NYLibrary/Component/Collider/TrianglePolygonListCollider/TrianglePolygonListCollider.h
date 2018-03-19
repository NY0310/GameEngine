#pragma once
#include "../Collider.h"

namespace NYLibrary
{
	/// <summary>
	/// 三角形ポリゴン
	/// </summary>
	class TrianglePolygonListCollider : public Collider
	{
	public:
		TrianglePolygonListCollider() = default;
		//TrianglePolygonListCollider(const std::string& tag,const std::vector<TriangleCollider*>& triangleCollider):Collider(tag,nullptr){}
		//三角形ポリゴンを取得する
		const std::vector<Triangle*>& GetTriangleList() { return triangleList; }
		//更新処理
		void Update() {}
		//交点を設定する
		void SetInter(const D3DXVECTOR3& inter) { this->inter = inter; }
		//線と三角形ポリゴンの当たり判定
		void Collision(SegmentCollider* segment);
		//当たった三角形の設定
		void SetCollisionTriangle(Triangle* triangle) { collisionTriangle = triangle; }
	private:
		//三角形ポリゴン
		std::vector<Triangle*> triangleList;
		//当たった三角形
		Triangle* collisionTriangle;
		//当たった交点
		D3DXVECTOR3 inter;
	};
}