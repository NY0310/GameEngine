/// <summary>
/// カメラを制御するクラス
/// </summary>

//	多重インクルードの防止
#pragma once

//	ヘッダファイルのインクルード
#pragma once;

#include "../Collision/Collision.h"
#include "../Device/Device.h"

namespace NYLibrary
{
	class Camera
	{
	public:
		//	コンストラクタ
		Camera();
		//	デストラクタ
		virtual ~Camera();

		//	更新
		virtual void Update();

		//bool Project(const Vector3 & worldPos, Vector2 * screenPos);

		//void UnProject(const Vector2 & screenPos, Segment * worldSegment);

		bool Project(const D3DXVECTOR3& worldPos, D3DXVECTOR2* screenPos);

		void UnProject(const D3DXVECTOR2 & screenPos, Segment * worldSegment);

		const D3DXMATRIX& GetBillboard() const { return billboard; }
		const D3DXMATRIX& GetBillboardConstrainY() const { return billboardConstrainY; }

		//	カメラの位置(視点座標）の取得
		const D3DXVECTOR3& GetEyePos() { return eyepos; }

		//	カメラの見ている先(注視点/参照点)の取得
		const D3DXVECTOR3& GetRefPos() { return refpos; }

		//カメラの上方向ベクトルの取得
		const D3DXVECTOR3& GetUpVec() { return upvec; }

		//	ビュー行列の取得
		const D3DXMATRIX& GetView();

		//	射影行列の取得
		const D3DXMATRIX& GetProjection();

		//拡大倍率の取得
		const float& GetZoom() { return zoom; }

		//	視点座標のセット
		void SetEyePos(const D3DXVECTOR3& eyepos);

		//	参照点のセット
		void SetRefPos(const D3DXVECTOR3& refpos);

		//	上方向ベクトルのセット
		void SetUpVec(const D3DXVECTOR3& upvec);

		//	垂直方向視野角のセット
		void SetFovY(float fovY);

		//	アスペクト比のセット
		void SetAspect(float aspect);

		//	ニアクリップのセット
		void SetNearClip(float nearclip);

		//	ファークリップのセット
		void SetFarClip(float farclip);

		//拡大倍率のセット
		void SetZoom(float zoom) { this->zoom = zoom; }

	protected:
		//	カメラの位置(視点座標）
		D3DXVECTOR3 eyepos;
		//	カメラの見ている先(注視点/参照点)
		D3DXVECTOR3 refpos;
		//	カメラの上方向ベクトル
		D3DXVECTOR3 upvec;
		//	ビュー行列
		D3DXMATRIX view;
		//	垂直方向視野角
		float fovY;
		//	アスペクト比(横縦の比率）
		float aspect;
		// ニアクリップ(手前の表示限界距離)
		float nearClip;
		//	ファークリップ(奥の表示限界距離)
		float farClip;
		//	射影行列
		D3DXMATRIX proj;
		// ビルボード行列
		D3DXMATRIX billboard;
		// ビルボード行列(Y軸周り限定）
		D3DXMATRIX billboardConstrainY;
		//拡大倍率
		float zoom;
	};
};