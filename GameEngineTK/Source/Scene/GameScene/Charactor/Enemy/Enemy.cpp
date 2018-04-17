#include "Enemy.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Enemy::Enemy()
	:Target("Resources/OBJMODEL/Enemy.obj")
{
}

/// <summary>
/// ������
/// </summary>
void Enemy::Initialize()
{
	LoadTextuerFile("Resources/BMP/Hand_ColorMap.bmp");
	Target::Initialize();
	SetScale(0.2f);
	SetPositionY(GetLocalSize().y *GetScaleY() / 2);
	SetTag("Enemy");
	frameCnt = 0;
}

/// <summary>
/// �X�V
/// </summary>
void Enemy::Update()
{
	Target::Update();
	Move();
	frameCnt++;
}

/// <summary>
/// ���񂾂Ƃ�
/// </summary>
void Enemy::OnDied()
{
	BreakStart();

}

/// <summary>
/// �ړ�
/// </summary>
void Enemy::Move()
{
	if (MOVE_INTERVAL < frameCnt)
	{
		endPosition = GetMovePosition();
		frameCnt = 0;
	}

}

/// <summary>
/// �ړ���̍��W���擾����
/// </summary>
/// <returns>�ړ���̍��W���擾</returns>
D3DXVECTOR3 Enemy::GetMovePosition()
{
	D3DXVECTOR3 position;
	position.x = RandMovePosition();
	position.y = 0;
	position.z = RandMovePosition();
	return position;
}


/// <summary>
/// ���炷�l�𗐐��ŎZ�o���n��
/// </summary>
/// <returns>���炷�l</returns>
float Enemy::RandMovePosition()
{
	float shift = static_cast<float>(rand() % MAX_MOVE_RADIUS) / 1000;
	shift /= 2;
	if (rand() % 2)
		shift *= -1;
	return shift;
}
