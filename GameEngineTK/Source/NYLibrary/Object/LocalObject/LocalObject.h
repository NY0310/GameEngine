#pragma once
#include <vector>
#include "../../Collision/Collision.h"
#include "../Object.h"

namespace NYLibrary
{
	class LocalObject : public virtual Object
	{
	public:
		LocalObject() { localSize = 0; }
		//三角形ポリゴンを設定する
		void SetPolygons(const std::vector<Triangle>& polygons) { this->polygons = polygons; }
		//三角形ポリゴンを取得する
		const std::vector<Triangle>&GetPolygons() {return polygons;}
		//ローカルの大きさを設定する
		void SetLocalSize(float localSize) { this->localSize = localSize; }
		//ローカルの大きさを取得する
		float GetLocalSize() { return localSize; }
	protected:
		//三角形ポリゴン
		std::vector<Triangle> polygons;
		//ローカル上の大きさ
		float localSize;
	};
}