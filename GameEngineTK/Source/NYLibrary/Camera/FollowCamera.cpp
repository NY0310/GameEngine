//	ヘッダファイルのインクルード
#include "FollowCamera.h"
#include "./../../Scene/GameScene/Charactor/Player/MainPlayer.h"

using namespace std;
using namespace NYLibrary;


//	静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 1;
unique_ptr<FollowCamera> FollowCamera::camera ;

FollowCamera* FollowCamera::GetInstance()
{
	if (!camera)
		camera.reset(new FollowCamera());
	return camera.get();
}

//	コンストラクタ
FollowCamera::FollowCamera()
{
	//	初期化処理
	isFPS = false;
	player = nullptr;

}

//	更新
void FollowCamera::Update()
{

	if (!player)
		return;
	//	カメラ視点座標、参照点座標
	D3DXVECTOR3 eyepos, refpos;
	if(isFPS == true)
	{//	FPSカメラの処理
		//	参照点座標を計算
		 D3DXVECTOR3 position = player->GetPosition() + D3DXVECTOR3(0, 0.2f, 0);
		//	自機からカメラ座標への差分
		D3DXVECTOR3 cameraV(0, 0, -CAMERA_DISTANCE);
		D3DXMATRIX rotmat;
		//	自機の後ろに回り込むための回転行列
//		 D3DXMatrixRotationY(&rotmat,targetAngle);
		//	カメラへのベクトルを回転
		D3DXVec3TransformNormal(&cameraV,&cameraV, &rotmat);
		//	ちょっと進んだ位置が視点座標
		eyepos = position + cameraV * 0.1f;
		//	がっつり進んだ位置が参照点座標
		refpos = position + cameraV;
	}
	else
	{//	TPSカメラの処理
	 //	参照点座標を計算
		refpos = player->GetPosition() + D3DXVECTOR3(0, 0.3, 0);
		//	自機からカメラ座標への差分
		D3DXVECTOR3 cameraV(0, 0.2,CAMERA_DISTANCE);
		//	自機の後ろに回り込むための回転行列
		D3DXMATRIX rotmat;
		//D3DXMatrixRotationY(&rotmat,targetAngle);
		//	カメラへのベクトルを回転
		D3DXVec3TransformNormal(&cameraV,&cameraV,&player->aimMatrix->GetRotationMatrix());
		//	カメラ座標を計算
		eyepos = refpos + cameraV;
	}


	//ローカル回転
	//Yaw
	if (eyepos.y != eyepos.y) 
	{
		//	視点を現在位置から補間する
		eyepos.y = eyepos.y + (eyepos.y - eyepos.y) * 0.05f;
		//	参照点を現在位置から補間する
		refpos.y = refpos.y + (refpos.y - refpos.y) * 0.2f;
	}
	//Pich
	else if (eyepos.x != eyepos.x)
	{
		eyepos.x = eyepos.x + (eyepos.x - eyepos.x) * 0.05f;
		refpos.x = refpos.x + (refpos.x - refpos.x) * 0.2f;
	}
	//Roll
	else if (eyepos.z != eyepos.z)
	{
		eyepos.z = eyepos.z + (eyepos.z - eyepos.z) * 0.05f;
		refpos.z = refpos.z + (refpos.z - refpos.z) * 0.2f;
	}

	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);


	//	基底クラスの更新
	Camera::Update();
}
