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
		//�����x�擾
		float GetTransparency()
		{
			return transparency;
		}
		//�����x�ݒ�
		void SetTransparency(float transparency) {
			this->transparency = transparency;
		}
		//�\�����邩���擾
		bool GetVisible() { return isVisible; }
		//�\�����邩��ݒ�
		void SetVisible(bool isVisible) { this->isVisible = isVisible; }
	private:
		D3DXVECTOR4 color;//�F
		bool isUseColor;//�F�𔽉f�����邩
		float transparency;//�����x
		bool isVisible;//�\�����邩
	};
}