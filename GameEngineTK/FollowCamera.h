
/* ---- 多重インクルードの防止 ---------- */
#pragma once
/* ---- ライブラリのインクルード ---------- */
#include <d3d11.h>
#include "Camera.h"

class FollowCamera : public Camera
{
public:
	FollowCamera(int width, int height);
	//FollowCamera();
	~FollowCamera();
	void Update() override;

	// 
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);
	void SetTargetAngle(float targetangle);
	//実機とカメラの距離
	static const float CAMERA_DISTANCE;
protected:
	//追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	//追従
	float m_targetAngle;
	
};

