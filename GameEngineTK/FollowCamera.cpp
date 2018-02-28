//	ヘッダファイルのインクルード
#include "FollowCamera.h"

using namespace std;
//	静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;
unique_ptr<FollowCamera> FollowCamera::camera ;

FollowCamera* FollowCamera::GetInstance()
{
	if (!camera)
		camera =  make_unique<FollowCamera>();
	return camera.get();
}

//	コンストラクタ
FollowCamera::FollowCamera()
{
	//	初期化処理
	tarGetTrans = D3DXVECTOR3(0,0,0);
	targetAngle = 0.0f;
	isFPS = false;
	//m_player = nullptr;
}

//	更新
void FollowCamera::Update()
{
	//	キーボードの状態絵御取得
	//Keyboard::State keystate = m_keyboard->GetState();
	//m_keyboardTracker.Update(keystate);

	//if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	//{
	//	m_isFPS = !m_isFPS;
	//}

	//追従対象の座標等をセット
	/*SetTarGetTrans(m_player->GetTrans());
	SetTargetAngle(m_player->GetRot().y);*/

	//	カメラ視点座標、参照点座標
	D3DXVECTOR3 eyepos, refpos;
	if(isFPS == true)
	{//	FPSカメラの処理
		//	参照点座標を計算
		 D3DXVECTOR3 position = tarGetTrans + D3DXVECTOR3(0, 0.2f, 0);
		//	自機からカメラ座標への差分
		D3DXVECTOR3 cameraV(0, 0, -CAMERA_DISTANCE);
		D3DXMATRIX rotmat;
		//	自機の後ろに回り込むための回転行列
		 D3DXMatrixRotationY(&rotmat,targetAngle);
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
		refpos = tarGetTrans + D3DXVECTOR3(0, 2, 0);
		//	自機からカメラ座標への差分
		D3DXVECTOR3 cameraV(0, 0, CAMERA_DISTANCE);
		//	自機の後ろに回り込むための回転行列
		D3DXMATRIX rotmat;
		D3DXMatrixRotationY(&rotmat,targetAngle);
		//	カメラへのベクトルを回転
		D3DXVec3TransformNormal(&cameraV,&cameraV,&rotmat);
		//	カメラ座標を計算
		eyepos = refpos + cameraV;
	}

	//	視点を現在位置から補間する
	eyepos = eyepos + (eyepos - eyepos) * 0.05f;
	//	参照点を現在位置から補間する
	refpos = refpos + (refpos - refpos) * 0.2f;

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

	////拡大倍率の補完
	//m_zoom += (zoom - m_zoom) * 0.05f;

	//this->SetEyePos(Vector3(0.00749884499f, 1.54812920f, 4.99988747f)); //視点位置
	//this->SetRefPos(Vector3(-3.61911745e-08f, 1.98155320f, -3.12580148e-08f));//注視位置


	//this->SetEyePos(Vector3(0.0f, 0.0f, -2.0f)); //視点位置
	//this->SetRefPos(Vector3(-0.0f, 0.5f, 0.0f));//注視位置
	//this->SetUpVec(Vector3(0.0f, 1.0f, 0.0f));//上方位置
	//m_fovY = XM_PI / 4;
	//m_aspect = (FLOAT)640 / (FLOAT)480;
	//m_NearClip = 0.1f;
	//m_FarClip = 100.0f;


	//	基底クラスの更新
	Camera::Update();
}
void FollowCamera::SetTarGetTrans(const D3DXVECTOR3& targetPos)
{
	this->tarGetTrans = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngle)
{
	this->targetAngle = targetAngle;
}

////	キーボードをセット
//void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
//{
//	m_keyboard = keyboard;
//}

