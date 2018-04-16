#include "PlayerState.h"
#include <Winuser.h>
#include <process.h>

using namespace NYLibrary;

const float PlayerState::ROTATION_COEFFICIENT = 35.0f;
const D3DXVECTOR2 PlayerState::MAX_ROTATION = D3DXVECTOR2(90, 45);
PlayerState::PlayerState()
{
}

void PlayerState::MoveUpdate(Player * player, D3DXVECTOR3 speed)
{
	initialMousePosition.x = static_cast<float>(Devices::Get().Width());
	initialMousePosition.y = static_cast<float>(Devices::Get().Height());

	//デバイス
	auto& devices = Devices::Get();
	//キーボード
	KeyboardUtil* keyBoard = KeyboardUtil::GetInstance();
	//移動速度
	D3DXVECTOR3 saveSpeed(0,0,0);
	//回転量
	D3DXVECTOR2 savaRot(0, 0);
	//前進
	if (keyBoard->IsPressed(DirectX::Keyboard::W))
		saveSpeed.z = -speed.z;
	//後退
	if (keyBoard->IsPressed(DirectX::Keyboard::S))
		saveSpeed.z = speed.z;
	//左旋回
	if (keyBoard->IsPressed(DirectX::Keyboard::A))
		saveSpeed.x += speed.x;
	//右旋回
	if (keyBoard->IsPressed(DirectX::Keyboard::D))
		saveSpeed.x -= speed.x;
	if (keyBoard->IsPressed(DirectX::Keyboard::E))
		exit(0);
	//移動させる
	if (saveSpeed.x != 0 || saveSpeed.z != 0)
	{
		Move(player, saveSpeed);
	}


	//マウスカーソルの座標を取得する
	LPPOINT  mousePos = new POINT();
	GetCursorPos(mousePos);

	//現フレームでのマウスの移動量
	D3DXVECTOR2 mouseTrans;
	mouseTrans.x = static_cast<float>(mousePos->x) - initialMousePosition.x;
	mouseTrans.y = static_cast<float>(mousePos->y) - initialMousePosition.y;
	//そのままの数値でクォータニオンを作成すると回転しすぎるので係数で除算
	mouseTotalTrans = mouseTrans / ROTATION_COEFFICIENT;
	//////回転に上限を設ける
	Math::ClampAbsolute(mouseTotalTrans.y, MAX_ROTATION.y);
	//クォータニオンを作成しプレイヤのを回転させる
	D3DXMATRIX rotY;
	D3DXMatrixRotationQuaternion(&rotY,& Rotation(D3DXVECTOR2(mouseTotalTrans.x, 0)));
	player->SetRotationMatrix(player->GetRotationMatrix() *rotY);
	D3DXMatrixRotationQuaternion(&rotY, &Rotation(D3DXVECTOR2(mouseTotalTrans.x, -mouseTotalTrans.y)));
	player->aimMatrix->SetRotationMatrix(player->aimMatrix->GetRotationMatrix() *rotY);

	//D3DXMatrixRotationX(&rotY, D3DXToRadian(mouseTotalTrans.x));
	//player->SetRotationMatrix(player->GetRotationMatrix() *rotY);
	//D3DXMatrixRotationYawPitchRoll(&rotY, D3DXToRadian(mouseTotalTrans.x), D3DXToRadian (-mouseTotalTrans.y),0);
	//player->aimMatrix->SetRotationMatrix(player->aimMatrix->GetRotationMatrix() *rotY);
	//エイムの座標を移動
	player->aimMatrix->SetPosition(player->GetPosition());


	delete mousePos;
	mousePos = nullptr;
}

/// <summary>
/// プレイヤの移動
/// </summary>
/// <param name="player">プレイヤ</param>
/// <param name="speed">移動速度</param>
void PlayerState::Move(Player * player, D3DXVECTOR3 speed)
{
	//	移動ベクトルw
	D3DXVECTOR3 moveV(speed);

	//	移動ベクトルを自機の角度分回転させる
	D3DXMATRIX rotY;
	D3DXMatrixRotationQuaternion(&rotY,&Rotation(D3DXVECTOR2(mouseTotalTrans.x,0)));
	D3DXVec3TransformNormal(&moveV, &moveV,&player->GetRotationMatrix());


	//	自機の座標を移動
	player->SetPosition(player->GetPosition() + moveV);
}

/// <summary>
/// プレイヤの回転
/// </summary>
/// <param name="player">プレイヤ</param>
/// <param name="angle">回転量</param>
D3DXQUATERNION PlayerState::Rotation(D3DXVECTOR2 angle)
{
	D3DXQUATERNION q(0, 0, 0, 1);
	D3DXQUATERNION q2(0, 0, 0, 1);
	D3DXVECTOR3 NAxis(0, 1, 0);
	D3DXVECTOR3 NAxis2(1, 0, 0);

	D3DXQuaternionRotationAxis(&q, &NAxis, D3DXToRadian(angle.x));
	D3DXQuaternionRotationAxis(&q2, &NAxis2, D3DXToRadian(angle.y));
	return q2 * q ;
}

