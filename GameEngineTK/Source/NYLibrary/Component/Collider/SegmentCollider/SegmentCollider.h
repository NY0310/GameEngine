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
		SegmentCollider(ObjectData* objectData);
		SegmentCollider() = delete;
		virtual ~SegmentCollider() = default;
		//初期化
		void Initialize();
		//更新
		void Update();
		//
		void CalcSegmentPosition();
		//線と三角形ポリゴンの当たり判定
		void Collision(SegmentCollider* segment) {}
		// 回転後の線分の座標を求める
		D3DXVECTOR3 CalcSegmentPosition(const D3DXVECTOR3 & localposition, D3DXMATRIX & matrix);
		//三角形ポリゴンとの当たり判定
		void Collision(TrianglePolygonListCollider* TrianglePolygonListCollider);
		//三角形ポリゴンとの当たり判定
		void Collision(PlaneCollider* planeCollider);


		//開始点のワールド行列を取得する
		const D3DXMATRIX& GetStartWorldMatrix() {
			return startWorldMatrix;
		}
		//終点のワールド行列を取得する
		const D3DXMATRIX& GetEndWorldMatrix() {
			return endWorldMatrix;
		}
	private:
		D3DXMATRIX startWorldMatrix;
		D3DXMATRIX endWorldMatrix;
	};
}
