#include "PlayerState.h"

using namespace NYLibrary;

//const float PlayerState::ROTATION = 0.03f;

void PlayerState::MoveUpdate(Player * player, D3DXVECTOR3 speed)
{
	//キーボード
	KeyboardUtil* keyBoard = KeyboardUtil::GetInstance();
	//移動速度
	D3DXVECTOR3 saveSpeed(0,0,0);
	//前進
	if (keyBoard->IsPressed(DirectX::Keyboard::W))
		saveSpeed.z = -speed.z;
	//後退
	if (keyBoard->IsPressed(DirectX::Keyboard::S))
		saveSpeed.z = speed.z;
	//左旋回
	if (keyBoard->IsPressed(DirectX::Keyboard::A))
		saveSpeed.x = -speed.x;
	//右旋回
	if (keyBoard->IsPressed(DirectX::Keyboard::D))
		saveSpeed.x = speed.x;

	//移動させる
	if (saveSpeed.x != 0 || saveSpeed.z != 0)
	{
		Move(player, saveSpeed);
	}

	//回転量
	int width = static_cast<int>(Devices::Get().Width());
	int hight = static_cast<int>(Devices::Get().Height());
	MouseUtil* mouse = MouseUtil::GetInstance();
	DirectX::XMINT2 pos = mouse->GetPos();


	float a = 0;
	a = static_cast<float>(pos.x) / width;
	float b = static_cast<float>(pos.y) / hight;
	D3DXVECTOR2 c = Math::ChangeRegularDevice(D3DXVECTOR2(a,b));
	c.x *= -1;
	c*= 90;
	Rotation(player,c);

}

/// <summary>
/// プレイヤの移動
/// </summary>
/// <param name="player">プレイヤ</param>
/// <param name="speed">移動速度</param>
void PlayerState::Move(Player * player, D3DXVECTOR3 speed)
{
	//	移動ベクトル
	D3DXVECTOR3 moveV(speed);
	//	移動ベクトルを自機の角度分回転させる
	D3DXVec3TransformNormal(&moveV, &moveV,&player->GetRotationMatrix());	
	//	自機の座標を移動
	player->SetPosition(player->GetPosition() +  speed);

}

/// <summary>
/// プレイヤの回転
/// </summary>
/// <param name="player">プレイヤ</param>
/// <param name="angle">回転量</param>
void PlayerState::Rotation(Player * player, D3DXVECTOR2 angle)
{
	D3DXQUATERNION q(0, 0, 0, 1);
	D3DXQUATERNION q2(0, 0, 0, 1);
	D3DXVECTOR3 NAxis(0, 1, 0);
	D3DXVECTOR3 NAxis2(1, 0, 0);

	D3DXQuaternionRotationAxis(&q, &NAxis, D3DXToRadian(angle.x));
	D3DXQuaternionRotationAxis(&q2, &NAxis2, D3DXToRadian(angle.y));
	q *= q2;
	player->SetQuaternion(q);
}
