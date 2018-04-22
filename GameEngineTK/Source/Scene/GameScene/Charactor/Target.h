#pragma once
#include "../../../PaintAsset/PaintSystem/PaintObj.h"
#include "Character.h"
#include "WeakSprite.h"

class Target : public PaintObj , public Character
{
public:
	//�R���X�g���N�^
	Target(LPSTR fileName);
	Target() = delete;
	//�f�X�g���N�^
	~Target() = default;
	// �R�s�[�R���X�g���N�^�֎~
	Target(const Target&) = delete;
	// ����֎~
	Target& operator=(const Target&) = delete;
	//�������q���Ƃ��ēo�^
	void CreateAddChild();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�����蔻��
	void OnCollisiton(Collider * collider);
	//���񂾂Ƃ�
	virtual void OnDied();
	//��_��
	bool IsWeakColor(const D3DXVECTOR4 & hitColor);
	//��_�F��ύX����
	void ChangeWeakColor();
	enum WeakColorKind
	{
		red,
		green,
		blue,
		aqua,
		pink,
		yellow,
		total
	};

	//��_���X�g
	D3DXVECTOR4  WeakColorList[total]
	{
		DirectX::Colors::Red,
		DirectX::Colors::Green,
		DirectX::Colors::Blue,
		DirectX::Colors::Aqua,
		DirectX::Colors::Magenta,
		DirectX::Colors::Yellow,
	};
private:
	//HP���
	static const int MAX_HP = 3;
	//��_�F
	D3DXVECTOR4 weakColor;
	//�덷���
	static const float DIFFERENCE_UPPER;
protected:
	//��_UI
	std::shared_ptr<WeakSprite> weakSprite;
};