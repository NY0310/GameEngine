#pragma once
#include <vector>
#include "../../Collision/Collision.h"
#include "../Object.h"

namespace NYLibrary
{
	class LocalObject : public virtual Object
	{
	public:
		LocalObject() {}
		//三角形ポリゴンを設定する
		void SetPolygons(std::vector<Triangle> polygons) { this->polygons = polygons; }
		//三角形ポリゴンを取得する
		std::vector<Triangle> GetPolygons() {return polygons;}
		//ローカルの大きさを設定する
		void SetLocalSize(float localSize) { this->localSize.x = localSize; 
		this->localSize.y = localSize;
		this->localSize.z = localSize;
		
		}
		//ローカルの大きさを設定する
		void SetLocalSizeX(float localSize) {
			this->localSize.x = localSize;
		}
		//ローカルの大きさを設定する
		void SetLocalSizeY(float localSize) {
			this->localSize.y = localSize;
		}
		//ローカルの大きさを設定する
		void SetLocalSizeZ(float localSize) {
			this->localSize.z = localSize;
		}
		//ローカルの大きさを設定する
		void SetLocalSize(const D3DXVECTOR3& localSize) {
			this->localSize = localSize;
		}
		//ローカルの大きさを取得する
		const D3DXVECTOR3& GetLocalSize() { return localSize; }
	protected:
		//三角形ポリゴン
		std::vector<Triangle> polygons;
		//ローカル上の大きさ
		D3DXVECTOR3 localSize;
	};
}