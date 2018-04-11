#pragma once
#include <D3DX10math.h>
#include <DirectXColors.h>
#include "../Object.h"

namespace NYLibrary
{
	class ColorObject : public virtual Object
	{
	public:
		ColorObject() { isUseColor = false; }
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
	private:
		//色
		D3DXVECTOR4 color;
		bool isUseColor;//色を反映させるか

	};
}