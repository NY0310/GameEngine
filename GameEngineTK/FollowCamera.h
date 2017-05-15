
/* ---- ���d�C���N���[�h�̖h�~ ---------- */
#pragma once
/* ---- ���C�u�����̃C���N���[�h ---------- */
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
	//���@�ƃJ�����̋���
	static const float CAMERA_DISTANCE;
protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//�Ǐ]
	float m_targetAngle;
	
};

