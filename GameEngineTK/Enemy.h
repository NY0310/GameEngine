/**
* @file Enemy.h
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
#include <WICTextureLoader.h>

#include <vector>
#include "CollisionNode.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <SpriteBatch.h>
#include "Bullet.h"

// ���@
class Enemy
{
public:

	//��]�p���擾
	const  DirectX::SimpleMath::Vector3& GetRot() {
		// �^���N�p�[�c�̉�]��Ԃ�
		return  m_Obj[0].GetRot();
	};
	void SetRot(const DirectX::SimpleMath::Vector3& rot)
		{
			// �^���N�p�[�c�̍��W��ݒ�
			m_Obj[0].SetRot(rot);
		}
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
	Enemy(DirectX::Keyboard* keyboard);
	// �f�X�g���N�^
	~Enemy();
	// ������
	void Initialize();
	void Load();
	// ���t���[���X�V
	void Update();
	// �s��̌v�Z
	void Calc();
	// �`��
	void Draw();
	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetTrans();
	// ���W��ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// ���[���h�s����擾
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	void FireBullet(int parts);

	// �S�g�p�̓����蔻����擾
	SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }


	//�摜�擾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() {
		return m_texture;
	}



	bool GetInScreen() { return m_InScreen; }
	DirectX::SimpleMath::Vector2 GetOrigin() { return m_origin; }


	//hp�擾
	int GetHp() { return m_hp; }
	//hp�ݒ�
	void SetHp(int hp) { m_hp = hp; }


	DirectX::SimpleMath::Vector3 GetBulletVel() { return m_BulletVel; }

	bool SetMoveFlag(bool  flag) { return MoveFlag = flag; }



	const Obj3d* GetObj3d() { return &m_Obj[0]; }


	std::vector<SphereNode*>& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }


protected:
	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeyboard;
	// �����o�ϐ�
	// ���{�b�g�̍\���p�[�c
	std::vector<Obj3d> m_Obj;
	// ���x
	DirectX::SimpleMath::Vector3 m_Velocity;
	// �W�����v�i�����j�����H
	bool m_isJump;


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


	//�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker KeybordTracker;

	//	���@�̃��[���h�s��1
	DirectX::SimpleMath::Matrix head_world;

	//�喃
	int m_timer;
	//�ڕW�p�x
	float m_DistAngle;

	//��]
	float m_cycle;

	// �S�g�p�̓����蔻��
	SphereNode m_CollisionNodeBody;



	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//�X�N���[���Ɏ��܂��Ă��邩
	bool m_InScreen;
	//�X�N���[�����W
	DirectX::SimpleMath::Vector2 m_screenPos;


	//�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_texture;

	DirectX::SimpleMath::Vector2 m_origin;

	//�̗͏��
	const int MAX_HP = 30;
	//�̗�
	int m_hp;

	//�ړ����邩
	bool MoveFlag;

	//�e��
	std::vector<Bullet*> m_bullet;
	//�e�ۗp�̓����蔻��
	std::vector<SphereNode*> m_CollisionNodeBullet;
};
