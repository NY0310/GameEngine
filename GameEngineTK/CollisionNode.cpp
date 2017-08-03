#include "CollisionNode.h"
using namespace DirectX::SimpleMath;

bool CollisionNode::m_DebugVisible = false;

/* ---- CollisionNode�̃����o�֐� ---------- */
void CollisionNode::SetParant(Obj3d * parant)
{
	m_mbj.SetObjParent(parant);
}


/* ---- SphereNode�̃����o�֐� ---------- */
SphereNode::SphereNode()
{
	//�f�t�H���g���a1���[�g��
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

	{//���苅�̗v�f���v�Z

		const Matrix& worldm = m_mbj.GetLocalWorld();

		//���f�����W�n�ł̒��S���W
		Vector3 center(0, 0, 0);
		//���f�����W�n�ł̉E�[�̓_
		Vector3 right(1, 0, 0);
		//���[���h���W�n�ɕϊ�
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);
		//���苅�̗v�f����
		Sphere::Center = center;
		Sphere::Radius = Vector3::Distance(center, right);
	}
}

void SphereNode::Draw()
{
	m_mbj.Draw();
}
