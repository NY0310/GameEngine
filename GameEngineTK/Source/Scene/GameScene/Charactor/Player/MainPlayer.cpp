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
/// 初期化(スキンメッシュ)
/// </summary>
void Player::Initialize()
{
	//Character::Initialize(MAX_HP);
	SkinMesh::Initialize();
	CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");
	//自身のステート
	playerState = PlayerWalk::GetInstance();

	//エイムの行列
	aimMatrix = make_unique<MatrixObject>();
	aimMatrix->MatrixInitialize();
	//カメラに自分を渡す
	FollowCamera* camera = FollowCamera::GetInstance();
	camera->SetPlayer(this);
	//回復フレームカウントを初期化
	recoveryFrameCnt = 0;
	aimMatrix->SetPositionY(GetPositionY());
	SetPositionZ(1);
}


/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	paintGun->SetAimMatirx(aimMatrix);
	auto a = this;

	playerState->Execute(a);
}


/// <summary>
/// 終了
/// </summary>
void Player::Finalize()
{
	aimMatrix.reset();
	PlayerWalk::Dispose();
}

/// <summary>
/// 状態を変更する
/// </summary>
/// <param name="state">状態</param>
void Player::ChangeState(PlayerState * state)
{
	playerState = state;
}



void Player::Render()
{
	SkinMesh::Render();
}

/// <summary>
/// 回復
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
