#include "PaintCollision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace NYLibrary;

//--------------------------------------------------------------------------------------
// �n�`�Ɛ����̌�������
// segment : ����
// �i�o�́jinter : ��_�i�|���S���̕��ʏ�ŁA�_�Ƃ̍Đڋߓ_�̍��W��Ԃ��j
//--------------------------------------------------------------------------------------
bool PaintCollision::IntersectSegment(Segment* segment, D3DXVECTOR2& uv)
{

////	if (mesh.pIndexBuffer == nullptr) return false;
//
//	// �傫�������ŏ�����
//	float distance = 1.0e5;
//	// �p�x����p�ɒn�ʂƂ݂Ȃ��p�x�̌��E�l<�x>
//	const float limit_angle = XMConvertToRadians(30.0f);
//	Vector3 l_inter;
//
//
//	// �t�s����v�Z
//	D3DXMATRIX d3dWorldLocal;
//	D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldMatrix);
//	Matrix WorldLocal = Math::D3DXMATRIXToMatrix(d3dWorldLocal);
//
//
//	// �R�s�[
//	Segment localSegment = *segment;
//	// ���������[���h���W���烂�f�����W�n�Ɉ�������
//	localSegment.Start = Vector3::Transform(localSegment.Start, WorldLocal);
//	localSegment.End = Vector3::Transform(localSegment.End, WorldLocal);
//	// �����̕����x�N�g�����擾
//	Vector3 segmentNormal = localSegment.End - localSegment.Start;
//	segmentNormal.Normalize();
//
//	// �S�Ă̎O�p�`�ɂ���
//	for (auto triangle : triangles)
//	{
//		float temp_distance;
//		Vector3 temp_inter;
//		if (triangle.Normal == Vector3(0, 0, 0))
//			continue;
//		// ������x�N�g���Ɩ@���̓���
//		// �������P�̃x�N�g���Q���m�̓��ς́A�R�T�C���i�x�N�g���̓��ς̒�`���j
//		float cosine = -segmentNormal.Dot(triangle.Normal);
//		//// �R�T�C���l����A������Ƃ̊p�x�����v�Z
//		//float angle = acosf(cosine);
//		//// ������Ƃ̊p�x�����E�p���傫����΁A�ʂ̌X�����傫���̂ŁA�n�ʂƂ݂Ȃ����X�L�b�v
//		//if ( angle > limit_angle ) continue;
//
//		//--������--
//		const float limit_cosine = cosf(limit_angle);
//		// �R�T�C�����P�̂Ƃ��Ƀx�N�g���Ԃ̊p�x��0�x�ł���A�x�N�g���̊p�x�����傫���قǁA�R�T�C���͏������̂ŁA
//		// �R�T�C���l�̂܂ܔ�r����ƁA�p�x�̔�r�̏ꍇ�Ƒ召�֌W���t�ł���
//		// �܂�A�R�T�C���l�����l��菬������΁A�ʂ̌X�����傫���̂ŁA�n�ʂƂ݂Ȃ����X�L�b�v
//		//if (cosine < limit_cosine) continue;
//		//--�����ł����܂�--
//		// �����ƎO�p�`�i�|���S���j�̌�������
//		if (CheckSegment2Triangle(localSegment, triangle, &temp_inter))
//		{
//			// �����̎n�_�ƏՓ˓_�̋������v�Z�i�߂肱�݋����j
//			temp_distance = Vector3::Distance(localSegment.start, temp_inter);
//			// �߂肱�݋�������܂łōŏ��Ȃ�
//			{
//				// �Փ˓_�̍��W�A�߂肱�݋������L�^
//				l_inter = temp_inter;
//				distance = temp_distance;
//
//
//				Vector3 p1 = triangle.P0;
//				Vector3 p2 = triangle.P1;
//				Vector3 p3 = triangle.P2;
//				Vector3 p = l_inter;
//
//				Vector2 uv1 = triangle.Uv0;
//				Vector2 uv2 = triangle.Uv1;
//				Vector2 uv3 = triangle.Uv2;
//
//				//�h����I�u�W�F�N�g�̃��[���h���W��������
//				//PerspectiveCollect(�������e���l������UV���)
//				Matrix mvp = Math::D3DXMATRIXToMatrix(worldMatrix * camera->GetView()* camera->GetProjection());
//				//�e�_��ProjectionSpace�ւ̕ϊ�
//				Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 0));
//				Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 0));
//				Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 0));
//				Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 0));
//
//
//				//�ʏ���W�ւ̕ϊ�(ProjectionSpace)
//				Vector2 p1_n = Vector2(p1_p.x, p1_p.y) / p1_p.w;
//				Vector2 p2_n = Vector2(p2_p.x, p2_p.y) / p2_p.w;
//				Vector2 p3_n = Vector2(p3_p.x, p3_p.y) / p3_p.w;
//				Vector2 p_n = Vector2(p_p.x, p_p.y) / p_p.w;
//
//
//				//���_�̂Ȃ��O�p�`��_p�ɂ��3�������A�K�v�ɂȂ�ʐς��v�Z
//				float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
//				float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
//				float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
//				//�ʐϔ䂩��uv����
//				float u = s1 / s;
//				float v = s2 / s;
//				float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
//				Vector2 _uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);
//				uv = Math::VectorToD3DXVECTOR2(_uv);
//				return true;
//			}
//		}
//	}
//
//
//
	return false;
}



//--------------------------------------------------------------------------------------
// �O�p�`�Ɖ~�̌�������
// segment : ����
// �i�o�́jinter : ��_�i�|���S���̕��ʏ�ŁA�_�Ƃ̍Đڋߓ_�̍��W��Ԃ��j
//--------------------------------------------------------------------------------------
bool PaintCollision::IntersectSphere(Sphere* sphere, D3DXVECTOR2 & uv)
{

	//// �t�s����v�Z
	//D3DXMATRIX d3dWorldLocal;
	//D3DXMatrixInverse(&d3dWorldLocal, nullptr, &worldMatrix);
	//Matrix WorldLocal = Math::D3DXMATRIXToMatrix(d3dWorldLocal);

	//// �q�b�g�t���O��������
	//bool hit = false;
	//// �傫�������ŏ�����
	//float over_length = 1.0e5;
	//Vector3 l_inter;
	//Vector3 l_normal;
	////	Vector3 l_down;
	//// �X�P�[�����擾
	//float scale = sphere->Radius * 2;

	//// �����R�s�[
	//Sphere* localsphere = sphere;

	//// �X�P�[��0�̏ꍇ�A���肵�Ȃ�
	//if (scale <= 1.0e-10) return false;

	//// ���̒��S�_�����[���h���W���烂�f�����W�n�Ɉ�������
	//localsphere->Center = Vector3::Transform(sphere->Center, WorldLocal);
	//// ���a�����[���h�����[���h���W�n���烂�f�����W�n�ɕϊ�
	//localsphere->radius = sphere->radius / scale;
	//for (auto triangle : triangles)
	//{
	//	{
	//		float temp_over_length;
	//		Vector3 temp_inter;

	//		Triangle& tri = triangle;

	//		// �O�p�`�Ƌ��̓����蔻��
	//		if (CheckSphere2Triangle(*localsphere, tri, &temp_inter))
	//		{

	//			Vector3 p1 = triangle.P0;
	//			Vector3 p2 = triangle.P1;
	//			Vector3 p3 = triangle.P2;
	//			Vector3 p = temp_inter;

	//			Vector2 uv1 = triangle.Uv0;
	//			Vector2 uv2 = triangle.Uv1;
	//			Vector2 uv3 = triangle.Uv2;

	//			//�h����I�u�W�F�N�g�̃��[���h���W��������
	//			//PerspectiveCollect(�������e���l������UV���)
	//			Matrix mvp = Math::D3DXMATRIXToMatrix(worldMatrix * camera->GetView()* camera->GetProjection());
	//			//�e�_��ProjectionSpace�ւ̕ϊ�
	//			Vector4 p1_p = MatrixTimes(mvp, Vector4(p1.x, p1.y, p1.z, 1));
	//			Vector4 p2_p = MatrixTimes(mvp, Vector4(p2.x, p2.y, p1.z, 1));
	//			Vector4 p3_p = MatrixTimes(mvp, Vector4(p3.x, p3.y, p1.z, 1));
	//			Vector4 p_p = MatrixTimes(mvp, Vector4(p.x, p.y, p.z, 1));


	//			//�ʏ���W�ւ̕ϊ�(ProjectionSpace)
	//			Vector2 p1_n = Vector2(p1_p.x, p1_p.y) / p1_p.w;
	//			Vector2 p2_n = Vector2(p2_p.x, p2_p.y) / p2_p.w;
	//			Vector2 p3_n = Vector2(p3_p.x, p3_p.y) / p3_p.w;
	//			Vector2 p_n = Vector2(p_p.x, p_p.y) / p_p.w;


	//			//���_�̂Ȃ��O�p�`��_p�ɂ��3�������A�K�v�ɂȂ�ʐς��v�Z
	//			float s = 0.5f * ((p2_n.x - p1_n.x) * (p3_n.y - p1_n.y) - (p2_n.y - p1_n.y) * (p3_n.x - p1_n.x));
	//			float s1 = 0.5f * ((p3_n.x - p_n.x) * (p1_n.y - p_n.y) - (p3_n.y - p_n.y) * (p1_n.x - p_n.x));
	//			float s2 = 0.5f * ((p1_n.x - p_n.x) * (p2_n.y - p_n.y) - (p1_n.y - p_n.y) * (p2_n.x - p_n.x));
	//			//�ʐϔ䂩��uv����
	//			float u = s1 / s;
	//			float v = s2 / s;
	//			float w = 1 / ((1 - u - v) * 1 / p1_p.w + u * 1 / p2_p.w + v * 1 / p3_p.w);
	//			Vector2 _uv = w * ((1 - u - v) * uv1 / p1_p.w + u * uv2 / p2_p.w + v * uv3 / p3_p.w);

	//			uv = Math::VectorToD3DXVECTOR2(_uv);
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	return false;
}

Vector4 PaintCollision::MatrixTimes(const Matrix& matrix, const Vector4& vector)
{
	Vector4 vec;
	vec.x = matrix._11 * vector.x + matrix._21 * vector.y + matrix._31 * vector.z + matrix._41 * vector.w;
	vec.y = matrix._12 * vector.x + matrix._22 * vector.y + matrix._32 * vector.z + matrix._42 * vector.w;
	vec.z = matrix._13 * vector.x + matrix._23 * vector.y + matrix._33 * vector.z + matrix._43 * vector.w;
	vec.w = matrix._14 * vector.x + matrix._24 * vector.y + matrix._34 * vector.z + matrix._44 * vector.w;
	return vec;
}



