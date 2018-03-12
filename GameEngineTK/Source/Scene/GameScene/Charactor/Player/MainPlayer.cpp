#include "MainPlayer.h"
#include "State/PlayerWalk.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace NYLibrary;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="maxHp">体力</param>
/// <param name="matrixObject">行列管理</param>
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
/// 初期化(スキンメッシュ)
/// </summary>
void Player::Initialize()
{
	SkinMesh::Initialize();
	CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");
	SetPosition(D3DXVECTOR3(0, 0, 0));
	SetScale(D3DXVECTOR3(10, 10, 10));
	//自身のステート
	playerState.reset(PlayerWalk::GetInstance());
	FollowCamera* camera = FollowCamera::GetInstance();
	camera->SetPlayer(this);
}


/// <summary>
/// 更新処理
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
/// 状態を変更する
/// </summary>
/// <param name="state">状態</param>
void Player::ChangeState(PlayerState * state)
{
	playerState.reset(state);
}
