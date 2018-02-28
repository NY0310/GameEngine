///***************************************************************************
//*|
//*|	�T�v�@�@�����蔻��m�[�h
//*|�@�쐬�ҁ@GS2 16 ���c�N��
//*|�@�쐬���@2017//
//*|___________________________________________________________________________
//****************************************************************************/
//
//#pragma once
//
//#include "Obj3d.h"
//#include "Collision.h"
///***************************************************************************
//*|
//*|	�T�v�@�@�����蔻��m�[�h
//*|�@�쐬�ҁ@GS2 16 ���c�N��
//*|�@�쐬���@2017//
//*|___________________________________________________________________________
//****************************************************************************/
//class CollisionNode
//{
//public:
//	static bool GetDebugVisible() { return m_DebugVisible; }
//	static void SetDebugVisible(bool visible) { m_DebugVisible = visible; }
//protected:
//	//�f�o�b�N�\��ON
//	static bool m_DebugVisible;
//public:
//	//�@����������
//	virtual void Initialize() = 0;
//	//�@�X�V
//	virtual void Update() = 0;
//	//�@�`��
//	virtual void Draw() = 0;
//	//
//	void SetParant(Obj3d* parant);
//
//	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans;}
//protected:
//	//�f�o�b�N�\���p�I�u�W�F�N�g
//	Obj3d m_mbj;
//	//�e����̃I�t�Z�b�g
//	DirectX::SimpleMath::Vector3 m_Trans;
//};
//
//
///***************************************************************************
//*|
//*|	�T�v�@�@�������蔻��m�[�h
//*|�@�쐬�ҁ@GS2 16 ���c�N��
//*|�@�쐬���@2017//
//*|___________________________________________________________________________
//****************************************************************************/
//class SphereNode : public CollisionNode, public Sphere
//{
//public:
//	//�R���X�g���N�^
//	SphereNode();
//	//�@����������
//	void Initialize();
//	//�@�X�V
//	void Update();
//	//�@�`��
//	void Draw();
//	//���[�J�����a�ݒ�
//	void SetLocalRadious(float radius) { m_LocalRadious = radius; }
//protected:
//	//���[�J�����a
//	float m_LocalRadious;
//
//};