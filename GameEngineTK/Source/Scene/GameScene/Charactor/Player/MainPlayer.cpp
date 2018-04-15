#include "MainPlayer.h"
#include "State/PlayerWalk.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace NYLibrary;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="maxHp">�̗�</param>
/// <param name="matrixObject">�s��Ǘ�</param>
Player::Player()
{
	int a = 0;
}

Player::~Player()
{
	int a = 0;
}

void Player::CreateAddChild()
{
	paintGun = make_shared<PaintGun>();
	this->AddChild(paintGun);
}

/// <summary>
/// ������(�X�L�����b�V��)
/// </summary>
void Player::Initialize()
{
	//Character::Initialize(MAX_HP);
	SkinMesh::Initialize();
	CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");
	//���g�̃X�e�[�g
	playerState = PlayerWalk::GetInstance();

	//�G�C���̍s��
	aimMatrix = make_unique<MatrixObject>();
	aimMatrix->MatrixInitialize();
	//�J�����Ɏ�����n��
	FollowCamera* camera = FollowCamera::GetInstance();
	camera->SetPlayer(this);
	//�񕜃t���[���J�E���g��������
	recoveryFrameCnt = 0;
	aimMatrix->SetPositionY(GetPositionY());
	SetPositionZ(1);
}


/// <summary>
/// �X�V����
/// </summary>
void Player::Update()
{
	paintGun->SetAimMatirx(aimMatrix);
	auto a = this;

	playerState->Execute(a);
}


/// <summary>
/// �I��
/// </summary>
void Player::Finalize()
{
	aimMatrix.reset();
	PlayerWalk::Dispose();
}

/// <summary>
/// ��Ԃ�ύX����
/// </summary>
/// <param name="state">���</param>
void Player::ChangeState(PlayerState * state)
{
	playerState = state;
}



void Player::Render()
{
	SkinMesh::Render();
}

/// <summary>
/// ��
/// </summary>
void Player::Recovery()
{
	if (RECOVERY_INTERVAL < recoveryFrameCnt)
	{
		Recovery();
		recoveryFrameCnt = 0;
	}
	recoveryFrameCnt++;
}

void Player::OnDied()
{

}
