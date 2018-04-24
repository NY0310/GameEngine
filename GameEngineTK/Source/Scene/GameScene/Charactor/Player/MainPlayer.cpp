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
}

Player::~Player()
{
}

/// <summary>
/// �q���𐶐����o�^
/// </summary>
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
	SkinMesh::Initialize();
	CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");
	//���g�̃X�e�[�g
	playerState = PlayerWalk::GetInstance();

	//�G�C���̍s��
	aimMatrix = make_unique<MatrixObject>();
	aimMatrix->MatrixInitialize();
	aimMatrix->SetPositionY(GetPositionY());
	//�J�����Ɏ�����n��
	FollowCamera* camera = FollowCamera::GetInstance();
	camera->SetPlayer(this);
	SetPositionZ(1);
}


/// <summary>
/// �X�V����
/// </summary>
void Player::Update()
{
	paintGun->SetAimMatirx(aimMatrix);
	playerState->Execute(this);
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







