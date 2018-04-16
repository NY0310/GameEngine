#include "PaintGun.h"

using namespace std;
PaintGun::PaintGun()
{

}

/// <summary>
/// オブジェクトを生成し子供として登録
/// </summary>
void PaintGun::CreateAddChild()
{
	aim = make_shared<Aim>();
	AddChild(aim);
	inkParticleManager = make_shared<InkParticleManager>();
	AddChild(inkParticleManager);
	inkTank = make_shared<InkTank>();
	AddChild(inkTank);
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
	MouseUtil* mouse = MouseUtil::GetInstance();

	if (mouse->IsPressed(MouseUtil::Left)&& !inkTank->IsEmptyInk())
	{
		//親の情報を取得(プレイヤー)
		MatrixObject* player = Getparent()._Get();

		D3DXVECTOR3 emitPosition = aimMatirx->GetPosition();
		//移動ベクトル
		D3DXVECTOR3 moveV(0,0,-1);

		//移動ベクトルを自機の角度分回転させる
		D3DXVec3TransformNormal(&moveV, &moveV,&aimMatirx->GetRotationMatrix());

		inkParticleManager->Shoot(emitPosition, moveV, inkTank->GetInkColor());
		inkTank->InkLost();
	}
}



void PaintGun::Finalize()
{}

