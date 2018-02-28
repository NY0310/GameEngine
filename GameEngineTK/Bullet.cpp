//#include "Bullet.h"
//
//
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//Bullet::Bullet(BulletDate data):
//	m_data(data),
//	m_LifeTime(0)
//{
//	m_ObjBullet.LoadModel(L"Resources\\head.cmo");
//	m_ObjBullet.SetTrans(m_data.Trans);
//}
//
//
//
//Bullet::~Bullet()
//{
//}
//
//void Bullet::Update()
//{
//	m_LifeTime++;
//	Vector3 pos = m_ObjBullet.GetTrans();
//	m_ObjBullet.SetTrans(pos + m_data.Velocity);
//	m_ObjBullet.Update();
//}
//
//void Bullet::Draw()
//{
//	m_ObjBullet.Draw();
//}
