//--------------------------------------------------------------------------------------
// �t�@�C����: Player.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

;


Player::Player(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	Initialize();


}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Player::Initialize()
{
	//	���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);

	m_Obj[PARTS_BODY].LoadModel(L"Resources\\Body2.cmo");
	m_Obj[PARTS_BREAST].LoadModel(L"Resources\\Body.cmo");

	m_Obj[PARTS_FING_L].LoadModel(L"Resources\\Fing.cmo");
	m_Obj[PARTS_FING_R].LoadModel(L"Resources\\Fing.cmo");

	for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	{
		m_Obj[i].LoadModel(L"Resources\\head.cmo");
	}


	m_Obj[PARTS_LEG_L].LoadModel(L"Resources\\Leg.cmo");
	m_Obj[PARTS_LEG_R].LoadModel(L"Resources\\Leg.cmo");

	//	�e�q�֌W�̍\�z(�q�p�[�c�ɐe��ݒ�)
	m_Obj[PARTS_FING_L].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_FING_R].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_LEG_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_LEG_R].SetObjParent(&m_Obj[PARTS_BODY]);


	for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	{
		m_Obj[i].SetObjParent(&m_Obj[PARTS_BREAST]);
	}


	m_Obj[PARTS_BREAST].SetTrans(Vector3(0.0f, 0.0f, 0.0f));

	//	�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̂���)���Z�b�g
	m_Obj[PARTS_BODY].SetTrans(Vector3(0, 0.5f, 0));
	m_Obj[PARTS_LEG_L].SetTrans(Vector3(-0.5f, -1.7f, 0));
	m_Obj[PARTS_LEG_R].SetTrans(Vector3(0.5f, -1.7f, 0.0));
	m_Obj[PARTS_FING_L].SetTrans(Vector3(-0.5f, 1.0f, 0));
	m_Obj[PARTS_FING_R].SetTrans(Vector3(0.5f, 1.0f, 0.0));

	m_Obj[PARTS_LEG_L].SetRot(Vector3(-90.0f, 0, 0));
	m_Obj[PARTS_LEG_R].SetRot(Vector3(-90.0f, 0, 0));

	m_Obj[PARTS_FING_L].SetRot(Vector3(0, -90.0f, 0));
	m_Obj[PARTS_FING_R].SetRot(Vector3(0, 90.0f, 0));


	for (int i = 0; i < MAX_HEAD; i++)
	{
		Start_head_rotate[i] = 60 * i + 1;

		m_Obj[PARTS_HEAD1 + i].ChangeOrder(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT);

		m_Obj[PARTS_HEAD1 + i].SetTrans(Vector3(2.0f, 2.0f, 0));

		m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i]), 0));
	}


	FingRotX = 0.0f;
	head_rotate = 0;
	FiringCnt = 0;
	flag = true;
	for (int i = 0; i < MAX_HEAD; i++)
	{
		Flag[i] = false;
	}

	m_time = 0;

	head_reset = false;

	head_rotate = 1.0;

	for (int i = 0; i < MAX_HEAD; i++)
	{//�e�ۗp�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBullet[ i].Initialize();
		//
		m_CollisionNodeBullet[i].SetParant(&m_Obj[PARTS_HEAD1 + i]);
		//
		m_CollisionNodeBullet[ i].SetTrans(Vector3(0.0f, 0.5f, 0));
		m_CollisionNodeBullet[ i].SetLocalRadious(0.5f);
	}





	{// �S�̗p�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBody.Initialize();
		// �e�p�[�c��ݒ�
		m_CollisionNodeBody.SetParant(&m_Obj[0]);
		m_CollisionNodeBody.SetTrans(Vector3(0, 2, 0));
		m_CollisionNodeBody.SetLocalRadious(2.0f);
	}

	m_isJump = false;
}
//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();


  
	//m_r++;
	//float rot = XMConvertToRadians(m_r);

	//m_Scale = Matrix::CreateScale(sinf(rot) + 1.0f);
	//
	if (KeybordTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		//�W�����v�J�n
 		StartJump();
	}

	//��������
	if (m_isJump)
	{
		//�d�͂ɂ�����
		m_Velocity.y -= GRAVITY_ACC;
		//�d�͑��x�̌��E����
		if (m_Velocity.y < -JUMP_SPEED_MAX)
		{
			m_Velocity.y = -JUMP_SPEED_MAX;
		}
	}


	{//���x�ɂ��ړ�
		Vector3 trans = GetTrans();
		trans += m_Velocity;
		SetTrans(trans);

	}



	if (KeybordTracker.IsKeyPressed(Keyboard::Keys::Space))
		if(FiringCnt != 7)
	{
		HeadMoveFlag[FiringCnt] = true;
		FiringCnt++;
	}


	if (keystate.Space && (FiringCnt == 7))
	{
		for (int i = 0; i < MAX_HEAD; i++)
		{
			HeadMoveFlag[i] = false;
			Start_head_rotate[i] = 60 * i;
			FiringCnt = 0;
			Flag[i] = false;
			MoveVec[i] = Vector3(0.0f,0.0f,0.0f);
			//�e�q�֌W���č\�z
			for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
			{
				m_Obj[i].SetObjParent(&m_Obj[PARTS_BREAST]);
			}

			const_cast<unique_ptr<WorldMatrixOrder>&>(m_Obj[PARTS_HEAD1 + i].GetOrder()).reset();
			m_Obj[PARTS_HEAD1 + i].SetOrder((m_Obj[PARTS_HEAD1 + i].GetWorldMatrixOrderFactory().Set(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT)));

			m_Obj[PARTS_HEAD1 + i].SetTrans(Vector3(2, 2, 0));

			m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i]), 0));


		}
	}




	//	�����񏈗�
	if (keystate.D)
	{
		//	���@�̊p�x����]
		//head_rot += -0.03f;
		float angle = m_Obj[0].GetRot().y;
		m_Obj[0].SetRot(Vector3(0, angle - 0.03f, 0));
	}

	//	�����񏈗�
	if (keystate.A)
	{
		//	���@�̊p�x����]
		//head_rot += 0.03f;
		float angle = m_Obj[0].GetRot().y;
		m_Obj[0].SetRot(Vector3(0, angle + 0.03f, 0));
	}


	//	�O�i����
	if (keystate.W)
	{
		//	�ړ��x�N�g��
		Vector3 moveV(0.0f, 0.0f, -0.2f);
		//	�ړ��x�N�g�������@�̊p�x����]������
		moveV = Vector3::TransformNormal(moveV, head_world);

		float angle = m_Obj[0].GetRot().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		//	���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTrans();
		//head_pos += moveV;
		m_Obj[0].SetTrans(pos + moveV);

		//m_Obj[0].SetRot(Vector3(-0.5, m_Obj[PARTS_BREAST].GetRot().y, m_Obj[PARTS_BREAST].GetRot().z));

	}
	/*else
	{
		m_Obj[PARTS_BREAST].SetRot(
			Vector3(0,
				m_Obj[PARTS_BREAST].GetRot().y,
				m_Obj[PARTS_BREAST].GetRot().z)
		);
	}*/


	//�L�[�{�[�h�̗l�Ԃ��擾
	Keyboard::State kb = m_pKeyboard->GetState();
	//�L�[�{�[�h�g���b�J�[�̍X�V
	KeybordTracker.Update(kb);





	for (int i = 0; i < MAX_HEAD; i++)
	{

		//if (HeadMoveFlag[i] == true)
		//{
		//	if (Flag[i] == false)
		//	{

		//		
		//		Flag[i] = true;
		//		//�e����؂藣��
		//		FireBullet(PARTS_HEAD1 + i);

		//		m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(m_Obj[0].GetRot()));
		//	

		//	}
		//	
		//	const_cast<unique_ptr<WorldMatrixOrder>&>(m_Obj[PARTS_HEAD1 + i].GetOrder()).reset();
		//	m_Obj[PARTS_HEAD1 + i].SetOrder((m_Obj[PARTS_HEAD1 + i].GetWorldMatrixOrderFactory().Set(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT)));

		//	//�e�ۃp�[�c�̍��W���ړ�
		//	Vector3 pos = m_Obj[PARTS_HEAD1 + i].GetTrans();
		//	m_Obj[PARTS_HEAD1 + i].SetTrans(pos + m_BulletVel[PARTS_HEAD1 + i]);
		//	
		//	 
		//	m_Obj[PARTS_HEAD1 + i].SetTrans(
		//		Vector3(m_Obj[PARTS_HEAD1 + i].GetTrans().x, m_Obj[PARTS_HEAD1 + i].GetTrans().y, m_Obj[PARTS_HEAD1 + i].GetTrans().z -1 * MoveVec[i].z));

		//	
		//	MoveVec[i].z += 0.5;
		//
		//}
		//else
		{
			m_Obj[PARTS_HEAD1 + i].ChangeOrder(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT);
			m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i] + head_rotate), 0));

			Start_head_rotate[i] += head_rotate;
		}
	}

	//	��ޏ���
	if (keystate.S)
	{
		//	�ړ��x�N�g��
		Vector3 moveV(0.0f, 0.0f, 0.2f);
		//	�ړ��x�N�g�������@�̊p�x����]������
		moveV = Vector3::TransformNormal(moveV, head_world);

		float angle = m_Obj[0].GetRot().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		//	���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTrans();
		//head_pos += moveV;
		m_Obj[0].SetTrans(pos + moveV);

		
	}





	

	//for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	//{
	//	it->Update();
	//}

	Vector3 a = GetTrans();

	head_rotate = 1.0;

	Calc();
}
//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Player::Calc()
{
	// �S�p�[�c���s��X�V
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();it != m_Obj.end();it++)
	{
		it->Update();
	}

	for (int i = 0; i < MAX_HEAD; i++)
	{
		m_CollisionNodeBullet[i].Update();
	}


	m_CollisionNodeBody.Update();
}
//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Player::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Draw();
	}
	for (int i = 0; i < MAX_HEAD; i++)
	{
		m_CollisionNodeBullet[i].Draw();
	}
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].GetTrans();
}
void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetTrans(trans);
}
const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[0].GetLocalWorld();
}

void Player::FireBullet(int parts)
{
	//�e�q�֌W�������������[���h���W���擾����
	Matrix worldm = m_Obj[parts].GetLocalWorld();
	Vector3 scale;
	Quaternion rotq;
	Vector3 pos;
	//�s���1�s�����AVector3�Ƃ��Ĉ���
	Vector3* m0 = (Vector3*)&worldm.m[0];
	Vector3* m1 = (Vector3*)&worldm.m[1];
	Vector3* m2 = (Vector3*)&worldm.m[2];
	Vector3* m3 = (Vector3*)&worldm.m[3];

	//���[���h���s�񂩂�e�v�f�𒊏o
	worldm.Decompose(scale, rotq, pos);

	//�e�p�[�c���番���A�Ɨ�������
	m_Obj[parts].SetObjParent(nullptr);
	m_Obj[parts].SetScale(scale);

	m_Obj[parts].SetRotQ(rotq);

	m_Obj[parts].SetTrans(pos);

	
	//�e�ۃp�[�c�ɑ��x��ݒ�
	m_BulletVel[parts]= Vector3(0, 0, -0.1f);
	//�p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
	m_BulletVel[parts] = Vector3::Transform(m_BulletVel[parts], rotq);
}



/// <summary>
/// �W�����v���J�n
/// </summary>
void Player::StartJump()
{
	if (!m_isJump)
	{
		// ������̏�����ݒ�
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

//�����J�n
void Player::StartFall()
{
	if (!m_isJump)
	{
		//������̏�����ݒ�
		m_Velocity.y = 0;
		m_isJump = true;
	}
}

void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}

//�������I��


//���[�v
float Player::Lerp(float startposition, float gorlpositon, float time)
{
	return startposition * (1 - time) + gorlpositon * time;
}