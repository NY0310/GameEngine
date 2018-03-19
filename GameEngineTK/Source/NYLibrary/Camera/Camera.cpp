//	ヘッダファイルのインクルード
#include "Camera.h"
using namespace NYLibrary;

//	コンストラクタ
Camera::Camera()
{
	//	メンバ変数の初期化
	eyepos = D3DXVECTOR3(0, 0, 5);
	refpos = D3DXVECTOR3(0, 0, 0);
	upvec = D3DXVECTOR3(0, 1, 0);

	fovY = XMConvertToRadians(60.0f);
	aspect = static_cast<float> (Devices::Get().Width()) / static_cast <float>(Devices::Get().Height());
	nearClip = 0.1f;
	farClip = 1000.0f;
	zoom = 1.0f;
	D3DXMatrixIdentity(&proj);

	//	ビュー行列の生成
	D3DXMatrixLookAtRH(&view,&eyepos, &refpos,&upvec);

	//	射影行列の生成
	D3DXMatrixPerspectiveFovRH(&proj,fovY / zoom, aspect, nearClip, farClip);
}

//	デストラクタ
Camera::~Camera()
{

}

//	更新
void Camera::Update()
{
	//	ビュー行列の生成
	D3DXMatrixLookAtRH(&view, &eyepos, &refpos, &upvec);

	//	射影行列の生成
	D3DXMatrixPerspectiveFovRH(&proj, fovY / zoom, aspect, nearClip, farClip);

}


/// <summary>
/// ３Ｄ→２Ｄ座標変換
/// ワールド座標からスクリーン座標を計算する
/// </summary>
/// <param name="pos2d"></param>
///// <returns>成否</returns>
bool Camera::Project(const D3DXVECTOR3& worldPos, D3DXVECTOR2* screenPos)
{
	auto& devices = Devices::Get();

	D3DXVECTOR4 clipPos;
	D3DXVECTOR4 worldPosV4(worldPos.x, worldPos.y, worldPos.z, 1.0f);

	// ビュー変換
	D3DXVec4Transform(&clipPos,&worldPosV4,&view);

	// 射影変換
	D3DXVec4Transform(&clipPos,&clipPos,&proj);

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
void Camera::UnProject(const D3DXVECTOR2& screenPos, Segment* worldSegment)
{
	auto& devices = Devices::Get();


	D3DXVECTOR2 clipPos;
	D3DXVECTOR4 clipPosNear;
	D3DXVECTOR4 clipPosFar;

	// ビューポートの取得
	//D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();

	// スクリーン座標→射影座標
	/*clipPos.x = (screenPos.x - viewport.TopLeftX) / (viewport.Width / 2.0f) - 1.0f;
	clipPos.y = (screenPos.y - viewport.TopLeftY) / (viewport.Height / 2.0f) - 1.0f;*/
	clipPos.y = -clipPos.y;

	clipPosNear.x = nearClip * clipPos.x;
	clipPosNear.y = nearClip * clipPos.y;
	clipPosNear.z = 0;
	clipPosNear.w = nearClip;

	clipPosFar.x = farClip * clipPos.x;
	clipPosFar.y = farClip * clipPos.y;
	clipPosFar.z = farClip;
	clipPosFar.w = farClip;

	// プロジェクション、ビュー逆変換
	D3DXMATRIX invMat = view * proj;
	D3DXMatrixIdentity(&invMat);

	D3DXMATRIX invView;
	D3DXMatrixIdentity(&invView);

	D3DXMATRIX invProj;
	D3DXMatrixIdentity(&invProj);

	// 射影座標→ビュー座標
	D3DXVECTOR4 viewStart, viewEnd, start, end;
	D3DXVec4Transform(&viewStart,&clipPosNear, &invProj);
	D3DXVec4Transform(&viewEnd,&clipPosFar, &invProj);
	// ビュー座標→ワールド座標
	D3DXVec4Transform(&start,&viewStart, &invView);
	D3DXVec4Transform(&end,&viewEnd, &invView);

	worldSegment->start.x = start.x;
	worldSegment->start.y = start.y;
	worldSegment->start.z = start.z;

	worldSegment->end.x = end.x;
	worldSegment->end.y = end.y;
	worldSegment->end.z = end.z;
}



//	ビュー行列を取得する関数
const D3DXMATRIX& Camera::GetView()
{
	return view;
}

//	射影行列を取得する関数
const D3DXMATRIX& Camera::GetProjection()
{
	return proj;
}

//	視点座標のセットする関数
void Camera::SetEyePos(const D3DXVECTOR3& eyepos)
{
	this->eyepos = eyepos;
}

//	参照点のセット
void Camera::SetRefPos(const D3DXVECTOR3& refpos)
{
	this->refpos = refpos;
}

//	上方向ベクトルのセット
void Camera::SetUpVec(const D3DXVECTOR3 & upvec)
{
	this->upvec = upvec;
}

//	垂直方向視野角のセット
void Camera::SetFovY(float fovY)
{
	this->fovY = fovY;
}

//	アスペクト比のセット
void Camera::SetAspect(float aspect)
{
	aspect = aspect;
}

//	ニアクリップのセット
void Camera::SetNearClip(float nearclip)
{
	nearClip = nearclip;
}

//	ファークリップのセット
void Camera::SetFarClip(float farclip)
{
	farClip = farclip;
}
