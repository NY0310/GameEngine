#include "MouseRay.h"
#include "../Math/Math.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;
using namespace NYLibrary;

std::unique_ptr<MouseRay> MouseRay::mouseRay;


MouseRay::MouseRay()
{
}

/// <summary>
/// �C���X�^���X���擾
/// </summary>
/// <returns>�}�E�X���C�̃A�h���X</returns>
MouseRay * MouseRay::GetInstance()
{
	if (!mouseRay)
		mouseRay.reset(new MouseRay);
	return  mouseRay.get();
}

/// <summary>
/// �}�E�X���烌�C���쐬
/// </summary>
Segment* MouseRay::RayCreate()
{
	//FollowCamera* camera = FollowCamera::GetInstance();
	//MouseUtil* mouse = MouseUtil::GetInstance();
	//Vector3 pos;
	//int x = mouse->GetPos().x;
	//int y = mouse->GetPos().y;
	//Matrix view = Math::D3DXMATRIXToMatrix(camera->GetView());
	//Matrix proj = Math::D3DXMATRIXToMatrix(camera->GetProjection());
	//CalcScreenToXZ(&pos, x, y, Devices::Get().Width(), Devices::Get().Height(), &view, &proj);
	//Segment* segment = new Segment();
	//segment->End = Math::D3DXVECTOR3ToVector(camera->GetEyePos()) + Vector3(0,0,0);
	//segment->Start = pos;
	//return segment;
	return nullptr;
}


// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
Vector3* MouseRay::CalcScreenToXZ(
	Vector3* pout,
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	Matrix* View,
	Matrix* Prj
) {
	Vector3 nearpos;
	Vector3 farpos;
	Vector3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	ray.Normalize();

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0) {
		// ����_
		float Lray = ray.Dot(Vector3(0, 1, 0));
		float LP0 = (-nearpos).Dot(Vector3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}

	return pout;
}








// �X�N���[�����W�����[���h���W�ɕϊ�
Vector3* MouseRay::CalcScreenToWorld(
	Vector3* pout,
	float Sx,  // �X�N���[��X���W
	float Sy,  // �X�N���[��Y���W
	float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
	float Screen_w,
	float Screen_h,
	Matrix* View,
	Matrix* Prj
) {
	// �e�s��̋t�s����Z�o
	Matrix InvView, InvPrj, VP, InvViewport;
	//D3DXMatrixInverse(&InvView, NULL, View);
	//	���̑���
	XMMATRIX XMview = XMLoadFloat4x4(View);
	InvView = XMMatrixInverse(nullptr, XMview);

	//D3DXMatrixInverse(&InvPrj, nullptr, Prj);
	//	���̑���
	XMMATRIX XMprj = XMLoadFloat4x4(Prj);
	InvPrj = XMMatrixInverse(nullptr, XMprj);

	//D3DXMatrixIdentity(&VP);
	//	���̑���
	VP = XMMatrixIdentity();

	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	//D3DXMatrixInverse(&InvViewport, nullptr, &VP);
	XMMATRIX XMvp = XMLoadFloat4x4(&VP);
	InvViewport = XMMatrixInverse(nullptr, XMvp);

	//// �t�ϊ�
	Matrix tmp = InvViewport * InvPrj * InvView;
	//D3DXVec3TransformCoord(pout, &Vector3(Sx, Sy, fZ), &tmp);

	XMVECTOR vec = XMLoadFloat3(&Vector3(Sx, Sy, fZ));

	XMMATRIX XMtmp = XMLoadFloat4x4(&tmp);

	XMStoreFloat3(pout, XMVector3TransformCoord(vec, XMtmp));

	return pout;
}
