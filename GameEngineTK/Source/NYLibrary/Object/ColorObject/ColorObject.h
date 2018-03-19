#pragma once
#include <D3DX10math.h>
#include <DirectXColors.h>
#include "../Object.h"

namespace NYLibrary
{
	class ColorObject : public virtual Object
	{
	public:
		//F‚ğİ’è‚·‚é
		void SetColor(const D3DXVECTOR4& color) { this->color = color; }
		//F‚ğæ“¾‚·‚é
		const D3DXVECTOR4& GetColor() { return color; }
	private:
		//F
		D3DXVECTOR4 color;
	};
}