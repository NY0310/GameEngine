#pragma once
#include <D3DX10math.h>
#include <DirectXColors.h>
#include "../Object.h"

namespace NYLibrary
{
	class ColorObject : public virtual Object
	{
	public:
		//�F��ݒ肷��
		void SetColor(const D3DXVECTOR4& color) { this->color = color; }
		//�F���擾����
		const D3DXVECTOR4& GetColor() { return color; }
	private:
		//�F
		D3DXVECTOR4 color;
	};
}