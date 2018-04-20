#include "Target.h"


const float Target::DIFFERENCE_UPPER = 0.1f;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="fileName">obj�t�@�C����</param>
Target::Target(LPSTR fileName):PaintObj(fileName)
{
	//��_�F��ύX����
	ChangeWeakColor();
}

/// <summary>
/// �I�u�W�F�N�g�𐶐����q���Ƃ��ēo�^
/// </summary>
void Target::CreateAddChild()
{
	weakSprite = make_shared<WeakSprite>(this);
	AddChild(weakSprite);
}

/// <summary>
/// ������
/// </summary>
void Target::Initialize()
{
	PaintObj::CreateAddChild();
	PaintObj::Initialize();
	weakSprite->SetColor(weakColor);
	AddComponent<TrianglePolygonListCollider>();
}

/// <summary>
/// �X�V
/// </summary>
void Target::Update()
{
	Character::CheckData();
}

/// <summary>
/// �I�u�W�F�N�g�Ɠo�^������Ăяo�����	
/// </summary>
/// <param name="collider">���������I�u�W�F�N�g</param>
void Target::OnCollisiton(Collider* collider)
{
	PaintObj::OnCollisiton(collider);
	InkSegmentCollider* ink = dynamic_cast<InkSegmentCollider*>(collider);
	TrianglePolygonListCollider* triangleList = GetComponent<TrianglePolygonListCollider>();
	//���������̂��C���N��
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
/// ���񂾂Ƃ��Ăяo�����
/// </summary>
void Target::OnDied()
{
	weakSprite->SetVisible(false);
}



/// <summary>
/// ��_�F��
/// </summary>
/// <param name="hitColor">���������F</param>
/// <returns>��_�F��</returns>
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

void Target::ChangeWeakColor()
{
	weakColor = WeakColorList[static_cast<int>(rand() % total)];
}

