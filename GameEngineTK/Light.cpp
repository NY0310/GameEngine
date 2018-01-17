#include "Light.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="vectol">視点</param>
Light::Light(D3DXVECTOR3 eyeposition):
	eyePosition(eyeposition)
{
	refPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	upVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtRH(&view, &eyePosition, &refPosition, &upVector);
}
