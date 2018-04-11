#include "Enemy.h"


const float Enemy::DIFFERENCE_UPPER = 0.7f;



Enemy::Enemy()
{
	weakColor = WeakColorList[static_cast<int>(rand()% total)];
}


void Enemy::CreateAddChild()
{
	weakSprite = make_shared<WeakSprite>();
	AddChild(weakSprite);
}

void Enemy::Initialize()
{
	PaintObj::CreateAddChild();
	PaintObj::Initialize();
	LoadOBJFile("Resources/OBJ/Enemy.obj");
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	SetTag("Enemy");
	SetPositionY(GetLocalSize() / 2);
	AddComponent<TrianglePolygonListCollider>();
	weakSprite->SetColor(weakColor);
	auto  a = GetLocalSize();
}

void Enemy::Update()
{
	Character::CheckData();
}



void Enemy::OnCollisiton(Collider* collider)
{
	PaintObj::OnCollisiton(collider);
	InkSegmentCollider* ink = dynamic_cast<InkSegmentCollider*>(collider);
	TrianglePolygonListCollider* triangleList = GetComponent<TrianglePolygonListCollider>();
	if (ink && triangleList)
	{
		D3DXVECTOR4 difference = weakColor - ink->GetColor();
		int totlaDifference = difference.x + difference.y + difference.z;
		if (DIFFERENCE_UPPER >= totlaDifference)
		{
			WeakHitDamage();
			exit(0);

		}
		else
		{
			HitDamage();
		}
	}

}

/// <summary>
/// Ž€‚ñ‚¾‚Æ‚«
/// </summary>
void Enemy::OnDied()
{

}




