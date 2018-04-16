#include "PaintGun.h"

using namespace std;
PaintGun::PaintGun()
{

}

/// <summary>
/// �I�u�W�F�N�g�𐶐����q���Ƃ��ēo�^
/// </summary>
void PaintGun::CreateAddChild()
{
	aim = make_shared<Aim>();
	AddChild(aim);
	inkParticleManager = make_shared<InkParticleManager>();
	AddChild(inkParticleManager);
	inkTank = make_shared<InkTank>();
	AddChild(inkTank);
}

/// <summary>
/// ����������
/// </summary>
void PaintGun::Initialize()
{
	auto& devices = Devices::Get();
}

/// <summary>
/// �X�V����
/// </summary>
void PaintGun::Update()
{
	Emit();
}

/// <summary>
/// �`�揈��
/// </summary>
void PaintGun::Render()
{
}

/// <summary>
/// �C���N�̔���
/// </summary>
void PaintGun::Emit()
{
	MouseUtil* mouse = MouseUtil::GetInstance();

	if (mouse->IsPressed(MouseUtil::Left)&& !inkTank->IsEmptyInk())
	{
		//�e�̏����擾(�v���C���[)
		MatrixObject* player = Getparent()._Get();

		D3DXVECTOR3 emitPosition = aimMatirx->GetPosition();
		//�ړ��x�N�g��
		D3DXVECTOR3 moveV(0,0,-1);

		//�ړ��x�N�g�������@�̊p�x����]������
		D3DXVec3TransformNormal(&moveV, &moveV,&aimMatirx->GetRotationMatrix());

		inkParticleManager->Shoot(emitPosition, moveV, inkTank->GetInkColor());
		inkTank->InkLost();
	}
}



void PaintGun::Finalize()
{}

