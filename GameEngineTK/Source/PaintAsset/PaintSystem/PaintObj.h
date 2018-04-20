/// <summary>
/// ペイントシステムを適応させたOBJクラス
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../../NYLibrary/ObjModel/Obj.h"
#include "Paint.h"
#include "../PaintCollision/PaintCollision.h"


class PaintObj : public Obj
{
public:
	//コンストラクタ
	PaintObj(LPSTR fileName,bool isPlane = false);
	//引数なしのコンストラクタ禁止
	PaintObj() = delete;
	//デストラクタ
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
	//キャンパスに付着したインクをクリアする
	void ClearCampus() { campus->ClearCampus(); }
	//インクが当たったUV値を算出
	D3DXVECTOR2 CalcInkCollisionUv(const Triangle& triangle, const D3DXVECTOR3 & inter);
private:
	//インクを塗るキャンパス
	std::shared_ptr<Paint> campus;
	//平面か
	bool isPlane;
};




