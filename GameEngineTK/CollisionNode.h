///***************************************************************************
//*|
//*|	概要　　当たり判定ノード
//*|　作成者　GS2 16 中田湧介
//*|　作成日　2017//
//*|___________________________________________________________________________
//****************************************************************************/
//
//#pragma once
//
//#include "Obj3d.h"
//#include "Collision.h"
///***************************************************************************
//*|
//*|	概要　　当たり判定ノード
//*|　作成者　GS2 16 中田湧介
//*|　作成日　2017//
//*|___________________________________________________________________________
//****************************************************************************/
//class CollisionNode
//{
//public:
//	static bool GetDebugVisible() { return m_DebugVisible; }
//	static void SetDebugVisible(bool visible) { m_DebugVisible = visible; }
//protected:
//	//デバック表示ON
//	static bool m_DebugVisible;
//public:
//	//　初期化処理
//	virtual void Initialize() = 0;
//	//　更新
//	virtual void Update() = 0;
//	//　描画
//	virtual void Draw() = 0;
//	//
//	void SetParant(Obj3d* parant);
//
//	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans;}
//protected:
//	//デバック表示用オブジェクト
//	Obj3d m_mbj;
//	//親からのオフセット
//	DirectX::SimpleMath::Vector3 m_Trans;
//};
//
//
///***************************************************************************
//*|
//*|	概要　　球当たり判定ノード
//*|　作成者　GS2 16 中田湧介
//*|　作成日　2017//
//*|___________________________________________________________________________
//****************************************************************************/
//class SphereNode : public CollisionNode, public Sphere
//{
//public:
//	//コンストラクタ
//	SphereNode();
//	//　初期化処理
//	void Initialize();
//	//　更新
//	void Update();
//	//　描画
//	void Draw();
//	//ローカル半径設定
//	void SetLocalRadious(float radius) { m_LocalRadious = radius; }
//protected:
//	//ローカル半径
//	float m_LocalRadious;
//
//};