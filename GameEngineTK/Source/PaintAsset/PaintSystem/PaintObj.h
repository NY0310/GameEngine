/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../../NYLibrary/Obj/Obj.h"
//#include "../../NYLibrary/Collision/Collision.h"
#include "Paint.h"
//#include "../../NYLibrary/Input/MouseRay.h"
#include "../PaintCollision/PaintCollision.h"


class PaintObj : public Obj
{
public:
	PaintObj(bool isPlane = false);
	~PaintObj() = default;
	//更新処理
	void Update();
	//描画
	void Render();
	//インク描画
	void InkRender() { campus->Render(); }
	//当たり判定呼び出し
	void OnCollisiton(Collider* collider);
	const D3DXVECTOR2& CalcInkCollisionUv(const Segment& segment, const Triangle& triangle, const D3DXVECTOR3 & inter);
private:
	//インクが当たったUV値を算出する
	std::unique_ptr<Paint>campus;
	std::unique_ptr<PaintCollision>paintCollision;
};




