#pragma once
//�w�b�_�[�t�@�C���̃C���N���[�h

#include <d3d11_1.h>
#include <vector> 
#include <memory>
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <cassert>
#include"AStar/Math.h"
#include "Collision.h"
#include "InkObj2.h"
#include "MatrixObject.h"



/// <summary>
/// �C���N��H��
/// </summary>
class InkParticle
{
public:
	//�R���X�g���N�^
	InkParticle();
	//�f�X�g���N�^
	~InkParticle();
	//����(���Ƀ������͊m�ۂ���Ă���)
	void Create(const DirectX::SimpleMath::Vector3& position,const DirectX::SimpleMath::Vector3& nDirection, const DirectX::SimpleMath::Vector4& color, const int index);
	//�X�V����
	void Update();
	//�`�悷�邩
	bool IsValidity() { return isValidity; }
	//�폜(�������͉�����Ȃ�)
	void Destroy() {
		assert(!isValidity, ("���̃C���N�͊��ɖ�����Ԃł�"));
		isValidity = false;}
	//�����蔻��p�̐����擾
	InkSegment* GetSegment() {
		return colisitionSegment.get();
	}
	DirectX::SimpleMath::Matrix& GetWVP() {
		auto a =  matrixObject->GetWVP();
		return a;
	}
	////���W���擾
	//const Vector3 GetPosition() {
	//	return  position;
	//}
	////�傫�����擾
	//const Vector3 GetScale() {
	//	return scale;
	//}
private:
	//���W
	//Vector3 position;
	//�ړ��x�N�g��
	DirectX::SimpleMath::Vector3 direction;
	////�傫��
	//Vector3 scale;
	////��]
	//Vector3 rotation;
	//���x	
	static const float speed;
	//�d��
	static const float gravity;
	//�\�����Ă���̎���
	int birthFrame;
	//������
	bool isValidity;
	//�����蔻��p�̐�
	std::unique_ptr<InkSegment> colisitionSegment;
	//�s��Ǘ�
	std::unique_ptr<MatrixObject> matrixObject;
public:
	// �R�s�[�R���X�g���N�^�֎~
	InkParticle(const InkParticle&) = delete;
	// ����֎~
	InkParticle& operator=(const InkParticle&) = delete;
};

/// <summary>
/// �C���N�̊Ǘ�
/// </summary>
class InkParticleManager
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
	void Shoot(const DirectX::SimpleMath::Vector3& emitPosition, DirectX::SimpleMath::Vector3& nDirection, const DirectX::SimpleMath::Vector4& color);
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
	static const int MAX_PARTICLE = 50;
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
	DirectX::SimpleMath::Vector3 ShiftDirection(DirectX::SimpleMath::Vector3 direction);
	//float����̂��炵���Z�o
	float RandShiftDirection();
private:
	//�����̂��炵�ő��
	static const int MAX_SHIFT_DIRECTION = 3;
	//�C���N�̃p�[�e�B�N��
	InkParticle* inkParticle[MAX_PARTICLE];
	//�C���N�̕`���
	std::unique_ptr<InkObj2> renderer;
	//�C���N���o���Ă���܂��o���܂ł̃C���^�[�o��
	const int INTERVAL_FRAME = 30;
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