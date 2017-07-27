/**
* @file Player.h
*
* @brief ���@
*
* @date 2015/06/06
*
* @author ���c�@�N��
*/
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include "Obj3d.h"
#include <vector>
#include "CollisionNode.h"

// ���@
class Player
{
public:
	//�d�͉����x
	const float GRAVITY_ACC = 0.03f;
	//�W�����v����
	const float JUMP_SPEED_FIRST = 0.5f;
	//
	const float JUMP_SPEED_MAX = 0.3f;
	//	���@�p�[�c
	enum PARTS
	{
		PARTS_BREAST,	//�@��
		PARTS_BODY,		//	��
		PARTS_FING_L,	//  ����
		PARTS_FING_R,	//  �E��
		PARTS_HEAD1,		//	��1
		PARTS_HEAD2,		//	��2
		PARTS_HEAD3,		//	��3
		PARTS_HEAD4,		//	��4
		PARTS_HEAD5,		//	��5
		PARTS_HEAD6,		//	��6
		PARTS_LEG_L,		//	����
		PARTS_LEG_R,		//	�E��

		PARTS_NUM
	};
	// �R���X�g���N�^
	Player(DirectX::Keyboard* keyboard);
	// �f�X�g���N�^
	~Player();
	// ������
	void Initialize();
	// ���t���[���X�V
	void Update();
	// �s��̌v�Z
	void Calc();
	// �`��
	void Draw();
	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetTrans();
	//��]�p���擾
	const  DirectX::SimpleMath::Vector3& GetRot() {
		// �^���N�p�[�c�̉�]��Ԃ�
		return  m_Obj[0].GetRot();
	};
	// ���W��ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// ���[���h�s����擾
	const DirectX::SimpleMath::Matrix& GetLocalWorld();

	void FireBullet(int parts);
	void StartJump();
	void StartFall();
	//�e�ۗp�̓����蔻����擾����
	const SphereNode& GetCollisionNodeBullet() {
		for (int i = 0; i < MAX_HEAD; i++) {
			return  m_CollisionNodeBullet[PARTS_HEAD1 + i];
		}
	}
	// �S�g�p�̓����蔻����擾
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//�W�����v
	//void StartJump();
	void StopJump();

	DirectX::SimpleMath::Vector3 GetVelocity() {
		return m_Velocity;
	}
protected:
	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeyboard;

	////�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker KeybordTracker;

	// �����o�ϐ�
	// ���{�b�g�̍\���p�[�c
	std::vector<Obj3d> m_Obj;
	// ���x
	DirectX::SimpleMath::Vector3 m_Velocity;
	// �W�����v�i�����j�����H
	bool m_isJump;

	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel[6];


	bool head_reset;


	float FingRotX;

	bool flag;


	bool jumpflag;


	int m_time;

	//���̐�
	const int MAX_HEAD = 6;

	//���̊p�x
	float head_rotate;

	float Start_head_rotate[6];


	float Lerp(float startposition, float gorlpositon, float time);



	int FiringCnt;

	bool HeadMoveFlag[6];

	bool Flag[6];
	DirectX::SimpleMath::Vector3 MoveVec[6];



	//	���@�̃��[���h�s��1
	DirectX::SimpleMath::Matrix head_world;

	//�e�ۗp�̓����蔻��
	SphereNode m_CollisionNodeBullet[8];
	// �S�g�p�̓����蔻��
	SphereNode m_CollisionNodeBody;


};
