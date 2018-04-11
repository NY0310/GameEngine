#pragma once
#include "../../../../PaintAsset/PaintSystem/PaintObj.h"
#include "../Character.h"
#include "../../../../PaintAsset/PaintGun/PaintGun.h"
#include "WeakSprite.h"

class Enemy : public PaintObj , public Character
{
public:
	//�R���X�g���N�^
	Enemy();
	//�f�X�g���N�^
	~Enemy() = default;
	// �R�s�[�R���X�g���N�^�֎~
	Enemy(const Enemy&) = delete;
	// ����֎~
	Enemy& operator=(const Enemy&) = delete;
	//�q�𐶐����q���Ƃ��ēo�^
	void CreateAddChild();

	//����������
	void Initialize();
	//
	void Update();
	//�����蔻��
	void OnCollisiton(Collider * collider);
	//���񂾂Ƃ�
	void OnDied();

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
		DirectX::Colors::Pink,
		DirectX::Colors::Yellow,
	};
private:
	//HP���
	static const int MAX_HP = 3;
	//�y�C���g�e
	std::shared_ptr<PaintGun> paintGun;
	//��_�F
	D3DXVECTOR4 weakColor;
	//�덷���
	static const float DIFFERENCE_UPPER;
	//��_UI
	std::shared_ptr<WeakSprite> weakSprite;
};