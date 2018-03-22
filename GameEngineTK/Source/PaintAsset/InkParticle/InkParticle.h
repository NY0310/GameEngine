#pragma once
//�w�b�_�[�t�@�C���̃C���N���[�h

#include <d3d11_1.h>
#include <vector> 
#include <memory>
#include "../../NYLibrary/Collision/Collision.h"
#include "InkObj2.h"
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
	//�ړ��x�N�g��
	D3DXVECTOR3 direction;
	//���x	
	static const float SPEED;
	//�\�����Ă���̎���
	int birthFrame;
	//������
	bool isValidity;
	//�����蔻��p�̐�
	std::unique_ptr<NYLibrary::InkSegment> colisitionSegment;
	//�������x
	static const float GRAVITY;
	//���[�J���̑傫��
	static const float ROCAL_SIZE;
	//�C���N�L������
	static const int LIFE_FRAME = 120;
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
	////���W���擾
	//const std::vector<InkParticleRenderer::ConstantInkData> GetConstantInkData() {
	//	return inkParticledata;
	//}
	//�����蔻��p�̋����擾
	const std::vector<InkSegment*> GetSegment() {
		return std::vector<InkSegment*>();
	}
	//�C���N�̍폜
	void Destroy(int inkSegmentIndex);
	//�p�[�e�B�N���ő吔
	static const int MAX_PARTICLE = 100;
private:
	//�C���N�p�[�e�B�N���̍X�V����
	void InkParticleUpdate();
	////���̍X�V����
	//void SegmentsUpdate();
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
	static const int MAX_SHIFT_DIRECTION = 100;
	//�C���N�̃p�[�e�B�N��
	std::shared_ptr<InkParticle> inkParticle[MAX_PARTICLE];
	//�C���N�̕`���
	std::unique_ptr<InkObj2> renderer;
	//�C���N���o���Ă���܂��o���܂ł̃C���^�[�o��
	const int INTERVAL_FRAME = 140;
	//�O��C���N���o���Ă���̌o�ߎ���
	int intervalFrameCnt;
	//�����蔻��p�̐�
	std::vector<InkSegment*> segments;
	//���W���擾
	std::vector<InkObj2::InkData> inkParticledata;
	//���˂�����
	bool isShoot;
public:
	// �R�s�[�R���X�g���N�^�֎~
	InkParticleManager(const InkParticleManager&) = delete;
	// ����֎~
	InkParticleManager& operator=(const InkParticleManager&) = delete;
};