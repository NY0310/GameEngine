#pragma once
#include <memory>
#include "../InkParticle/InkParticle.h"
#include "../../NYLibrary/Collision/Collision.h"
#include "../../NYLibrary/Device/Device.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "../../NYLibrary/Input/MouseRay.h"
#include "Aim.h"

class PaintGun : public NYLibrary::GameObjectNode
{
public:
	//�R���X�g���N�^
	PaintGun();
	void CreateAddChild();
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
		this->emitPosition = emitPosition;
	}
private:
	//�C���N����
	void Emit();
	//�C���N�̃p�[�e�B�N���}�l�[�W���[
	std::shared_ptr<InkParticleManager> inkParticleManager;
	//�G�C��
	std::shared_ptr<Aim> aim;
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