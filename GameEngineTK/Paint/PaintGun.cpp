#include "PaintGun.h"

using namespace std;
PaintGun::PaintGun()
{

}

/// <summary>
/// 初期化処理
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
/// 更新処理
/// </summary>
void PaintGun::Update()
{
	AimUpdate();
	Shoot();
	inkParticleManager->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void PaintGun::Render()
{
	inkParticleManager->Render();
	aim->Render();
}

/// <summary>
/// インクの発射
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
/// エイム更新処理
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
/// タンククラス
/// </summary>

void InkTank::Update()
{
	maxInk = redAmount + greenAmount + yellowAmount;
}

/// <summary>
/// タンクのインクをリセット
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
