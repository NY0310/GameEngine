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
#include "LandShape.h"
#include "Stage.h"
#include "Bullet.h"
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
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
		//PARTS_HEAD1,		//	��1
		//PARTS_HEAD2,		//	��2
		//PARTS_HEAD3,		//	��3
		//PARTS_HEAD4,		//	��4
		//PARTS_HEAD5,		//	��5
		//PARTS_HEAD6,		//	��6
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
	////�e�ۗp�̓����蔻����擾����
	std::vector<SphereNode*>& GetCollisionNodeBullet() {

		return m_CollisionNodeBullet;
	}
	// �S�g�p�̓����蔻����擾
	SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//�W�����v
	//void StartJump();
	void StopJump();

	DirectX::SimpleMath::Vector3 GetVelocity() {
		return m_Velocity;
	}

	//hp�擾
	int GetHp() { return m_hp; }
	//hp�ݒ�
	void SetHp(int hp) { m_hp = hp; }

	//�q�b�g�J�E���g�擾
	int Gethitcnt() { return hitcnt; }
	//�q�b�g�J�E���g�ݒ�
	void Sethitcnt(int cnt) { hitcnt = cnt; }

	void ResetCnt() { hitcnt = 0; }


	int GetMAX_HOMING() { return MAX_HOMING;}


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
	std::vector<SphereNode*> m_CollisionNodeBullet;
	// �S�g�p�̓����蔻��
	SphereNode m_CollisionNodeBody;

	//�e
	Obj3d m_Shadow;


	// �e�̃I�t�Z�b�g
	const float SHADOW_OFFSET = 0.08f;
	// ��������e�܂ł̍ő勗��
	const float SHADOW_DISTANCE = 30.0f;
	// �������牽���[�g����܂Ő��������̓�������Ƃ邩
	const float SEGMENT_LENGTH = 1.0f;
	// �������牽���[�g�����܂ŋz�����s����
	const float ADSORP_LENGTH = 0.5f;



	//�e��
	std::vector<Bullet*> m_bullet;

	//�̗͏��
	const int MAX_HP = 100;
	//�̗�
	int m_hp;

	//hp�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_hittexture;

	DirectX::SimpleMath::Vector2 m_origin;



	//�q�b�g�J�E���g
	int hitcnt;
	int MAX_HOMING = 10;
};
