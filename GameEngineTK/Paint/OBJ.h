/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "OBJAbstract.h"
#include "../Collision.h"
//#include "Ink.h"
//#include "../Input/MouseRay.h"
//#include "PaintCollision.h"


class OBJ : public OBJAbstract
{
public:
	OBJ();
	~OBJ() = default;
	//更新処理
	void UpDate();
	//描画
	void Render();
	//インク描画
	void InkRender() {/* campus->Render();*/ }
private:
	//std::unique_ptr<Campus>campus;
//	std::unique_ptr<PaintCollision>paintCollision;
	//MouseRay* mouseRay;
};




