//	ヘッダファイルのインクルード
#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//	静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

//	コンストラクタ
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
{
	//	初期化処理
	m_tarGetTrans = Vector3::Zero;
	m_targetAngle = 0.0f;
	m_keyboard = nullptr;
	m_isFPS = false;
	m_player = nullptr;
}

//	更新
void FollowCamera::Update()
{
	//	キーボードの状態絵御取得
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);

	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		m_isFPS = !m_isFPS;
	}

	//追従対象の座標等をセット
	SetTarGetTrans(m_player->GetTrans());
	SetTargetAngle(m_player->GetRot().y);

	//	カメラ視点座標、参照点座標
	Vector3 eyepos, refpos;
	if(m_isFPS == true)
	{//	FPSカメラの処理
		Vector3 position;
		//	参照点座標を計算
		position = m_tarGetTrans + Vector3(0, 0.2f, 0);
		//	自機からカメラ座標への差分
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		//	自機の後ろに回り込むための回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//	カメラへのベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//	ちょっと進んだ位置が視点座標
		eyepos = position + cameraV * 0.1f;
		//	がっつり進んだ位置が参照点座標
		refpos = position + cameraV;
	}
	else
	{//	TPSカメラの処理
	 //	参照点座標を計算
		refpos = m_tarGetTrans + Vector3(0, 2, 0);
		//	自機からカメラ座標への差分
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
		//	自機の後ろに回り込むための回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//	カメラへのベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//	カメラ座標を計算
		eyepos = refpos + cameraV;
	}

	//	視点を現在位置から補間する
	eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
	//	参照点を現在位置から補間する
	refpos = m_refpos + (refpos - m_refpos) * 0.2f;

	this->SetEyePos(eyepos);
	this->SetRefPos(refpos);

	//	基底クラスの更新
	Camera::Update();
}

void FollowCamera::SetTarGetTrans(const DirectX::SimpleMath::Vector3 & targetPos)
{
	m_tarGetTrans = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngle)
{
	m_targetAngle = targetAngle;
}

//	キーボードをセット
void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}

