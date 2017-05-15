/// <summary>
///	カメラを制御するクラス
/// </summary>

/* ---- 多重インクルードの防止 ---------- */
#pragma once
/* ---- ライブラリのインクルード ---------- */
#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
public:
	//コンストラクタ
	Camera();
	//
	Camera(int width, int height);
	//デストラクタ
	virtual ~Camera();
	//更新処理
	virtual void Update();
	//ビュー行列取得
	DirectX::SimpleMath::Matrix GetViewMatrix()
	{
		return m_viewmat;
	}
	//射影行列の取得
	DirectX::SimpleMath::Matrix GetProjectionMatrix()
	{
		return m_projmat;
	}
	//視点座標の取得
	DirectX::SimpleMath::Vector3 GetEyepos()
	{
		return m_eyepos;
	}
	//参照点の取得
	DirectX::SimpleMath::Vector3 GetRefPos()
	{
		return m_refpos;
	}
	//上方向ベクトル
	DirectX::SimpleMath::Vector3 GetUpVec()
	{
		return m_upvec;
	}
	//垂直方向視野角の取得
	float GetFovY()
	{
		return m_fovY;
	}
	//アスペクト比の取得
	float GetAspect()
	{
		return m_aspect;
	}
	//ニアクリップの取得
	float GetNearClip()
	{
		return m_near_clip;
	}
	//ファークリップの取得
	float GetFarClip()
	{
		return m_far_clip;
	}


	//視点座標のセット
	void SetEyePos(DirectX::SimpleMath::Vector3 eyepos)
	{
		m_eyepos = eyepos;
	}
	////参照点のセット
	void SetRefPos(DirectX::SimpleMath::Vector3 refpos)
	{
		m_refpos = refpos;
	}
	//上方向ベクトルのセット
	void SetUpvec(DirectX::SimpleMath::Vector3 upvec)
	{
		m_upvec = upvec;
	}
	//垂直方向視野角の設定
	void SetFovY(float fory)
	{
		m_fovY = fory;
	}
	//アスペクト比の設定
	void SetAspect(float aspect)
	{
		m_aspect = aspect;
	}
	//ニアクリップの設定
	void SetNearClip(float nearclip)
	{
		m_near_clip = nearclip;
	}
	//ファークリップの設定
	void SetFarClip(float farclip)
	{
		m_far_clip = farclip;
	}
protected:
	//ビュー行列
	DirectX::SimpleMath::Matrix  m_viewmat;
	//視点座標（カメラの位置）
	DirectX::SimpleMath::Vector3 m_eyepos;
	//参照点/注視点(カメラの見ている先）
	DirectX::SimpleMath::Vector3 m_refpos;
	//上方向ベクトル
	DirectX::SimpleMath::Vector3 m_upvec;
	//射影行列
	DirectX::SimpleMath::Matrix m_projmat;
	//垂直方向視野角(縦方向に何度までうつるか)
	float m_fovY;
	//アスペクト比(横縦の比率)
	float m_aspect;
	//ニアクリップ(手前の表示限界距離)
	float m_near_clip;
	//ファークリップ(奥の表示限界距離)
	float m_far_clip;
};


