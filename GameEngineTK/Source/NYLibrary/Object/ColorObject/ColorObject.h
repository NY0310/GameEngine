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
		//�F��ݒ肷��
		void SetColor(const D3DXVECTOR4& color) { this->color = color; isUseColor = true; }
		//�F���擾����
		const D3DXVECTOR4& GetColor() { return color; }
		//�F���g�����ݒ�
		void SetIsUseColor(bool isUseColor) {
			this->isUseColor = isUseColor;
		}
		//�F���g�����擾
		bool GetIsUseColor() { return isUseColor; }
	private:
		//�F
		D3DXVECTOR4 color;
		bool isUseColor;//�F�𔽉f�����邩

	};
}