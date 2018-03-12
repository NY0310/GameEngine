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
Player::Player(int maxHp) :
	Character(maxHp)
{
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
	SkinMesh::Initialize();
	CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");
	SetPosition(D3DXVECTOR3(0, 0, 0));
	SetScale(D3DXVECTOR3(10, 10, 10));
	//���g�̃X�e�[�g
	playerState.reset(PlayerWalk::GetInstance());
	FollowCamera* camera = FollowCamera::GetInstance();
	camera->SetPlayer(this);
}


/// <summary>
/// �X�V����
/// </summary>
void Player::Update()
{
	paintGun->SetEmitPosition(GetPosition());

	playerState->Execute(this);
}



void Player::Finalize()
{
}

/// <summary>
/// ��Ԃ�ύX����
/// </summary>
/// <param name="state">���</param>
void Player::ChangeState(PlayerState * state)
{
	playerState.reset(state);
}
