#pragma once
#include <vector>
#include "../../NYLibrary/Sprite/Sprite.h"
#include "PaintGun.h"

class InputInkUI : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	InputInkUI();
	//�`�悷��e�N�X�`����ݒ肷��
	void SetRenderTexture(InkTank::StandardColor standardColor) { SetShaderResourceView( srv[standardColor]); }
private:
	//�V�F�[�_�[���\�[�X�r���[
	ComPtr<ID3D11ShaderResourceView> srv[InkTank::StandardColor::total];
};
