#pragma once
#include <vector>
#include "../../NYLibrary/Sprite/Sprite.h"
#include "PaintGun.h"

class InputInkUI : public NYLibrary::Sprite
{
public:
	//コンストラクタ
	InputInkUI();
	//描画するテクスチャを設定する
	void SetRenderTexture(InkTank::StandardColor standardColor);
private:
	//シェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView> srv[InkTank::StandardColor::total];
	//座標係数
	static const D3DXVECTOR3 positionCoefficient;
};
