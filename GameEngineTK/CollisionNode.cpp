#include "CollisionNode.h"
using namespace DirectX::SimpleMath;

bool CollisionNode::m_DebugVisible = false;

/* ---- CollisionNodeのメンバ関数 ---------- */
void CollisionNode::SetParant(Obj3d * parant)
{
	m_mbj.SetObjParent(parant);
}


/* ---- SphereNodeのメンバ関数 ---------- */
SphereNode::SphereNode()
{
	//デフォルト半径1メートル
	m_LocalRadious = 1.0f;
}


void SphereNode::Initialize()
{
	m_mbj.LoadModel(L"SphereNode.cmo");
}

void SphereNode::Update()
{
	m_mbj.SetTrans(m_Trans);
	m_mbj.SetScale(Vector3(m_LocalRadious));
	m_mbj.Update();

	{//判定球の要素を計算

		const Matrix& worldm = m_mbj.GetLocalWorld();

		//モデル座標系での中心座標
		Vector3 center(0, 0, 0);
		//モデル座標系での右端の点
		Vector3 right(1, 0, 0);
		//ワールド座標系に変換
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);
		//判定球の要素を代入
		Sphere::Center = center;
		Sphere::Radius = Vector3::Distance(center, right);
	}
}

void SphereNode::Draw()
{
	m_mbj.Draw();
}
