#include "MainPlayer.h"

using namespace std;
using namespace DirectX::SimpleMath;

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

/// <summary>
/// ������(�X�L�����b�V��)
/// </summary>
void Player::Initialize()
{
	Charactor::Initialize();
	animetionSkin = make_unique<SkinMesh>();
	animetionSkin->Initialize();
	animetionSkin->CreateFromX("Resources/X/Hand_animation_2motion_1truck.x");
	matrixObject->SetPosition(D3DXVECTOR3(1, 1, 1));
	matrixObject->SetScale(D3DXVECTOR3(5, 5, 5));
}


/// <summary>
/// �X�V����
/// </summary>
void Player::Update()
{
	matrixObject->Update();
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
