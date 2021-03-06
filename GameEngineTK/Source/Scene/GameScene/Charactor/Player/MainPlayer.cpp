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
}

Player::~Player()
{
}

/// <summary>
/// 子供を生成し登録
/// </summary>
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
	//自身のステート
	playerState = PlayerWalk::GetInstance();

	//エイムの行列
	aimMatrix = make_unique<MatrixObject>();
	aimMatrix->MatrixInitialize();
	aimMatrix->SetPositionY(GetPositionY());
	//カメラに自分を渡す
	FollowCamera* camera = FollowCamera::GetInstance();
	camera->SetPlayer(this);
	SetPositionZ(1);
}


/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	paintGun->SetAimMatirx(aimMatrix);
	playerState->Execute(this);
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







