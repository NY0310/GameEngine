#include "Target.h"


const float Target::DIFFERENCE_UPPER = 0.1f;



Target::Target(LPSTR fileName):PaintObj(fileName)
{
	weakColor = WeakColorList[static_cast<int>(rand()% total)];
}


void Target::CreateAddChild()
{
	weakSprite = make_shared<WeakSprite>();
	AddChild(weakSprite);
	hp = 10;
}

void Target::Initialize()
{
	PaintObj::CreateAddChild();
	PaintObj::Initialize();
	weakSprite->SetColor(weakColor);
	AddComponent<TrianglePolygonListCollider>();
}

void Target::Update()
{
	Character::CheckData();
}



void Target::OnCollisiton(Collider* collider)
{
	PaintObj::OnCollisiton(collider);
	InkSegmentCollider* ink = dynamic_cast<InkSegmentCollider*>(collider);
	TrianglePolygonListCollider* triangleList = GetComponent<TrianglePolygonListCollider>();
	//当たったのがインクか
	if (ink && triangleList)
	{
		if(IsWeakColor(ink->GetColor()))
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
/// 死んだとき
/// </summary>
void Target::OnDied()
{
	exit(0);

}



/// <summary>
/// 弱点色か
/// </summary>
/// <param name="hitColor">当たった色</param>
/// <returns>弱点色か</returns>
bool Target::IsWeakColor(const D3DXVECTOR4& hitColor)
{
	D3DXVECTOR4 color = weakColor;
	D3DXVECTOR4 color2 = hitColor;
	color.w = 0;
	color2.w = 0;
	D3DXVECTOR4 difference = color - color2;
	difference = Math::NaturalNumber(difference);
	float totlaDifference = difference.x + difference.y + difference.z;
	return DIFFERENCE_UPPER >= totlaDifference;
}