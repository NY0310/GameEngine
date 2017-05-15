/* ---- ライブラリのインクルード ---------- */
#include "Camera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
/***************************************************************************
*|	概要　　コンストラクタ
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
Camera::Camera()
	:m_eyepos(0,0,0)
	,m_refpos(0,0,0)
	,m_upvec(0,0,0)
	,m_fovY(XMConvertToRadians(60.0f))
	,m_near_clip(0.1f)
	,m_far_clip(0.1f)
{
	//ビュー行列生成
	//m_viewmat = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//射影行列の生成
	//m_projmat = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_near_clip, m_far_clip);
}
/***************************************************************************
*|	概要　　コンストラクタ
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
Camera::Camera(int width, int height)
	:m_eyepos(0, 0, 5)
	,m_refpos(0, 0, 0)
	,m_upvec(0, 1, 0)
	,m_fovY(XMConvertToRadians(60.0f))
	,m_aspect(width / height)
	,m_near_clip(0.1f)
	,m_far_clip(1000.0f)
{
	//角度を保ったまま長さを1にする
	m_upvec.Normalize();
	//ビュー行列生成
	m_viewmat = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//射影行列の生成
	m_projmat = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_near_clip, m_far_clip);
}
/***************************************************************************
*|	概要　　デストラクタ
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
Camera::~Camera()
{
}

/***************************************************************************
*|	概要　　更新処理
*|	引数　　無し
*|　戻り値　無し
****************************************************************************/
void Camera::Update()
{
	//ビュー行列生成
	m_viewmat = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//射影行列の生成
	m_projmat = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_near_clip, m_far_clip);
}
