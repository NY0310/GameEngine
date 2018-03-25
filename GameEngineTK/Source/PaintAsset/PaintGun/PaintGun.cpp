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
	AimUpdate();
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
		a++;
		if (a == 7)
		{
			a = 0;
		}

		//親の情報を取得(プレイヤー)
		MatrixObject* player = Getparent()._Get();
		const D3DXVECTOR3 shitGunPosition(0, 0.5f, -0.2f);
		emitPosition = player->GetPosition() + shitGunPosition ;
		////	移動ベクトルw
		D3DXVECTOR3 moveV(0,0,-1);

		//移動ベクトルを自機の角度分回転させる
		D3DXVec3TransformNormal(&moveV, &moveV,&player->GetRotationMatrix());	

		////D3DXQUATERNION playerQuaternion = player._Get()->GetQuaternion();
		/// 
		MouseUtil* mouse = MouseUtil::GetInstance();
		if (mouse->IsPressed(MouseUtil::Left))
		inkParticleManager->Shoot(emitPosition, moveV, color[a]);

		//D3DXVECTOR3 mouseVec = Math::VectorToD3DXVECTOR3(segment->Start - segment->End);
		//D3DXVec3Normalize(&mouseVec, &mouseVec);
		//const float longsize = 10;
		//mouseVec *= longsize;
		//MouseUtil* mouse = MouseUtil::GetInstance();
		//if (mouse->IsPressed(MouseUtil::Left))
}

/// <summary>
/// エイム更新処理
/// </summary>
void PaintGun::AimUpdate()
{
	//MouseRay* mouseRay = MouseRay::GetInstance();
	//MouseUtil* mouse = MouseUtil::GetInstance();
	//Segment* segment = mouseRay->RayCreate();
	//D3DXVECTOR3 mouseVec = segment->start - emitPosition;
	//D3DXVec3Normalize(&mouseVec, &mouseVec);
	//const float longsize = 10;
	//D3DXVECTOR3 balletVec = mouseVec * longsize - emitPosition;
	//D3DXVec3Normalize(&balletVec, &balletVec);
	//balletVec *= 3;
	//aim->matrixObject->ChangeOrder(WorldMatrixOrder::SCALEM_TRANSMAT_ROTOMAT);
	//aim->matrixObject->transfer(balletVec);
	//aim->matrixObject-> matrixObject->eulerAngles(nDirection);(D3DXVECTOR3(mouseVec.x, mouseVec.y, -emitPosition.z));

	//auto& devices = Devices::Get();
	//D3DXVECTOR2 position2D;
	//aim->Set2DPosition(D3DXVECTOR2(mouse->GetX() - (int)devices.Width() / 2, (int)devices.Height() / 2 - mouse->GetY()));
	//auto camera = FollowCamera::GetInstance();
	//camera->Project(mouseVec, &position2D);
	 
	//camera->SetTargetAngle(mouseVec.x);
	//D3DXVECTOR3 mouseVec = Math::VectorToD3DXVECTOR3(segment->Start);
	//D3DXVec3Normalize(&mouseVec, &mouseVec);
	//D3DXVECTOR2 position2D;
	//auto camera = FollowCamera::GetInstance();
	//camera->Project(mouseVec, &position2D);

	//aim->Set3DPosition(D3DXVECTOR3(mouseVec));


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
