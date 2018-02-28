///// <summary>
///// �U�����\�t���̒e
///// </summary>
//#pragma once
//
//#include "Obj3D.h"
//
//class Enemy;
//
//class HomingBullet
//{
//public:
//	// ��s���x<m/frame>
//	static const float SPEED;
//	// 1�t���[���ł̊p�x�ω�����<�x�����W�A��>
//	static const float ANGLE_DELTA_MAX;
//	// ����
//	static const int LIFE_TIME;
//
//	HomingBullet();
//
//	void Initialize();
//
//	bool Update();
//
//	void Draw();
//
//	void Fire(const DirectX::SimpleMath::Vector3 & pos, const DirectX::SimpleMath::Vector3 & direction);
//
//	//���x�擾
//	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_Velocity = velocity; }
//	//���W�擾
//	void SetTarget(Enemy* target) { m_Target = target; }
//	//�Ώێ擾
//	const Enemy* GetEnemy() {return m_Target;}
//protected:
//	// 3D�I�u�W�F�N�g
//	Obj3d m_Obj;
//	// ���x
//	DirectX::SimpleMath::Vector3 m_Velocity;
//	// �Ώ�
//	Enemy* m_Target;
//	// ����
//	int m_LifeTime;
//	//�_���[�W
//	const int DAMAGE = 10;
//};
//
