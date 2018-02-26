#pragma once
#include <memory>
#include "../InkParticle.h"
#include "../Input/MouseRay.h"
#include "../CollisionNode.h"
#include "../AStar/Math.h"
#include "../Sprite/Sprite.h"
#include "../Device.h"

class PaintGun
{
public:
	PaintGun();
	//������
	void Initialize();
	//�X�V����
	void Update();
	//�`�揈��
	void Render();
	//�I������
	void Finalize();
private:
	//�C���N����
	void Shoot();
	//�G�C���̍X�V����
	void AimUpdate();
	//�C���N�̃p�[�e�B�N���}�l�[�W���[
	std::unique_ptr<InkParticleManager> inkParticleManager;
	//�G�C��
	std::unique_ptr<Sprite> aim;
};




class InkTank
{
public:
	void Update();
	void Reset();
	void Shoot();
private:
	enum StandardColor
	{
		red,
		green,
		yellow
	};
	void CalcColor();
	void ColorChange();
	D3DXVECTOR4 color;
	int redAmount;
	int greenAmount;
	int yellowAmount;
	int maxInk;
	StandardColor inColor;
};