#pragma once
#include "../Collider.h"

namespace NYLibrary
{
	class ObjectData;

	//線分
	class PlaneCollider : public Collider, public Plane
	{
	public:
		//コンストラクタ
		PlaneCollider(ObjectData* objectData);
		virtual ~PlaneCollider() = default;
		//初期化
		void Initialize();
		//更新
		void Update();
		//線と三角形ポリゴンの当たり判定
		void Collision(PlaneCollider* segment) {}
		//三角形ポリゴンとの当たり判定
		void Collision(SegmentCollider* segmentCollider);
		//交点を設定する
		void SetInter(const D3DXVECTOR3& inter) { this->inter = inter; }
		//当たった交点を取得
		const D3DXVECTOR3& GetInter() { return inter; }

	private:
		//当たった交点
		D3DXVECTOR3 inter;

	};
}
