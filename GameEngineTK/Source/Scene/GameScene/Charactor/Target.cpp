#include "Target.h"


const float Target::DIFFERENCE_UPPER = 0.1f;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fileName">objファイル名</param>
Target::Target(LPSTR fileName):PaintObj(fileName)
{
}

/// <summary>
/// オブジェクトを生成し子供として登録
/// </summary>
void Target::CreateAddChild()
{
	weakSprite = make_shared<WeakSprite>(this);
	AddChild(weakSprite);
	//弱点色を変更する
	ChangeWeakColor();

}

/// <summary>
/// 初期化
/// </summary>
void Target::Initialize()
{
	PaintObj::CreateAddChild();
	PaintObj::Initialize();
	weakSprite->SetColor(weakColor);
	AddComponent<TrianglePolygonListCollider>();
}

/// <summary>
/// 更新
/// </summary>
void Target::Update()
{
	Character::CheckData();
}

/// <summary>
/// オブジェクトと登録したら呼び出される	
/// </summary>
/// <param name="collider">当たったオブジェクト</param>
void Target::OnCollisiton(Collider* collider)
{
	PaintObj::OnCollisiton(collider);
	InkSegmentCollider* ink = dynamic_cast<InkSegmentCollider*>(collider);
	TrianglePolygonListCollider* triangleList = GetComponent<TrianglePolygonListCollider>();
	//当たったのがインクか
	if (ink && triangleList)
	{
		//当たったインクが弱点色か
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
/// 死んだとき呼び出される
/// </summary>
void Target::OnDied()
{
	weakSprite->SetVisible(false);
	SetComponentActive(false);
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

/// <summary>
/// 弱点を変更する
/// </summary>
void Target::ChangeWeakColor()
{
	weakColor = WeakColorList[static_cast<int>(rand() % total)];
	weakSprite->SetColor(weakColor);
}

