#include "FollowCamera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Update()
{	
	//視点、参照点
	Vector3 eyepos, refpos;
	//ターゲット座標は、時期の座標に追従
	//参照点座標を計算
	refpos = m_targetPos + Vector3(0, 2.0f, 0);
	//ターゲット座標からカメラ座標への差分
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
	//時期の後ろに回り込むための回転行列
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	//カメラへのベクトルを回転
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//カメラ座標系さん
	eyepos = refpos + cameraV;
	//カメラの更新
	SetEyePos(eyepos);
	SetRefPos(refpos);

	//基底クラスの更新
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}
