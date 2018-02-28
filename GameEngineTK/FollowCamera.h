/// <summary>
/// 自機に追従するカメラクラス
/// </summary>

//	多重インクルードの防止
#pragma once

#include <Windows.h>
#include "Camera.h"
//#include "Charactor/Player/MainPlayer.h"

class FollowCamera : public Camera
{
public:
	static FollowCamera* GetInstance();

	//	自機とカメラの距離
	static const float CAMERA_DISTANCE;
	//	コンストラクタ
	FollowCamera();
	FollowCamera& operator= (const FollowCamera&) = delete;
	FollowCamera(const FollowCamera&) = delete;
	//	更新
	void Update() override;

	//	追従対象の座標をセット
	void SetTarGetTrans(const D3DXVECTOR3& tarGetTrans);
	//	追従対象の回転角をセット
	void SetTargetAngle(float targetAngle);


	//const DirectX::SimpleMath::Vector3& GetEyepos()const { return ; }
	//プレイヤをセット
	//void SetPlayer(Player* player) { m_player = player; }
private:
	//自クラスインスタンス
	static  std::unique_ptr<FollowCamera> camera;
protected:
	//	自機の座標
	D3DXVECTOR3 tarGetTrans;
	//	追従対象の回転角
	float targetAngle;
	//	FPSフラグ
	bool isFPS;
};