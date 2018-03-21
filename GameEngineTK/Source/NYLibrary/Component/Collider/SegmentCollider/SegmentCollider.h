#pragma once
#include "../Collider.h"


namespace NYLibrary
{
	class ObjectData;

	//線分
	class SegmentCollider : public Collider, public Segment
	{
	public:
		//コンストラクタ
		SegmentCollider(const std::string& tag, ObjectData* matrixObject);
		//初期化
		void Initialize();
		//線と三角形ポリゴンの当たり判定
		void Collision(SegmentCollider* segment) {}
		//三角形ポリゴンとの当たり判定
		void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider);
	};
}
