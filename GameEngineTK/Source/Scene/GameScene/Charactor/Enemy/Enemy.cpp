#include "Enemy.h"


const float Enemy::DIFFERENCE_UPPER = 0.1f;



Enemy::Enemy()
{
	weakColor = WeakColorList[static_cast<int>(rand()% total)];
}


void Enemy::CreateAddChild()
{
	weakSprite = make_shared<WeakSprite>();
	AddChild(weakSprite);
	hp = 10;
}

void Enemy::Initialize()
{
	PaintObj::CreateAddChild();
	PaintObj::Initialize();
	LoadOBJFile("Resources/OBJMODEL/Enemy.obj");
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	SetTag("Enemy");
	SetScale(0.2f);
	SetPositionY(GetLocalSize() *GetScaleY() / 2);
	weakSprite->SetColor(weakColor);
	AddComponent<TrianglePolygonListCollider>();
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
		difference = Math::NaturalNumber(difference);
		float totlaDifference = difference.x + difference.y + difference.z;
		if (DIFFERENCE_UPPER >= totlaDifference)
		{
			WeakHitDamage();

		}
		else
		{
			HitDamage();
		}
	}

}

/// <summary>
/// ���񂾂Ƃ�
/// </summary>
void Enemy::OnDied()
{
	exit(0);

}




