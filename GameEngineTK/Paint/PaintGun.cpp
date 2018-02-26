#include "PaintGun.h"

using namespace std;
PaintGun::PaintGun()
{

}

/// <summary>
/// ����������
/// </summary>
void PaintGun::Initialize()
{
	inkParticleManager = make_unique<InkParticleManager>();
	inkParticleManager->Initialize();
	aim = make_unique<Sprite>();
	aim->Initialize();
	aim->LoadTexture(L"Resources/PNG/Aim.png");
}

/// <summary>
/// �X�V����
/// </summary>
void PaintGun::Update()
{
	AimUpdate();
	Shoot();
	inkParticleManager->Update();
}

/// <summary>
/// �`�揈��
/// </summary>
void PaintGun::Render()
{
	inkParticleManager->Render();
	aim->Render();
}

/// <summary>
/// �C���N�̔���
/// </summary>
void PaintGun::Shoot()
{
	MouseRay* mouseRay = MouseRay::GetInstance();

	Segment* segment = mouseRay->RayCreate();
	static int a = 0;
	D3DXVECTOR4 color[7]{
		Colors::Blue,
		Colors::Red,
		Colors::Green,
		Colors::Yellow,
		Colors::Purple,
		Colors::Aqua,
		Colors::Orange
	};
	if (segment)
	{
		a++;
		D3DXVECTOR3 mouseVec = Math::VectorToD3DXVECTOR3(segment->Start - segment->End);
		D3DXVec3Normalize(&mouseVec, &mouseVec);
		const float longsize = 3;
		mouseVec *= longsize;
		inkParticleManager->Shoot(D3DXVECTOR3(Math::VectorToD3DXVECTOR3(segment->End)), D3DXVECTOR3(Math::VectorToD3DXVECTOR3(segment->Start - segment->End)), color[a]);
		delete segment;
	}

}

/// <summary>
/// �G�C���X�V����
/// </summary>
void PaintGun::AimUpdate()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	auto& devices = Devices::Get();
	aim->SetPosition(D3DXVECTOR3(mouse->GetX() - (int)devices.Width() / 2, (int)devices.Height() / 2 - mouse->GetY(), 0));
}


void PaintGun::Finalize()
{
	inkParticleManager.release();
	aim.release();
}


/// <summary>
/// �^���N�N���X
/// </summary>

void InkTank::Update()
{
	maxInk = redAmount + greenAmount + yellowAmount;
}

/// <summary>
/// �^���N�̃C���N�����Z�b�g
/// </summary>
void InkTank::Reset()
{
	redAmount = 0;
	greenAmount = 0;
	yellowAmount = 0;
}

void InkTank::Shoot()
{

}

void InkTank::CalcColor()
{
	color = D3DXVECTOR4(red / maxInk, green / maxInk, yellow / maxInk, 1);
}

void InkTank::ColorChange()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	int wheel =  mouse->GetWheelValue();
}
