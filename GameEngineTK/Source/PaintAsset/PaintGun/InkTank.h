#pragma once
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "../../NYLibrary/Input/KeyboardUtil.h"
#include "../../NYLibrary/Input/MouseUtil.h"
#include "InkTankUI.h"

class InputInkUI;

class InkTank :public NYLibrary::GameObjectNodeEmpty
{
public:
	//�q�����쐬���o�^
	void CreateAddChild();
	//�������@
	void Initialize();
	//�X�V����
	void Update();
	//�C���N���o��
	void InkLost();
	//�C���N�̐F���擾����
	const D3DXVECTOR4& GetInkColor() { return inkColor; }
	//�F�̎��
	enum StandardColor
	{
		red,
		green,
		blue,
		total
	};
	//�C���N����
	bool IsEmptyInk() { return colorAmount[total] == 0; }
private:
	//�O�t���[���̃z�C�[������
	int oldWheel;
	//�C���N�ʂ��v�Z
	void CalcTotalAmount();
	//�F���v�Z
	void CalcColor();
	//�ǉ�����F��ύX����
	void ChangeColor();
	//�C���N�̐F
	D3DXVECTOR4 inkColor;
	//�C���N��
	float colorAmount[total + 1];
	//�ǉ�����F
	StandardColor inColor;
	//�ő�C���N��
	static int const MAX_INK = 50;
	//����C���N��UI
	std::shared_ptr<InputInkUI> inputInkUI;
	//�C���N�^���NUI
	std::shared_ptr<InkTankUI> inkTankUI;
	//���₷�C���N��
	static const int ADD_INK = 1;
};