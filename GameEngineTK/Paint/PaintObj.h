/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "Obj.h"
#include "../Collision.h"
#include "Paint.h"
#include "../Input/MouseRay.h"
#include "PaintCollision.h"


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
private:
	std::unique_ptr<Paint>campus;
	std::unique_ptr<PaintCollision>paintCollision;
};




