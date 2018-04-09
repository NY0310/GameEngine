/// <summary>
/// 自機に追従するカメラクラス
/// </summary>

//	多重インクルードの防止
#pragma once

#include <Windows.h>
#include "Camera.h"
class Player;

namespace NYLibrary
{
	class FollowCamera : public Camera
	{
	public:
		static FollowCamera* GetInstance();

		//	自機とカメラの距離
		static const float CAMERA_DISTANCE;
		FollowCamera& operator= (const FollowCamera&) = delete;
		FollowCamera(const FollowCamera&) = delete;
		//	更新
		void Update() override;
		//プレイヤをセット
		void SetPlayer(Player* player) { this->player = player; }
	private:
		//	コンストラクタ
		FollowCamera();
		//自クラスインスタンス
		static  std::unique_ptr<FollowCamera> camera;
	protected:
		//	FPSフラグ
		bool isFPS;
		Player* player;
	};
};
