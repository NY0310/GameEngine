#include "Light.h"
using namespace NYLibrary;
using namespace std;

unique_ptr<Light> Light::light;//ライト

/// <summary>
/// ライトクラスのインスタンスを取得する
/// </summary>
/// <returns>ライトクラスのインスタンス</returns>
Light * Light::GetInstance()
{
	if (! light.get())
	{
		light.reset(new Light());
	}

	return light.get();
}


/// <summary>
/// コンストラクタ
/// </summary>
Light::Light()
{
	SetPosition(D3DXVECTOR3(0, 0.0f, 0.0f));
	//SetRotationY(D3DXToRadian(90));
	//SetRotationX(90);
	ChangeOrder(WorldMatrixOrder::TRANSMAT_ROTOMAT_SCALEM);
	eyePosition = D3DXVECTOR3(0, 0.0f, 0.0f); //カメラ（視点）位置

}



void Light::Calc()
{
	MatrixObject::Calc();
	D3DXMATRIX rot;
	D3DXMATRIX tra;
	D3DXMatrixTranslation(&tra, eyePosition.x,eyePosition.y, eyePosition.z);
	D3DXMatrixRotationY(&rot, (D3DXToRadian(90)));
	view  = rot * tra;
	//D3DXVECTOR3 vEyePt( 0.0f, 7.0f,-14.0f ); //カメラ（視点）位置
	//D3DXVECTOR3 vLookatPt( 0.0f, 2.0f, 0.0f );//注視位置
	//D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );//上方位置

	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	//D3DXMatrixLookAtLH( &view, &eyePosition, &vLookatPt, &vUpVec );
	//auto a = GetWorldMatrix();
	//D3DXMatrixInverse(&view, nullptr, &view);
	D3DXMatrixInverse(&view, nullptr, &view);
}

//
///// <summary>
///// 更新
///// </summary>
//void Light::Update()
//{
//	D3DXMatrixLookAtRH(&view, &eyePosition, &refPosition, &upVector);
//
//}
