#include "Light.h"
using namespace NYLibrary;
using namespace std;

unique_ptr<Light> Light::light;//���C�g

/// <summary>
/// ���C�g�N���X�̃C���X�^���X���擾����
/// </summary>
/// <returns>���C�g�N���X�̃C���X�^���X</returns>
Light * Light::GetInstance()
{
	if (! light.get())
	{
		light.reset(new Light());
	}

	return light.get();
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
Light::Light()
{
	SetPosition(D3DXVECTOR3(0, 0.0f, 0.0f));
	//SetRotationY(D3DXToRadian(90));
	//SetRotationX(90);
	ChangeOrder(WorldMatrixOrder::TRANSMAT_ROTOMAT_SCALEM);
	eyePosition = D3DXVECTOR3(0, 0.0f, 0.0f); //�J�����i���_�j�ʒu

}



void Light::Calc()
{
	MatrixObject::Calc();
	D3DXMATRIX rot;
	D3DXMATRIX tra;
	D3DXMatrixTranslation(&tra, eyePosition.x,eyePosition.y, eyePosition.z);
	D3DXMatrixRotationY(&rot, (D3DXToRadian(90)));
	view  = rot * tra;
	//D3DXVECTOR3 vEyePt( 0.0f, 7.0f,-14.0f ); //�J�����i���_�j�ʒu
	//D3DXVECTOR3 vLookatPt( 0.0f, 2.0f, 0.0f );//�����ʒu
	//D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );//����ʒu

	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	//D3DXMatrixLookAtLH( &view, &eyePosition, &vLookatPt, &vUpVec );
	//auto a = GetWorldMatrix();
	//D3DXMatrixInverse(&view, nullptr, &view);
	D3DXMatrixInverse(&view, nullptr, &view);
}

//
///// <summary>
///// �X�V
///// </summary>
//void Light::Update()
//{
//	D3DXMatrixLookAtRH(&view, &eyePosition, &refPosition, &upVector);
//
//}
