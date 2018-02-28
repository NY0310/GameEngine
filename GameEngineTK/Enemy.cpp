////--------------------------------------------------------------------------------------
//// �t�@�C����: Enemy.cpp
//// �쐬��:
//// �쐬��:
//// ����:
////--------------------------------------------------------------------------------------
//#include "Enemy.h"
//
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//using namespace std;
//
//;
//
//
//Enemy::Enemy(DirectX::Keyboard* keyboard)
//{
//	m_pKeyboard = keyboard;
//
//	Initialize();
//
//
//}
//
//Enemy::~Enemy()
//{
//}
//
////-----------------------------------------------------------------------------
//// ������
////-----------------------------------------------------------------------------
//void Enemy::Initialize()
//{
//
//	//auto& devices = Devices::Get();
//
//
//	////	���@�p�[�c�̓ǂݍ���
//	//m_Obj.resize(PARTS_NUM);
//	//Load();
//	//m_Obj[PARTS_BODY].LoadModel(L"Resources\\Body2.cmo");
//	//m_Obj[PARTS_BREAST].LoadModel(L"Resources\\Body.cmo");
//
//	//m_Obj[PARTS_FING_L].LoadModel(L"Resources\\Fing.cmo");
//	//m_Obj[PARTS_FING_R].LoadModel(L"Resources\\Fing.cmo");
//
//	//for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
//	//{
//	//	m_Obj[i].LoadModel(L"Resources\\head.cmo");
//	//}
//
//
//	//m_Obj[PARTS_LEG_L].LoadModel(L"Resources\\Leg.cmo");
//	//m_Obj[PARTS_LEG_R].LoadModel(L"Resources\\Leg.cmo");
//
//
//
//
//	////	�e�q�֌W�̍\�z(�q�p�[�c�ɐe��ݒ�)
//	//m_Obj[PARTS_FING_L].SetObjParent(&m_Obj[PARTS_BREAST]);
//	//m_Obj[PARTS_FING_R].SetObjParent(&m_Obj[PARTS_BREAST]);
//	//m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_BREAST]);
//	//m_Obj[PARTS_LEG_L].SetObjParent(&m_Obj[PARTS_BODY]);
//	//m_Obj[PARTS_LEG_R].SetObjParent(&m_Obj[PARTS_BODY]);
//
//
//	//for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
//	//{
//	//	m_Obj[i].SetObjParent(&m_Obj[PARTS_BREAST]);
//	//}
//
//
//
//
//
//	//m_Obj[PARTS_BREAST].SetTrans(Vector3(10.0f, 0.0f, 10.0f));
//
//	////	�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̂���)���Z�b�g
//	//m_Obj[PARTS_BODY].SetTrans(Vector3(0, 0.5f, 0));
//	//m_Obj[PARTS_LEG_L].SetTrans(Vector3(-0.5f, -1.7f, 0));
//	//m_Obj[PARTS_LEG_R].SetTrans(Vector3(0.5f, -1.7f, 0.0));
//	//m_Obj[PARTS_FING_L].SetTrans(Vector3(-0.5f, 1.0f, 0));
//	//m_Obj[PARTS_FING_R].SetTrans(Vector3(0.5f, 1.0f, 0.0));
//
//	//m_Obj[PARTS_LEG_L].SetRot(Vector3(-0.5f, 0, 0));
//	//m_Obj[PARTS_LEG_R].SetRot(Vector3(-0.5f, 0, 0));
//
//	//m_Obj[PARTS_FING_L].SetRot(Vector3(0, -0.5f, 0));
//	//m_Obj[PARTS_FING_R].SetRot(Vector3(0, 0.5f, 0));
//
//
//	//for (int i = 0; i < MAX_HEAD; i++)
//	//{
//	//	Start_head_rotate[i] = 60 * i + 1;
//
//	//	m_Obj[PARTS_HEAD1 + i].ChangeOrder(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT);
//	//	m_Obj[PARTS_HEAD1 + i].SetTrans(Vector3(2.0f, 2.0f, 0));
//
//	//	m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i]), 0));
//	//}
//
//
//	//FingRotX = 0.0f;
//	//head_rotate = 0;
//	//FiringCnt = 0;
//	////flag = true;
//	////for (int i = 0; i < MAX_HEAD; i++)
//	////{
//	////	Flag[i] = false;
//	////}
//
//	//m_time = 0;
//
//	//head_reset = false;
//
//	//head_rotate = 1.0;
//
//	////�����ʒu�̐ݒ�
//	//Vector3 pos;
//	///*pos.x = rand() % 10;
//	//pos.z = rand() % 10;*/
//	////pos.x = 1;
//	////pos.z = 1;
//	////pos.y = 4;
//
//	////SetTrans(pos);
//
//	//m_timer = 0;
//	//m_DistAngle = 0;
//
//	//m_cycle = 0;
//
//
//
//
//
//	//{// �S�̗p�̓����蔻��m�[�h�̐ݒ�
//	//	m_CollisionNodeBody.Initialize();
//	//	// �e�p�[�c��ݒ�
//	//	m_CollisionNodeBody.SetParant(&m_Obj[0]);
//	//	m_CollisionNodeBody.SetTrans(Vector3(0, 2, 0));
//	//	m_CollisionNodeBody.SetLocalRadious(2.0f);
//	//}
//
//	//Microsoft::WRL::ComPtr<ID3D11Resource> resTexture;
//	//// �e�N�X�`���̃��[�h
//	//CreateWICTextureFromFile(devices.Device().Get(), L"HP.png", resTexture.GetAddressOf(),
//	//	m_texture.ReleaseAndGetAddressOf());
//
//	//Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
//	//resTexture.As(&texture);
//
//	//// �e�N�X�`���̒��S������o��
//	//CD3D11_TEXTURE2D_DESC textureDesc;
//	//texture->GetDesc(&textureDesc);
//
//	//m_origin.x = float(textureDesc.Width / 2);
//	//m_origin.y = float(textureDesc.Height / 2);
//
//	//m_InScreen = false;
//
//	//m_hp = MAX_HP;
//
//	//MoveFlag = false;
//}
//
//
//void Enemy::Load()
//{
//	wifstream ifs("Player.csv");
//
//	wstring line;
//
//	// 1�s�ڂ��X�L�b�v
//	getline(ifs, line);
//
//	vector<wstring> part_names;
//	vector<wstring> parent_names;
//
//	// ���납��f�[�^�����
//	while (getline(ifs, line)) {
//
//		wstring filepath;
//		wstring filename;
//
//		wchar_t del = L',';
//
//		// �J���}���󔒂ɒu������
//		std::replace(line.begin(), line.end(), L',', L' ');
//
//		// 1�s���𕶎���X�g���[���ɕϊ�
//		wistringstream stream(line);
//
//		// �t�@�C�����̕������ǂݍ���
//		stream >> filename;
//		// �v���W�F�N�g�̊�_����̑��΃p�X�Ńt�@�C������₤
//		filepath = L"Resources/" + filename + L".cmo";
//
//		Obj3d obj;
//		// �ǂݍ���
//		obj.LoadModel(filepath.c_str());
//
//		// �X�P�[�����O�̓ǂݎ��
//		Vector3 scale;
//		stream >> scale.x;
//		stream >> scale.y;
//		stream >> scale.z;
//		obj.SetScale(scale);
//
//		// �p�x�̓ǂݎ��
//		Vector3 rotation;
//		stream >> rotation.z;
//		stream >> rotation.x;
//		stream >> rotation.y;
//		obj.SetRot(rotation);
//
//		// ���W�̓ǂݎ��
//		Vector3 position;
//		stream >> position.x;
//		stream >> position.y;
//		stream >> position.z;
//		obj.SetTrans(position);
//
//		//m_Obj.push_back(obj);
//
//		wstring parent_name;
//
//		stream >> parent_name;
//
//		part_names.push_back(filename);
//
//		parent_names.push_back(parent_name);
//	}
//
//	////// �e�q�֌W�̑g�ݗ���
//	//for (int i = 0; i < m_Obj.size(); i++)
//	//{
//	//	// �e�̎w�肠��
//	//	if (parent_names[i].length() > 0)
//	//	{
//	//		for (int j = 0; j < m_Obj.size(); j++)
//	//		{
//	//			if (j == i) continue;
//
//	//			// �w��̐e����
//	//			if (part_names[j] == parent_names[i])
//	//			{
//	//				m_Obj[i].SetObjParent(&m_Obj[j]);
//	//			}
//	//		}
//	//	}
//	//}
//
//
//}
//
//
///*==================================
//�ړI�̊p�x�ւ̍ŒZ�p�x���擾�i���W�A���j
//����	_angle0	�x�[�X�ƂȂ�p�x
//_angle1	�ڕW�Ƃ���p�x
//�߂�l	�����p�x
//�p�x�O����p�x�P�ɍŒZ�R�[�X�Ō������ۂɉ��Z����p�x���擾����
//===================================*/
//static float GetShortAngleRad(float _angle0, float _angle1)
//{
//	float angle_sub;
//
//	angle_sub = _angle1 - _angle0;	// �p�x�̍�
//									// �����P�W�O�x(�΁j���傫�������ꍇ�A�t��]�̕����߂��̂ŁA�}�C�i�X�ɕϊ�
//									// �ŏI�I��-180�`+180�x�͈̔͂ɁB
//	if (angle_sub > XM_PI)
//	{
//		angle_sub -= XM_2PI;
//	}
//	else if (angle_sub < -XM_PI)
//	{
//		angle_sub += XM_2PI;
//	}
//
//	return angle_sub;
//}
//
//
//
////-----------------------------------------------------------------------------
//// �X�V
////-----------------------------------------------------------------------------
//void Enemy::Update()
//{
//	// �L�[�{�[�h�̏�Ԏ擾
//
//	FiringCnt++;
//	m_timer--;
//	if (m_timer < 0)
//	{
//		m_timer = 60;
//
//		// �ڕW�p�������_���ɒ��I
//		// -0.5�`+0.5�̗���
//		float rnd = (float)rand() / RAND_MAX - 0.5f;
//		// -90�`+90�̗���
//		rnd *= 180.0f;
//		rnd = XMConvertToRadians(rnd);
//
//		m_DistAngle += rnd;
//	}
//
//	{
//		// ���@�̊p�x����]
//		Vector3 rot = GetRot();
//
//		// ���̊p�x�ƖڕW�p�x�̍�
//		float angle = m_DistAngle - rot.y;
//		if (angle > XM_PI)
//		{
//			angle -= XM_2PI;
//		}
//		if (angle < -XM_PI)
//		{
//			angle += XM_2PI;
//		}
//
//		// ���
//		rot.y += angle * 0.01f;
//
//		SetRot(rot);
//	}
//
//
//	
////	if(MoveFlag )
//	{
//		// �ړ��ʂ̃x�N�g��
//		Vector3 moveV(0, 0, -0.1f);
//		// �ړ��x�N�g�������@�̊p�x����]������
//		//moveV = Vector3::TransformNormal(moveV, head_world);
//		float angle = m_Obj[0].GetRot().y;
//		Matrix rotmat = Matrix::CreateRotationY(angle);
//		moveV = Vector3::TransformNormal(moveV, rotmat);
//
//
//
//
//		// ���@�̍��W���ړ�
//		Vector3 pos = m_Obj[0].GetTrans();
//		m_Obj[0].SetTrans(pos + moveV);
//	}
//
//
//	Vector3 a = GetTrans();
//	
//
//	//�L�[�{�[�h�̗l�Ԃ��擾
//	Keyboard::State kb = m_pKeyboard->GetState();
//	//�L�[�{�[�h�g���b�J�[�̍X�V
//	KeybordTracker.Update(kb);
//
//
//	for (int i = 0; i < MAX_HEAD; i++)
//	{
//
//		if (HeadMoveFlag[i] == true)
//		{
//			if (Flag[i] == false)
//			{
//
//
//				Flag[i] = true;
//				//�e����؂藣��
//				FireBullet(PARTS_HEAD1 + i);
//
//				m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(m_Obj[0].GetRot()));
//			}
//			//m_Obj[PARTS_HEAD1 + i].transmat_rotomat_change_flag = true;
//
//
//
//
//			m_Obj[PARTS_HEAD1 + i].SetTrans(
//				Vector3(m_Obj[PARTS_HEAD1 + i].GetTrans().x, m_Obj[PARTS_HEAD1 + i].GetTrans().y, m_Obj[PARTS_HEAD1 + i].GetTrans().z - 1 * MoveVec[i].z));
//
//
//			MoveVec[i].z += 0.5;
//
//		}
//		else
//		{
//			//m_Obj[PARTS_HEAD1 + i].transmat_rotomat_change_flag = false;
//			//m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i] + head_rotate), 0));
//
//			Start_head_rotate[i] += head_rotate;
//		}
//	}
//
//
//	auto& devices = Devices::Get();
//	//// �G�̃��[���h���W�ɑΉ�����X�N���[�����W�𓾂�
//	//Vector2 screenPos;
//	//if (Obj3d::GetCamera()->Project(m_Obj[0].GetTrans() + Vector3(0,3,0), &screenPos))
//	//{
//	//	// �r���[�|�[�g�̎擾
//	//	D3D11_VIEWPORT viewport = devices.Viewport();;
//
//	//	// ��ʂ͈̔͂Ɏ��܂��Ă��邩�H
//	//	if (viewport.TopLeftX <= screenPos.x && screenPos.x <= viewport.TopLeftX + viewport.Width &&
//	//		viewport.TopLeftY <= screenPos.y && screenPos.y <= viewport.TopLeftY + viewport.Height)
//	//	{
//	//		// �X�N���[�����W���L�^
//	//		m_screenPos = screenPos;
//	//		m_InScreen = true;
//	//	}
//	//	else {
//	//		m_InScreen = false;
//	//	}
//	//}
//
//
//	if(FiringCnt % 120 == 0)
//	{
//		BulletDate data;
//		data.Trans = m_Obj[0].GetTrans() + Vector3(0, 1, 0);
//
//		Matrix worldm = m_Obj[0].GetLocalWorld();
//
//		// ���o�����������܂��Ă����ϐ�
//		Vector3 scale;			// ���[���h�X�P�[�����O
//		Quaternion rotation;	// ���[���h��]
//		Vector3 translation;	// ���[���h���W
//
//								// ���[���h�s�񂩂�e�v�f�𒊏o
//		worldm.Decompose(scale, rotation, translation);
//
//
//		//�e�ۃp�[�c�ɑ��x��ݒ�
//		data.Velocity = Vector3(0, 0, -0.5f);
//		//�p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
//		data.Velocity = Vector3::Transform(data.Velocity, rotation);
//
//
//		Bullet* bullet = new Bullet(data);
//
//
//		m_bullet.push_back(bullet);
//
//		{//�e�ۗp�̓����蔻��m�[�h�̐ݒ�
//			SphereNode* spherenode = new SphereNode();
//			spherenode->Initialize();
//			//
//			spherenode->SetParant(&bullet->GetObj3d());
//			//
//			spherenode->SetTrans(Vector3(0.0f, 0.0f, 0));
//			spherenode->SetLocalRadious(0.2f);
//			m_CollisionNodeBullet.push_back(spherenode);
//		}
//
//	}
//
//	for (vector<Bullet*>::iterator it = m_bullet.begin(); it != m_bullet.end(); it++)
//		(*it)->Update();
//
//
//	Calc();
//}
////-----------------------------------------------------------------------------
//// �s��X�V
////-----------------------------------------------------------------------------
//void Enemy::Calc()
//{
//	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
//		it != m_Obj.end();
//		it++)
//	{
//		it->Update();
//	}
//
//	for (std::vector<Bullet*>::iterator it = m_bullet.begin();
//		it != m_bullet.end();
//		it++) {
//		(*it)->Update();
//	}
//
//
//
//	for (std::vector<SphereNode*>::iterator it = m_CollisionNodeBullet.begin();
//		it != m_CollisionNodeBullet.end(); it++)
//	{
//		(*it)->Update();
//	}
//
//
//	m_CollisionNodeBody.Update();
//}
////-----------------------------------------------------------------------------
//// �`��
////-----------------------------------------------------------------------------
//void Enemy::Draw()
//{
//	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
//	{
//		it->Draw();
//	}
//
//	//m_CollisionNodeBody.Draw();
//
//
//
//	auto& devices = Devices::Get();
//
//
//	//for (std::vector<SphereNode*>::iterator it = m_CollisionNodeBullet.begin();
//	//	it != m_CollisionNodeBullet.end(); it++)
//	//{
//	//	(*it)->Draw();
//	//}
//
//
//	//// �A�C�R���`��
//	//if (m_InScreen)
//	//{
//	//	float  a  = static_cast<float>( m_hp) /static_cast<float>( MAX_HP);
//	//	const RECT rect ={0, 0, 170 * a, 30};
//	//	devices.SpriteBatch()->Draw(m_texture.Get(), m_screenPos ,&rect, Colors::White,
//	//		0.0f, m_origin, Vector2(0.2f,0.2f), SpriteEffects_None, 0.0f);
//	//	//devices.SpriteBatch()->Draw(m_texture.Get(), m_screenPos, &rect, Colors::White,
//	//	//	0.0f, m_origin, Vector2(1,1), SpriteEffects_None, 0);
//
//	//}
//
//
//	for (vector<Bullet*>::iterator it = m_bullet.begin(); it != m_bullet.end(); it++)
//	{
//		(*it)->Draw();
//	}
//
//}
//
//const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
//{
//	// �^���N�p�[�c�̍��W��Ԃ�
//	return m_Obj[0].GetTrans();
//}
//void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
//{
//	// �^���N�p�[�c�̍��W��ݒ�
//	m_Obj[0].SetTrans(trans);
//}
//const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
//{
//	// �^���N�p�[�c�̃��[���h�s���Ԃ�
//	return m_Obj[0].GetLocalWorld();
//}
//
////���[�v
//float Enemy::Lerp(float startposition, float gorlpositon, float time)
//{
//	return startposition * (1 - time) + gorlpositon * time;
//}
//
//
//
//
//
//void Enemy::FireBullet(int parts)
//{
//	//�e�q�֌W�������������[���h���W���擾����
//	Matrix worldm = m_Obj[parts].GetLocalWorld();
//	Vector3 scale;
//	Quaternion rotq;
//	Vector3 pos;
//	//�s���1�s�����AVector3�Ƃ��Ĉ���
//	Vector3* m0 = (Vector3*)&worldm.m[0];
//	Vector3* m1 = (Vector3*)&worldm.m[1];
//	Vector3* m2 = (Vector3*)&worldm.m[2];
//	Vector3* m3 = (Vector3*)&worldm.m[3];
//
//	//���[���h���s�񂩂�e�v�f�𒊏o
//	worldm.Decompose(scale, rotq, pos);
//
//	//�e�p�[�c���番���A�Ɨ�������
//	m_Obj[parts].SetObjParent(nullptr);
//	m_Obj[parts].SetScale(scale);
//
//	m_Obj[parts].SetRotQ(rotq);
//
//	m_Obj[parts].SetTrans(pos);
//
//
//	//�e�ۃp�[�c�ɑ��x��ݒ�
//	m_BulletVel = Vector3(0, 0, -0.1f);
//	//�p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
//	m_BulletVel = Vector3::Transform(m_BulletVel, rotq);
//}
