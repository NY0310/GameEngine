#pragma once
//�w�b�_�[�t�@�C���̃C���N���[�h

#include <d3d11_1.h>
#include <vector> 
#include <memory>
#include <DirectXColors.h>
#include <cassert>
#include "../../NYLibrary/Collision/Collision.h"
#include "InkObj2.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"


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
	/*	assert(!isValidity, ("���̃C���N�͊��ɖ�����Ԃł�"));*/
		isValidity = false;}
	//�����蔻��p�̐����擾
	NYLibrary::InkSegment* GetSegment() {
		return colisitionSegment.get();
	}
	//const D3DXMATRIX& GetWVP() {
	//	auto a =  GetWVP();
	//	return a;
	//}
private:
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
public:
	// �R�s�[�R���X�g���N�^�֎~
	InkParticle(const InkParticle&) = delete;
	// ����֎~
	InkParticle& operator=(const InkParticle&) = delete;
	//�N�H�[�^�j�I���쐬
	void CalcQuaternion();
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
	//���̍X�V����
	void SegmentsUpdate();
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