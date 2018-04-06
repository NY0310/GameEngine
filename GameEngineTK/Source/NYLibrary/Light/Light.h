/// <summary>
/// ライト
/// </summary>
#pragma once
#include <memory>
#include "../Object/MatrixObject/MatrixObject.h"

namespace NYLibrary
{
	class Light : public MatrixObject
	{
	public:
		//デストラクタ
		~Light() = default;
		//コピーコンストラクタ禁止
		Light(const Light&) = delete;
		//代入禁止
		Light& operator=(const Light&) = delete;
		static Light* GetInstance();
		////更新
		//void Update();
		//ライトビューを取得
		const D3DXMATRIX& GetLightView() { return view; }
		//ライト視点を設定
		void SetLightEyePosition(const D3DXVECTOR3& eyePosition) { this->eyePosition = eyePosition; }
		//ライト視点を取得
		D3DXVECTOR3 GetLightPosition() { return eyePosition; }
		//行列計算
		void Calc();
	private:
		Light();
		//引数なしのコンストラクタ
		static std::unique_ptr<Light> light;//ライト
		D3DXMATRIX view;//ビュー
		D3DXVECTOR3 eyePosition;//視点
		D3DXVECTOR3 refPosition;//注視点
		D3DXVECTOR3 upVector;//上方向ベクトル
	};
};