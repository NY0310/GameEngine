#include "PaintGun.h"

using namespace std;
PaintGun::PaintGun()
{

}

void PaintGun::CreateAddChild()
{
	aim = make_shared<Aim>();
	AddChild(aim);
	inkParticleManager = make_shared<InkParticleManager>();
	AddChild(inkParticleManager);
}

/// <summary>
/// 初期化処理
/// </summary>
void PaintGun::Initialize()
{
	auto& devices = Devices::Get();
}

/// <summary>
/// 更新処理
/// </summary>
void PaintGun::Update()
{
	Emit();
}

/// <summary>
/// 描画処理
/// </summary>
void PaintGun::Render()
{
}

/// <summary>
/// インクの発射
/// </summary>
void PaintGun::Emit()
{
	//MouseRay* mouseRay = MouseRay::GetInstance();
	D3DXVECTOR4 color = Colors::Orange;


	//親の情報を取得(プレイヤー)
	MatrixObject* player = Getparent()._Get();
	const D3DXVECTOR3 shitGunPosition(0, 0.5f, -0.2f);
	emitPosition = player->GetPosition() + shitGunPosition ;
	//移動ベクトル
	D3DXVECTOR3 moveV(0,0,-1);

	//移動ベクトルを自機の角度分回転させる
	D3DXVec3TransformNormal(&moveV, &moveV,&player->GetRotationMatrix());	

	/// 
	MouseUtil* mouse = MouseUtil::GetInstance();
	if (mouse->IsPressed(MouseUtil::Left))
	inkParticleManager->Shoot(emitPosition, moveV, color);
}



void PaintGun::Finalize()
{
	inkParticleManager.reset();
	aim.reset();
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
	color = D3DXVECTOR4(static_cast<float>(colorAmount[red] / colorAmount[total]), static_cast<float>(colorAmount[green] / colorAmount[total]), static_cast<float>(colorAmount[blue] / colorAmount[total]), 1);
}

void InkTank::ChangeColor()
{
	MouseUtil* mouse = MouseUtil::GetInstance();
	int wheel =  mouse->GetWheelValue();
}
