#pragma once
#include <memory>
#include "../InkParticle/InkParticle.h"
#include "../../NYLibrary/Collision/Collision.h"
#include "../../NYLibrary/Device/Device.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "Aim.h"
#include "InkTank.h"

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
	void SetAimMatirx(const std::shared_ptr<MatrixObject>& aimMatirx) {
		this->aimMatirx = aimMatirx;
	}
private:
	//�C���N����
	void Emit();
	//�C���N�̃p�[�e�B�N���}�l�[�W���[
	std::shared_ptr<InkParticleManager> inkParticleManager;
	//�G�C��
	std::shared_ptr<Aim> aim;
	//���ˍ��W
	std::shared_ptr<MatrixObject> aimMatirx;
	//�C���N�̃^���N
	std::shared_ptr<InkTank> inkTank;
};



