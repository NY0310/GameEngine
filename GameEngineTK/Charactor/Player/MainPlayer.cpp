#include "MainPlayer.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace SceneGraph;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="maxHp">�̗�</param>
/// <param name="matrixObject">�s��Ǘ�</param>
Player::Player(int maxHp):
	Charactor(maxHp)
{
}

///// <summary>
///// ������
///// </summary>
//void Player::Initialize(Vector3 position)
//{
//	Charactor::Initialize(position);
//	Initialize();
//}

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
	Charactor::Initialize();
	animetionSkin = make_unique<SkinMesh>();
	animetionSkin->Initialize();
	animetionSkin->CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");
	matrixObject->SetPosition(D3DXVECTOR3(0, 0, 1));
	matrixObject->SetScale(D3DXVECTOR3(5, 5, 5));
}


/// <summary>
/// �X�V����
/// </summary>
void Player::Update()
{
	paintGun->SetEmitPosition(matrixObject->GetPosition());
	matrixObject->Update();
	FollowCamera* camera = FollowCamera::GetInstance();
	camera->SetTarGetTrans(matrixObject->GetPosition());
	camera->SetTargetAngle(matrixObject->GetRotation().y);


}

/// <summary>
/// 
/// </summary>
void Player::Render()
{
	animetionSkin->Render(matrixObject.get());
}

void Player::Finalize()
{
	animetionSkin.release();
}
