#pragma once
#include <memory>
#include "../InkParticle.h"
#include "../Input/MouseRay.h"
#include "../CollisionNode.h"
#include "../AStar/Math.h"
#include "../Sprite/Sprite.h"
#include "../Device.h"
#include "../Node/GameObjectNode.h"

class PaintGun : public scenegraph::GameObjectNode
{
public:
	//�R���X�g���N�^
	PaintGun();
	//������
	void Initialize();
	//�X�V����
	void Update();
	//�`�揈��
	void Render();
	//�I������
	void Finalize();
	//���ˈʒu��ݒ肷��
	void SetEmitPosition(const D3DXVECTOR3& emitPosition) {
		this->emitPosition = emitPosition
			;
	}

private:
	//�C���N����
	void Emit();
	//�G�C���̍X�V����
	void AimUpdate();
	//�C���N�̃p�[�e�B�N���}�l�[�W���[
	std::unique_ptr<InkParticleManager> inkParticleManager;
	//�G�C��
	std::unique_ptr<Sprite> aim;
	//���ˍ��W
	D3DXVECTOR3 emitPosition;
};




class InkTank
{
public:
	//�������@
	void Initialize();
	//�X�V����
	void Update();
	//����
	void Emit();
private:
	enum StandardColor
	{
		red,
		green,
		blue,
		total
	};
	//�F���v�Z
	void CalcColor();
	//�ǉ�����F��ύX����
	void ChangeColor();
	D3DXVECTOR4 color;
	//�C���N��
	int colorAmount[total + 1];
	//�ǉ�����F
	StandardColor inColor;
};