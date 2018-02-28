//#include "HomingBullet.h"
//#include "Enemy.h"
//#include "ModelEffect.h"
//
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//
//// ��s���x<m/frame>
//const float HomingBullet::SPEED = 0.3f;
//// 1�t���[���ł̊p�x�ω�����<�x�����W�A��>
//const float HomingBullet::ANGLE_DELTA_MAX = DirectX::XMConvertToRadians(30.0f);
//// ����
//const int HomingBullet::LIFE_TIME = 120;
//
//HomingBullet::HomingBullet()
//{
//	m_Target = nullptr;
//	m_LifeTime = 0;
//}
//
//void HomingBullet::Initialize()
//{
//	m_Velocity = Vector3(0, 0, -SPEED);
//	m_Obj.LoadModel(L"Resources\\head.cmo");
//}
//
///// <summary>
///// ���t���[���X�V
///// </summary>
//bool HomingBullet::Update()
//{
//	bool death = false;
//
//	if (m_LifeTime > 0)
//	{
//		if (--m_LifeTime <= 0)
//		{
//			death = true;
//		}
//	}
//
//	// �U���Ώۂ������
//	if (!death && m_Target)
//	{
//		// ���݂̐i�s�����x�N�g�������߂�i�P�ɑ��x�x�N�g���𐳋K���������́j
//		Vector3 moveDir;
//		m_Velocity.Normalize(moveDir);
//
//		// ���W�̍����x�N�g�������߂�
//		Vector3 targetDir = m_Target->GetTrans() + Vector3(0,2,0) - m_Obj.GetTrans();
//		float distance = targetDir.Length();
//		if (distance >= 0.2f)
//		{
//			targetDir.Normalize();
//
//			// ���ς̌�������A2�x�N�g���̓��ς��p�x���̃R�T�C���l
//			// A: ���݂̐i�s�����x�N�g��
//			// B: �U���Ώۂ̕����x�N�g��
//			float cosine = moveDir.Dot(targetDir);
//			// 2�x�N�g���p�x��<���W�A��>�����߂�
//			float delta = acosf(cosine);
//			// ���̃t���[���ł̉�]�p<���W�A��>������
//			float angle;
//			if (distance <= 2.0f)
//			{// �������߂��ƗU�����\��������
//				angle = min(delta, ANGLE_DELTA_MAX);
//			}
//			else
//			{
//				// �������Z���ƗU�����\��������
//				float rate = 1.0f - (float)m_LifeTime / LIFE_TIME;
//				angle = min(delta, rate * ANGLE_DELTA_MAX);
//			}
//
//			// A����B�։񂷍ۂ̉�]�����O�ςŋ��߂�
//			Vector3 axis = moveDir.Cross(targetDir);
//			axis.Normalize();
//			// �w�肵����]������Ɏw�胉�W�A�������񂷃N�H�[�^�j�I���𓾂�
//			Quaternion qDelta = Quaternion::CreateFromAxisAngle(axis, angle);
//			// ����̉�]���𔽉f
//			Quaternion q = m_Obj.GetRotQ();
//			q = q * qDelta;
//			m_Obj.SetRotQ(q);
//			// ���x�x�N�g������]
//			m_Velocity = Vector3::Transform(Vector3(0, 0, -SPEED), q);
//		}
//		else
//		{
//		m_Target->SetHp(m_Target->GetHp() - DAMAGE);
//			// �ǐՏI���i�{���Ȃ�Փ˂��Ēe��������j
//			m_Target = nullptr;
//
//			death = true;
//		}
//	}
//
//	if (death)
//	{
//		// �ǐՏI���i�{���Ȃ�Փ˂��Ēe��������j
//		ModelEffectManager::getInstance()->Entry(
//			L"HitEffect.cmo",
//			10,
//			m_Obj.GetTrans(),	// ���W
//			Vector3(0, 0, 0),	// ���x
//			Vector3(0, 0, 0),	// �����x
//			Vector3(0, 0, 0),	// ��]�p�i�����j
//			Vector3(0, 0, 0),	// ��]�p�i�ŏI�j
//			Vector3(0, 0, 0),	// �X�P�[���i�����j
//			Vector3(6, 6, 6)	// �X�P�[���i�ŏI�j
//		);
//
//	
//		return true;
//	}
//
//	// �O�i
//	{
//		// ���x���ړ�����
//		m_Obj.SetTrans(m_Obj.GetTrans() + m_Velocity);
//	}
//
//	// �s��X�V
//	m_Obj.Update();
//
//	return false;
//}
//
///// <summary>
///// �`��
///// </summary>
//void HomingBullet::Draw()
//{
//	// �`��
//	m_Obj.Draw();
//}
//
///// <summary>
///// ����
///// </summary>
///// <param name="direction">�i�s�����x�N�g��</param>
//void HomingBullet::Fire(const DirectX::SimpleMath::Vector3 & pos, const DirectX::SimpleMath::Vector3 & direction)
//{
//	// ����
//	m_LifeTime = LIFE_TIME;
//	// ���x
//	m_Velocity = direction;
//	m_Velocity.Normalize();
//	m_Velocity *= SPEED;
//
//	m_Obj.SetTrans(pos);
//
//	// �����̉�]����i�s��������Z�o����
//	{
//		Vector3 Zminus = -Vector3::UnitZ;
//		// ���ς̌�������A2�x�N�g���̓��ς��p�x���̃R�T�C���l
//		// A: �f�t�H���g�̐i�s�����x�N�g��(Z�}�C�i�X����)
//		// B: �U���Ώۂ̕����x�N�g��
//		float cosine = Zminus.Dot(direction);
//		// 2�x�N�g���p�x��<���W�A��>�����߂�
//		float delta = acosf(cosine);
//
//		// A����B�։񂷍ۂ̉�]�����O�ςŋ��߂�
//		Vector3 axis = Zminus.Cross(direction);
//		axis.Normalize();
//		// �w�肵����]������Ɏw�胉�W�A�������񂷃N�H�[�^�j�I���𓾂�
//		Quaternion q = Quaternion::CreateFromAxisAngle(axis, delta);
//		// ������]���Z�b�g
//		m_Obj.SetRotQ(q);
//	}
//}
