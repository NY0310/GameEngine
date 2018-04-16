/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../../NYLibrary/ObjModel/Obj.h"
//#include "../../NYLibrary/Collision/Collision.h"
#include "Paint.h"
//#include "../../NYLibrary/Input/MouseRay.h"
#include "../PaintCollision/PaintCollision.h"


class PaintObj : public Obj
{
public:
	PaintObj(LPSTR fileName,bool isPlane = false);
	PaintObj() = delete;
	~PaintObj() = default;
	//子供を生成し、子供として登録
	void CreateAddChild();
	//初期化
	void Initialize();
	//描画
	void Render();
	//インク描画
	void InkRender() { campus->Render(); }
	//当たり判定呼び出し
	void OnCollisiton(Collider* collider);
	D3DXVECTOR2 CalcInkCollisionUv(const Triangle& triangle, const D3DXVECTOR3 & inter);
private:
	//インクを塗るキャンパス
	std::shared_ptr<Paint>campus;
	//平面化
	bool isPlane;
};




