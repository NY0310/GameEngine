/// <summary>
/// カメラを制御するクラス
/// </summary>

//	多重インクルードの防止
#pragma once

//	ヘッダファイルのインクルード
#include <d3d11_1.h>
#include <SimpleMath.h>

#include "Collision.h"
#include "Device.h"
class Camera
{
public:
	//	コンストラクタ
	Camera(int width, int height);
	//	デストラクタ
	virtual ~Camera();

	//	更新
	virtual void Update();

	//bool Project(const Vector3 & worldPos, Vector2 * screenPos);

	//void UnProject(const Vector2 & screenPos, Segment * worldSegment);

	bool Project(const DirectX::SimpleMath::Vector3 & worldPos, DirectX::SimpleMath::Vector2 * screenPos);

	void UnProject(const DirectX::SimpleMath::Vector2 & screenPos, Segment * worldSegment);

	const DirectX::SimpleMath::Matrix& GetBillboard() const { return m_Billboard; }
	const DirectX::SimpleMath::Matrix& GetBillboardConstrainY() const { return m_BillboardConstrainY; }

	//	カメラの位置(視点座標）の取得
	const DirectX::SimpleMath::Vector3& GetEyePos() { return m_eyepos; }

	//	カメラの見ている先(注視点/参照点)の取得
	const DirectX::SimpleMath::Vector3& GetRefPos() { return m_refpos; }

	//カメラの上方向ベクトルの取得
	const DirectX::SimpleMath::Vector3& GetUpVec() { return m_upvec; }

	//	ビュー行列の取得
	const DirectX::SimpleMath::Matrix& GetView();

	//	射影行列の取得
	const DirectX::SimpleMath::Matrix& GetProjection();

	//拡大倍率の取得
	const float& GetZoom() { return m_zoom; }

	//	視点座標のセット
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);

	//	参照点のセット
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);

	//	上方向ベクトルのセット
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);

	//	垂直方向視野角のセット
	void SetFovY(float fovY);

	//	アスペクト比のセット
	void SetAspect(float aspect);

	//	ニアクリップのセット
	void SetNearClip(float nearclip);

	//	ファークリップのセット
	void SetFarClip(float farclip);

	//拡大倍率のセット
	void SetZoom(float zoom) { m_zoom = zoom; }

protected:
	//	カメラの位置(視点座標）
	DirectX::SimpleMath::Vector3 m_eyepos;
	//	カメラの見ている先(注視点/参照点)
	DirectX::SimpleMath::Vector3 m_refpos;
	//	カメラの上方向ベクトル
	DirectX::SimpleMath::Vector3 m_upvec;
	//	ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//	垂直方向視野角
	float m_fovY;
	//	アスペクト比(横縦の比率）
	float m_aspect;
	// ニアクリップ(手前の表示限界距離)
	float m_NearClip;
	//	ファークリップ(奥の表示限界距離)
	float m_FarClip;
	//	射影行列
	DirectX::SimpleMath::Matrix m_proj;
	// ビルボード行列
	DirectX::SimpleMath::Matrix m_Billboard;
	// ビルボード行列(Y軸周り限定）
	DirectX::SimpleMath::Matrix m_BillboardConstrainY;
	//拡大倍率
	float m_zoom;
};