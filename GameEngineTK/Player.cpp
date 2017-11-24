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
	auto& devices = Devices::Get();


	//	���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);

	m_Obj[PARTS_BODY].LoadModel(L"Resources\\Body2.cmo");
	m_Obj[PARTS_BREAST].LoadModel(L"Resources\\Body.cmo");

	m_Obj[PARTS_FING_L].LoadModel(L"Resources\\Fing.cmo");
	m_Obj[PARTS_FING_R].LoadModel(L"Resources\\Fing.cmo");

	//for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	//{
	//	m_Obj[i].LoadModel(L"Resources\\head.cmo");
	//}


	m_Obj[PARTS_LEG_L].LoadModel(L"Resources\\Leg.cmo");
	m_Obj[PARTS_LEG_R].LoadModel(L"Resources\\Leg.cmo");

	//	�e�q�֌W�̍\�z(�q�p�[�c�ɐe��ݒ�)
	m_Obj[PARTS_FING_L].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_FING_R].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_LEG_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_LEG_R].SetObjParent(&m_Obj[PARTS_BODY]);


	//for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	//{
	//	m_Obj[i].SetObjParent(&m_Obj[PARTS_BREAST]);
	//}


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


	//for (int i = 0; i < MAX_HEAD; i++)
	//{
	//	Start_head_rotate[i] = 60 * i + 1;

	//	m_Obj[PARTS_HEAD1 + i].ChangeOrder(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT);

	//	m_Obj[PARTS_HEAD1 + i].SetTrans(Vector3(2.0f, 2.0f, 0));

	//	m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i]), 0));
	//}


	FingRotX = 0.0f;
	head_rotate = 0;
	FiringCnt = 0;
	flag = true;
	//for (int i = 0; i < MAX_HEAD; i++)
	//{
	//	Flag[i] = false;
	//}

	m_time = 0;

	head_reset = false;

	head_rotate = 1.0;

	hitcnt = 0;


	Microsoft::WRL::ComPtr<ID3D11Resource> resTexture;
	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(devices.Device().Get(), L"HP.png", resTexture.GetAddressOf(),
		m_texture.ReleaseAndGetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	resTexture.As(&texture);

	// �e�N�X�`���̒��S������o��
	CD3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	m_origin.x = float(textureDesc.Width / 2);
	m_origin.y = float(textureDesc.Height / 2);



	Microsoft::WRL::ComPtr<ID3D11Resource> hitresTexture;
	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(devices.Device().Get(), L"homing.png", hitresTexture.GetAddressOf(),
		m_hittexture.ReleaseAndGetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> hittexture;
	hitresTexture.As(&hittexture);





	{// �S�̗p�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBody.Initialize();
		// �e�p�[�c��ݒ�
		m_CollisionNodeBody.SetParant(&m_Obj[0]);
		m_CollisionNodeBody.SetTrans(Vector3(0, 2, 0));
		m_CollisionNodeBody.SetLocalRadious(2.0f);
	}


	m_Shadow.LoadModel(L"shadow.cmo");
	m_Shadow.SetTrans(Vector3(0, SHADOW_OFFSET, 0));

	m_isJump = false;


	m_hp = MAX_HP;

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
		// StartJump();
	}
	Vector3 inter;

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

		// �e�̈ʒu�X�V
		Vector3 shadow_pos = m_Obj[0].GetTrans();
		shadow_pos.y = inter.y + SHADOW_OFFSET;
		m_Shadow.SetTrans(shadow_pos);
	}


	// �e�̍X�V
	{
		Vector3 pos = m_Obj[0].GetTrans();
		pos.y = SHADOW_OFFSET;
		m_Shadow.SetTrans(pos);
	}

	// ���������n�`�����蔻��
	{
		const Vector3& vel = GetVelocity();

		bool hit = false;
		Segment player_segment;
		Vector3 player_pos = GetTrans();
		player_segment.Start = player_pos + Vector3(0, SEGMENT_LENGTH, 0);
		// �e�̈ʒu�����o���邽�߁A�������牺�����ɗ]�T�������Ĕ�������
		player_segment.End = player_pos + Vector3(0, -SHADOW_DISTANCE, 0);

		// �傫�������ŏ�����
		float distance = 1.0e5;



		float temp_distance;
		Vector3 temp_inter;


		Stage stage;
		// ���ʂƂ̓�����𔻒�
		if ((&stage.GetLandShape())->IntersectSegment(player_segment, &temp_inter))
		{
			hit = true;
			temp_distance = Vector3::Distance(player_segment.Start, temp_inter);
			if (temp_distance < distance)
			{
				inter = temp_inter;
				distance = temp_distance;
			}
		}

	}



	{//���x�ɂ��ړ�
		Vector3 trans = GetTrans();
		trans += m_Velocity;
		SetTrans(trans);

	}


	if (KeybordTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		BulletDate data;
		data.Trans = m_Obj[0].GetTrans() + Vector3(0, 1, 0);

		Matrix worldm = m_Obj[0].GetLocalWorld();

		// ���o�����������܂��Ă����ϐ�
		Vector3 scale;			// ���[���h�X�P�[�����O
		Quaternion rotation;	// ���[���h��]
		Vector3 translation;	// ���[���h���W

								// ���[���h�s�񂩂�e�v�f�𒊏o
		worldm.Decompose(scale, rotation, translation);


		//�e�ۃp�[�c�ɑ��x��ݒ�
		data.Velocity = Vector3(0, 0, -0.5f);
		//�p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
		data.Velocity = Vector3::Transform(data.Velocity, rotation);


		Bullet* bullet = new Bullet(data);


		m_bullet.push_back(bullet);

		{//�e�ۗp�̓����蔻��m�[�h�̐ݒ�
			SphereNode* spherenode = new SphereNode();
			spherenode->Initialize();
			//
			spherenode->SetParant(&bullet->GetObj3d());
			//
			spherenode->SetTrans(Vector3(0.0f, 0.0f, 0));
			spherenode->SetLocalRadious(0.2f);
			m_CollisionNodeBullet.push_back(spherenode);
		}

	}



	for (vector<Bullet*>::iterator it = m_bullet.begin(); it != m_bullet.end(); it++)
	{
		(*it)->Update();

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
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Update();
	}

	for (std::vector<Bullet*>::iterator it = m_bullet.begin();
		it != m_bullet.end();
		it++) {
		(*it)->Update();
	}


	for (std::vector<SphereNode*>::iterator Sphereit = m_CollisionNodeBullet.begin(); Sphereit != m_CollisionNodeBullet.end(); Sphereit++)

	{
		(*Sphereit)->Update();
		(*Sphereit)->Draw();
	}

	// �e�̍X�V
	m_Shadow.Update();


}
//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Player::Draw()
{
	auto& devices = Devices::Get();


	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Draw();
	}


	// �e�����Z�`��
	m_Shadow.DrawSubtractive();


	for (vector<Bullet*>::iterator it = m_bullet.begin(); it != m_bullet.end(); it++)
	{
		(*it)->Draw();
	}


	float  a = static_cast<float>(m_hp) / static_cast<float>(MAX_HP);
	const RECT rect = { 0, 0, 170 * a, 30 };
	devices.SpriteBatch()->Draw(m_texture.Get(), Vector2(410, 560), &rect, Colors::White,
		0.0f, m_origin, Vector2(1.0f, 1.0f), SpriteEffects_None, 0.0f);


	float  b = static_cast<float>(hitcnt) / static_cast<float>(MAX_HOMING);
	const RECT homingrect = { 0, 0, 170 * b, 30 };
	Color color;
	if (hitcnt == MAX_HOMING)
	{
		color = Colors::Red;
	}
	else
	{
		color = Colors::White;
	}
	devices.SpriteBatch()->Draw(m_hittexture.Get(), Vector2(100, 440), &homingrect, color,
		-1.6f, m_origin, Vector2(1.0f, 1.0f), SpriteEffects_None, 0.0f);



	m_CollisionNodeBody.Update();
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
	////�s���1�s�����AVector3�Ƃ��Ĉ���
	//Vector3* m0 = (Vector3*)&worldm.m[0];
	//Vector3* m1 = (Vector3*)&worldm.m[1];
	//Vector3* m2 = (Vector3*)&worldm.m[2];
	//Vector3* m3 = (Vector3*)&worldm.m[3];

	//���[���h���s�񂩂�e�v�f�𒊏o
	worldm.Decompose(scale, rotq, pos);

	//�e�p�[�c���番���A�Ɨ�������
	m_Obj[parts].SetObjParent(nullptr);
	m_Obj[parts].SetScale(scale);

	m_Obj[parts].SetRotQ(rotq);

	m_Obj[parts].SetTrans(pos);


	//�e�ۃp�[�c�ɑ��x��ݒ�
	m_BulletVel[parts] = Vector3(0, 0, -0.1f);
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


//���[�v4
float Player::Lerp(float startposition, float gorlpositon, float time)
{
	return startposition * (1 - time) + gorlpositon * time;
}