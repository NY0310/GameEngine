#pragma once
#include "Obj3d.h"

struct BulletDate {
	DirectX::SimpleMath::Vector3 Trans;
	DirectX::SimpleMath::Vector3 Velocity;

};

class Bullet
{

private:

	Obj3d m_ObjBullet;
	BulletDate m_data;

	// Žõ–½
	int m_LifeTime;
public:
	Bullet(BulletDate data);
	~Bullet();
	void Update();
	void Draw();
	int GetLifeTime() { return m_LifeTime; }
	Obj3d& GetObj3d() { return m_ObjBullet; }
};

