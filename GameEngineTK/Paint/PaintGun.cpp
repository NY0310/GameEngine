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
	aim = make_unique<Sprite>(Sprite::Dimension3);
	aim->Initialize();
	aim->LoadTexture(L"Resources/PNG/Aim.png");
}

/// <summary>
/// 更新処理
/// </summary>
void PaintGun::Update()
{
	AimUpdate();
	Emit();
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
void PaintGun::Emit()
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
		const float longsize = 10;
		mouseVec *= longsize;
		inkParticleManager->Shoot(emitPosition, D3DXVECTOR3(mouseVec - emitPosition), color[a]);

		delete segment;
	}

}

/// <summary>
/// エイム更新処理
/// </summary>
void PaintGun::AimUpdate()
{
	MouseRay* mouseRay = MouseRay::GetInstance();
	MouseUtil* mouse = MouseUtil::GetInstance();
	Segment* segment = mouseRay->RayCreate();
	D3DXVECTOR3 mouseVec = Math::VectorToD3DXVECTOR3(segment->Start - segment->End);
	D3DXVec3Normalize(&mouseVec, &mouseVec);
	const float longsize = 10;
	mouseVec *= longsize;

	//auto& devices = Devices::Get();
	//D3DXVECTOR2 position2D;
	//auto camera = FollowCamera::GetInstance();
	//camera->Project(mouseVec, &position2D);

	aim->Set3DPosition(D3DXVECTOR3(mouseVec.x, mouseVec.y , mouseVec.z));
	//camera->SetRefPos(D3DXVECTOR3(position2D.x - (int)devices.Width() / 2, (int)devices.Height() / 2 - position2D.y, 0));
	//D3DXVECTOR3 mouseVec = Math::VectorToD3DXVECTOR3(segment->Start);
	//D3DXVec3Normalize(&mouseVec, &mouseVec);
	//D3DXVECTOR2 position2D;
	//auto camera = FollowCamera::GetInstance();
	//camera->Project(mouseVec, &position2D);

	//aim->Set3DPosition(D3DXVECTOR3(mouseVec));

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
	colorAmount[total] = colorAmount[red] + colorAmount[green] + colorAmount[blue];
}

/// <summary>
/// タンクのインクをリセット
/// </summary>
void InkTank::Initialize()
{
	for (int i = 0; i < total; i++)
	{
		colorAmount[i] = 0;
	}
}

void InkTank::Emit()
{

}

void InkTank::CalcColor()
{
	color = D3DXVECTOR4(colorAmount[red] / colorAmount[total], colorAmount[green] / colorAmount[total], colorAmount[blue] / colorAmount[total], 1);
}

void InkTank::ChangeColor()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	int wheel =  mouse->GetWheelValue();
}
