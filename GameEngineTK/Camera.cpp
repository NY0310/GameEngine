//	ヘッダファイルのインクルード
#include "Camera.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

//	コンストラクタ
Camera::Camera()
{
	//	メンバ変数の初期化
	m_eyepos = Vector3(0, 0, 5);
	m_refpos = Vector3(0, 0, 0);
	m_upvec = Vector3(0, 1, 0);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = Devices::Get().Width() / Devices::Get().Height();
	m_NearClip = 0.1f;
	m_FarClip = 1000.0f;
	m_zoom = 1.0f;
	m_proj = Matrix::Identity;

	//	ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//	射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY / m_zoom, m_aspect, m_NearClip, m_FarClip);
}

//	デストラクタ
Camera::~Camera()
{

}

//	更新
void Camera::Update()
{
	//	ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//	射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY / m_zoom, m_aspect, m_NearClip, m_FarClip);
}


/// <summary>
/// ３Ｄ→２Ｄ座標変換
/// ワールド座標からスクリーン座標を計算する
/// </summary>
/// <param name="pos2d"></param>
///// <returns>成否</returns>
bool Camera::Project(const Vector3& worldPos, Vector2* screenPos)
{
	auto& devices = Devices::Get();

	Vector4 clipPos;
	Vector4 worldPosV4(worldPos.x, worldPos.y, worldPos.z, 1.0f);

	// ビュー変換
	clipPos = Vector4::Transform(worldPosV4, m_view);

	// 射影変換
	clipPos = Vector4::Transform(clipPos, m_proj);

	// ビューポートの取得
	D3D11_VIEWPORT viewport = devices.Viewport();

	// ニアクリップより前方にいなければ計算できない
	if (clipPos.w <= 1.0e-5) return false;

	// ビューポート変換
	float ndcX = clipPos.x / clipPos.w;
	float ndcY = -clipPos.y / clipPos.w;

	screenPos->x = (ndcX + 1.0f) * 0.5f * viewport.Width + viewport.TopLeftX;
	screenPos->y = (ndcY + 1.0f) * 0.5f * viewport.Height + viewport.TopLeftY;

	return true;
}

/// <summary>
/// ３Ｄ→２Ｄ座標変換
/// スクリーン座標を、ニアクリップ、ファークリップ間の線分に変換する
/// </summary>
/// <param name="screenPos"></param>
/// <param name="worldSegment"></param>
void Camera::UnProject(const Vector2& screenPos, Segment* worldSegment)
{
	auto& devices = Devices::Get();


	Vector2 clipPos;
	Vector4 clipPosNear;
	Vector4 clipPosFar;

	// ビューポートの取得
	//D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();

	// スクリーン座標→射影座標
	/*clipPos.x = (screenPos.x - viewport.TopLeftX) / (viewport.Width / 2.0f) - 1.0f;
	clipPos.y = (screenPos.y - viewport.TopLeftY) / (viewport.Height / 2.0f) - 1.0f;*/
	clipPos.y = -clipPos.y;

	clipPosNear.x = m_NearClip * clipPos.x;
	clipPosNear.y = m_NearClip * clipPos.y;
	clipPosNear.z = 0;
	clipPosNear.w = m_NearClip;

	clipPosFar.x = m_FarClip * clipPos.x;
	clipPosFar.y = m_FarClip * clipPos.y;
	clipPosFar.z = m_FarClip;
	clipPosFar.w = m_FarClip;

	// プロジェクション、ビュー逆変換
	Matrix invMat = m_view * m_proj;
	invMat.Invert(invMat);

	Matrix invView;
	m_view.Invert(invView);

	Matrix invProj;
	m_proj.Invert(invProj);

	// 射影座標→ビュー座標
	Vector4 viewStart = Vector4::Transform(clipPosNear, invProj);
	Vector4 viewEnd = Vector4::Transform(clipPosFar, invProj);
	// ビュー座標→ワールド座標
	Vector4 start = Vector4::Transform(viewStart, invView);
	Vector4 end = Vector4::Transform(viewEnd, invView);

	worldSegment->Start.x = start.x;
	worldSegment->Start.y = start.y;
	worldSegment->Start.z = start.z;

	worldSegment->End.x = end.x;
	worldSegment->End.y = end.y;
	worldSegment->End.z = end.z;
}



//	ビュー行列を取得する関数
const Matrix& Camera::GetView()
{
	return m_view;
}

//	射影行列を取得する関数
const Matrix& Camera::GetProjection()
{
	return m_proj;
}

//	視点座標のセットする関数
void Camera::SetEyePos(const Vector3& eyepos)
{
	m_eyepos = eyepos;
}

//	参照点のセット
void Camera::SetRefPos(const Vector3& refpos)
{
	m_refpos = refpos;
	//m_refpos = Vector3(0, 0, 0);
}

//	上方向ベクトルのセット
void Camera::SetUpVec(const DirectX::SimpleMath::Vector3 & upvec)
{
	m_upvec = upvec;
}

//	垂直方向視野角のセット
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

//	アスペクト比のセット
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

//	ニアクリップのセット
void Camera::SetNearClip(float nearclip)
{
	m_NearClip = nearclip;
}

//	ファークリップのセット
void Camera::SetFarClip(float farclip)
{
	m_FarClip = farclip;
}
