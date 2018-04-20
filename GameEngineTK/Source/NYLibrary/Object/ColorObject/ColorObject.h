#pragma once
#include <D3DX10math.h>
#include <DirectXColors.h>
#include "../Object.h"

namespace NYLibrary
{
	class ColorObject : public virtual Object
	{
	public:
		ColorObject() { isUseColor = false; transparency = 1;  isVisible = true; }
		//色を設定する
		void SetColor(const D3DXVECTOR4& color) { this->color = color; isUseColor = true; }
		//色を取得する
		const D3DXVECTOR4& GetColor() { return color; }
		//色を使うか設定
		void SetIsUseColor(bool isUseColor) {
			this->isUseColor = isUseColor;
		}
		//色を使うか取得
		bool GetIsUseColor() { return isUseColor; }
		//透明度取得
		float GetTransparency()
		{
			return transparency;
		}
		//透明度設定
		void SetTransparency(float transparency) {
			this->transparency = transparency;
		}
		//表示するかを取得
		bool GetVisible() { return isVisible; }
		//表示するかを設定
		void SetVisible(bool isVisible) { this->isVisible = isVisible; }
	private:
		D3DXVECTOR4 color;//色
		bool isUseColor;//色を反映させるか
		float transparency;//透明度
		bool isVisible;//表示するか
	};
}