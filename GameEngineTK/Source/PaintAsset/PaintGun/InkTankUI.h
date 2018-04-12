#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"
#include "InkContainerUI.h"

class InkTankUI : public NYLibrary::Sprite
{
public:
	//�R���X�g���N�^
	InkTankUI();
	//�I�u�W�F�N�g�𐶐����q���Ƃ��ēo�^
	void CreateAddChild();
	//������
	void Initialize();
	//�C���N�^���N�̗ʂƐF��ݒ肷��
	void SetInkTankData(float proportion, D3DXVECTOR4 color) { SetScaleY(proportion); SetColor(color); }
private:
	//���W�W��
	static const D3DXVECTOR3 positionCoefficient;
	//�C���N�̗e��̃X�v���C�g	
	std::shared_ptr<InkContainerUI> inkContainerUI;
};
