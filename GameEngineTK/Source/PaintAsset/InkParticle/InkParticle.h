#pragma once
//�w�b�_�[�t�@�C���̃C���N���[�h

#include <d3d11_1.h>
#include <vector> 
#include <memory>
#include "../../NYLibrary/Collision/Collision.h"
#include "InkObj.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "../InkSegmentCollider/InkSegmentCollider.h"

/// <summary>
/// �C���N��H��
/// </summary>
class InkParticle : public NYLibrary::GameObjectNode
{
public:
	//�R���X�g���N�^
	InkParticle();
	//�f�X�g���N�^
	~InkParticle();
	//����(���Ƀ������͊m�ۂ���Ă���)
	void Create(const D3DXVECTOR3& position,const D3DXVECTOR3& nDirection, const D3DXVECTOR4& color, const int index);
	//�X�V����
	void Update();
	//�`�悷�邩
	bool IsValidity() { return isValidity; }
	//�폜(�������͉�����Ȃ�)
	void Destroy() {
		SetComponentActive(false);
		isValidity = false;
		birthFrame = 0;
	}

private:
	//�ړ�
	void Move();
	//�N�H�[�^�j�I���쐬
	void CalcQuaternion();
	//�����c���Ă��邩�m�F
	void LifeCheck();
	//�����蔻��
	void OnCollisiton(Collider* collider);
	//�ړ��x�N�g��
	D3DXVECTOR3 direction;
	//���x	
	static const float SPEED;
	//�\�����Ă���̎���
	int birthFrame;
	//������
	bool isValidity;
	//�������x
	static const float GRAVITY;
	//���[�J���̑傫��
	static const float ROCAL_SIZE;
	//�C���N�L������
	static const int LIFE_FRAME = 50;
public:
	// �R�s�[�R���X�g���N�^�֎~
	InkParticle(const InkParticle&) = delete;
	// ����֎~
	InkParticle& operator=(const InkParticle&) = delete;
};

/// <summary>
/// �C���N�̊Ǘ�
/// </summary>
class InkParticleManager : public NYLibrary::GameObjectNodeEmpty
{
public:
	//�R���X�g���N�^
	InkParticleManager();
	//�f�X�g���N�^
	~InkParticleManager();
	//������
	void Initialize();
	//�X�V����
	void Update();
	//�`��
	void Render();
	//�C���N�𔭎˂���
	void Shoot(const D3DXVECTOR3& emitPosition, D3DXVECTOR3& nDirection, const D3DXVECTOR4& color);
	//�p�[�e�B�N���ő吔
	static const int MAX_PARTICLE = 50;
private:
	//���W�̍X�V����
	void InkDataUpdate();
	//�C���^�[�o���X�V����
	void IntervalUpdate();
	//���������炷
	D3DXVECTOR3 ShiftDirection(D3DXVECTOR3& direction);
	//float����̂��炵���Z�o
	float RandShiftDirection();
private:
	//�����̂��炵�ő��
	static const int MAX_SHIFT_DIRECTION;
	//�C���N�̃p�[�e�B�N��
	std::shared_ptr<InkParticle> inkParticle[MAX_PARTICLE];
	//�C���N�̕`���
	std::unique_ptr<InkObj> renderer;
	//�C���N���o���Ă���܂��o���܂ł̃C���^�[�o��
	static const int INTERVAL_FRAME;
	//�O��C���N���o���Ă���̌o�ߎ���
	int intervalFrameCnt;
	//���W���擾
	std::vector<InkObj::InkData> inkParticledata;
	//���˂�����
	bool isShoot;
public:
	// �R�s�[�R���X�g���N�^�֎~
	InkParticleManager(const InkParticleManager&) = delete;
	// ����֎~
	InkParticleManager& operator=(const InkParticleManager&) = delete;
};