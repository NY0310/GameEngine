#include "Collision.h"
#include "../Component/Collider/TrianglePolygonListCollider/TrianglePolygonListCollider.h"
#include "../Component/Component.h"

using namespace DirectX::SimpleMath;
using namespace NYLibrary;


bool NYLibrary::CheckSpere2Sphere(const Sphere&sphereA, const Sphere& sphereB)
{
	//��A��B�̒��S���W�����߂�
	D3DXVECTOR3 sub = sphereB.center - sphereA.center;
	//A��B�̋������v�Z
	float distanceSquare = sub.x * sub.x * +sub.y  * sub.y + sub.z * sub.z;

	//���a�̘a���v�Z
	float radiusSquare = sphereA.radius + sphereB.radius;

	radiusSquare = radiusSquare * radiusSquare;

	//�����𔼌a�̘a���傫����Γ������Ă��Ȃ�
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	//�������Ă���
	return true;
}

// 3�_����O�p�`���\�z
void NYLibrary::ComputeTriangle(const D3DXVECTOR3& _p0, const D3DXVECTOR3& _p1, const D3DXVECTOR3& _p2, Triangle* _Triangle)
{
	//���W���R�s�[
	_Triangle->p0 = _p0;
	_Triangle->p1 = _p1;
	_Triangle->p2 = _p2;
	//�@���x�N�g�����v�Z
	D3DXVECTOR3 P0_P1 = _p1 - _p0;
	D3DXVECTOR3 P1_P2 = _p2 - _p1;
	D3DXVECTOR3 P2_P0 = _p2 - _p0;



	//2�x�N�g���ɐ����ȃx�N�g���𓾂�
	D3DXVec3Cross(&_Triangle->normal,&P0_P1,&P1_P2);
	//�x�N�g���𒷂���1�ɒ�������
	D3DXVec3Normalize(&_Triangle->normal, &_Triangle->normal);
}


//--------------------------------------------------------------------------------------------
// ���@�� : �_�ƎO�p�`�̍ŋߐړ_���v�Z
// ���@�� : _point	�_
//			_triangle	�O�p�`
//			_closest	�ŋߐړ_(���ʏo�͗p�j
// �߂�l : �Ȃ�
// ���@�� : 
//--------------------------------------------------------------------------------------------
void NYLibrary::ClosestPtPoint2Triangle(const D3DXVECTOR3& _point, const Triangle& _triangle, D3DXVECTOR3* _closest)
{
	// _point��P0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	D3DXVECTOR3 P0_P1 = _triangle.p1 - _triangle.p0;
	D3DXVECTOR3 P0_P2 = _triangle.p2 - _triangle.p0;
	D3DXVECTOR3 P0_PT = _point - _triangle.p0;

	float d1 =  D3DXVec3Dot(&P0_P1, &P0_PT);
	float d2 = D3DXVec3Dot(&P0_P2, &P0_PT);


	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// P0���ŋߖT
		*_closest = _triangle.p0;
		return;
	}

	// _point��P1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	D3DXVECTOR3 P1_PT = _point - _triangle.p1;

	float d3 = D3DXVec3Dot(&P0_P1, &P1_PT);
	float d4 = D3DXVec3Dot(&P0_P2,&P1_PT);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// P1���ŋߖT
		*_closest = _triangle.p1;
		return;
	}

	// _point��p0_P1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P0_P1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;

	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*_closest = _triangle.p0 + v * P0_P1;
		return;
	}

	// _point��P2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	D3DXVECTOR3 P2_PT = _point - _triangle.p2;

	float d5 = D3DXVec3Dot(&P0_P1, &P2_PT);
	float d6 = D3DXVec3Dot(&P0_P2, &P2_PT);

	if (d6 >= 0.0f && d5 <= d6)
	{
		*_closest = _triangle.p2;
		return;
	}

	// _point��P0_P2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P0_P2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*_closest = _triangle.p0 + w * P0_P2;
		return;
	}

	// _point��P1_P2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����_point��P1_P2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

		*_closest = _triangle.p1 + w * (_triangle.p2 - _triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*_closest = _triangle.p0 + P0_P1 * v + P0_P2 * w;
}

// �O�p�`�Ɠ_�̓����蔻��
bool NYLibrary::CheckPoint2Triangle(const D3DXVECTOR3& _point, const Triangle& _triangle)
{
	//�_�ƎO�p�`�͓��ꕽ�ʏ�ɂ�����̂Ƃ��Ă��܂��B���ꕽ�ʏ�ɖ����ꍇ�͐��������ʂɂȂ�܂���
	//����͊O�Ƃ݂Ȃ��܂��B
	//ABC���O�p�`���ǂ����̃`�F�b�N�͏ȗ�...
	// �_0��1�A 1��2�A 2��0 �̃x�N�g�������ꂼ��v�Z
	D3DXVECTOR3 v01 = _triangle.p1 - _triangle.p0;
	D3DXVECTOR3 v12 = _triangle.p2 - _triangle.p1;
	D3DXVECTOR3 v20 = _triangle.p0 - _triangle.p2;

	// �O�p�`�̊e���_����_�ւ̃x�N�g�������ꂼ��v�Z
	D3DXVECTOR3 v0p = _point - _triangle.p0;
	D3DXVECTOR3 v1p = _point - _triangle.p1;
	D3DXVECTOR3 v2p = _point - _triangle.p2;

	// �e�Ӄx�N�g���ƁA�_�ւ̃x�N�g���̊O�ς��v�Z
	D3DXVECTOR3 c0;
	D3DXVECTOR3 c1;
	D3DXVECTOR3 c2;
	D3DXVec3Cross(&c0, &v01, &v0p);
	D3DXVec3Cross(&c0, &v01, &v0p);
	D3DXVec3Cross(&c0, &v01, &v0p);

	// ���ςœ����������ǂ������ׂ�
	float dot01 = D3DXVec3Dot(&c0, &c1);
	float dot02 = D3DXVec3Dot(&c0,&c2);
	// �O�σx�N�g�����S�ē��������Ȃ�A�O�p�`�̓����ɓ_������
	if (dot01 > 0 && dot02 > 0)
	{
		return true;
	}

	//�O�p�`�̊O���ɓ_������
	return false;
}



//--------------------------------------------------------------------------------------------
// ���@�� : ���Ɩ@���t���O�p�`�̓�����`�F�b�N
// ���@�� : _sphere		��
//			_tri		�@���t���O�p�`
//			_inter		��_�i�ȗ��j
// �߂�l : �������Ă��邩�ۂ�
// ���@�� : ���ʂ̓�����͂Ƃ�Ȃ�
//--------------------------------------------------------------------------------------------
bool NYLibrary::CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _triangle, D3DXVECTOR3 *_inter)
{
	D3DXVECTOR3 p;

	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(_sphere.center, _triangle, &p);

	D3DXVECTOR3 v = p - _sphere.center;

	// ���ƎO�p�`����������̂́A���̒��S����_p�܂ł̋��������̔��a�����������ꍇ
	if (D3DXVec3Dot(&v,&v) <= _sphere.radius * _sphere.radius)
	{
		if (_inter)
		{
			*_inter = p;
		}

		return true;
	}

	return false;

	// ���ƕ��ʁi�O�p�`������Ă��镽�ʁj�̓����蔻��
	// ���ƕ��ʂ̋������v�Z
	float ds = D3DXVec3Dot(&_sphere.center,&_triangle.normal);
	float dt = D3DXVec3Dot(&_triangle.p0,&_triangle.normal);
	float dist = ds - dt;
	// ���������a�ȏ�Ȃ�A������Ȃ�
	if (fabsf(dist) > _sphere.radius)	return false;

	// ���S�_�𕽖ʂɎˉe�����Ƃ��A�O�p�`�̓����ɂ���΁A�������Ă���
	// �ˉe�������W
	D3DXVECTOR3 center = -dist * _triangle.normal + _sphere.center;

	// �O�p�`�̊O���ɂȂ���΁A�������Ă��Ȃ�
	if (!CheckPoint2Triangle(center, _triangle))	return false;

	if (_inter)
	{
		*_inter = center;	// ��_���R�s�[
	}

	return true;
}


//--------------------------------------------------------------------------------------------
// ���@�� : �����i�L���j�Ɩ@���t���O�p�`�̓�����`�F�b�N
// ���@�� : _segment		�����̐����istart��end���������j
//			_tri		�@���t���O�p�`
//			_inter		��_�i�ȗ��j
// �߂�l : �������Ă��邩�ۂ�
// ���@�� : ���ʂ̓�����͂Ƃ�Ȃ�
//--------------------------------------------------------------------------------------------
//bool NYLibrary::CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, D3DXVECTOR3 *_inter)
//{
//	const float epsilon = -1.0e-5f;	// �덷�z���p�̔����Ȓl
//	D3DXVECTOR3 	LayV;		// �����̏I�_���n�_
//	D3DXVECTOR3 	tls;		// �O�p�`�̒��_0�������̎n�_
//	D3DXVECTOR3 	tle;		// �O�p�`�̒��_0�������̏I�_
//	float 	distl0;
//	float 	distl1;
//	float 	dp;
//	float 	denom;
//	float 	t;
//	D3DXVECTOR3	s;			// �����ƕ��ʂƂ̌�_
//	D3DXVECTOR3 	st0;		// ��_���O�p�`�̒��_0
//	D3DXVECTOR3 	st1;		// ��_���O�p�`�̒��_1
//	D3DXVECTOR3 	st2;		// ��_���O�p�`�̒��_2
//	D3DXVECTOR3 	t01;		// �O�p�`�̒��_0�����_1
//	D3DXVECTOR3 	t12;		// �O�p�`�̒��_1�����_2
//	D3DXVECTOR3 	t20;		// �O�p�`�̒��_2�����_0
//	D3DXVECTOR3	m;
//
//	// �����̎n�_���O�p�n�̗����ɂ���΁A������Ȃ�
//	tls = _segment.start - _triangle.p0;
//	distl0 = D3DXVec3Dot(&tls,&_triangle.normal);	// �����̎n�_�ƕ��ʂ̋���
//	if (distl0 <= epsilon) return false;
//
//	// �����̏I�_���O�p�n�̕\���ɂ���΁A������Ȃ�
//	tle = _segment.end - _triangle.p0;
//	distl1 = D3DXVec3Dot(&tle,&_triangle.normal);	// �����̏I�_�ƕ��ʂ̋���
//	if (distl1 >= -epsilon) return false;
//
//	// �����ƕ��ʂƂ̌�_s�����
//	denom = distl0 - distl1;
//	t = distl0 / denom;
//	LayV = _segment.end - _segment.start;	// �����̕����x�N�g�����擾
//	s = t * LayV + _segment.start;
//
//	// ��_���O�p�`�̓����ɂ��邩�ǂ����𒲂ׂ�B
//	// �O�p�`�̓����ɂ���ꍇ�A��_����e���_�ւ̃x�N�g���Ɗe�Ӄx�N�g���̊O�ρi�O�g�j���A�S�Ė@���Ɠ�������������
//	// ��ł���������v���Ȃ���΁A������Ȃ��B
//	st0 = _triangle.p0 - s;
//	t01 = _triangle.p1 - _triangle.p0;
//	D3DXVec3Cross(&m,&st0,&t01);
//	dp = D3DXVec3Dot(&m,&_triangle.normal);
//	if (dp <= epsilon) return false;
//
//	st1 = _triangle.p1 - s;
//	t12 = _triangle.p2 - _triangle.p1;
//	D3DXVec3Cross(&m,&st1,&t12);
//	dp = D3DXVec3Dot(&m,&_triangle.normal);
//	if (dp <= epsilon) return false;
//
//	st2 = _triangle.p2 - s;
//	t20 = _triangle.p0 - _triangle.p2;
//	D3DXVec3Cross(&m,&st2,&t20);
//	dp = D3DXVec3Dot(&m,&_triangle.normal);
//	if (dp <= epsilon) return false;
//
//	if (_inter)
//	{
//		*_inter = s;	// ��_���R�s�[
//	}
//
//	return true;
//}

void NYLibrary::CheckSegment2AllTriangle(SegmentCollider * segment, TrianglePolygonListCollider * trianglePolygonListCollider)
{
	D3DXVECTOR3 inter;
	for (auto& triangle : trianglePolygonListCollider->GetTriangleList())
	{
		if (CheckSegment2Triangle(segment, triangle,&inter))
		{
			//���Ƃ̌�_��ݒ肷��
			trianglePolygonListCollider->SetInter(inter);
			//���������O�p�`�|���S����ݒ肷��
			trianglePolygonListCollider->SetCollisionTriangle(triangle);
			//���������R���C�_�[��o�^����
			segment->AddCollisionCollider(trianglePolygonListCollider);
			trianglePolygonListCollider->AddCollisionCollider(segment);
			//���������ʒm������
			segment->OnCollision();
			trianglePolygonListCollider->OnCollision();

		}
	}
}

//--------------------------------------------------------------------------------------------
// ���@�� : �����i�L���j�Ɩ@���t���O�p�`�̓�����`�F�b�N
// ���@�� : segment		�����̐����istart��end���������j
//			_tri		�@���t���O�p�`
//			_inter		��_�i�ȗ��j
// �߂�l : �������Ă��邩�ۂ�
// ���@�� : ���ʂ̓�����͂Ƃ�Ȃ�
//--------------------------------------------------------------------------------------------
bool NYLibrary::CheckSegment2Triangle(const SegmentCollider* segment, Triangle* triangle, D3DXVECTOR3* inter)
{
	const float epsilon = -1.0e-5f;	// �덷�z���p�̔����Ȓl
	D3DXVECTOR3 	LayV;		// �����̏I�_���n�_
	D3DXVECTOR3 	tls;		// �O�p�`�̒��_0�������̎n�_
	D3DXVECTOR3 	tle;		// �O�p�`�̒��_0�������̏I�_
	float 	distl0;
	float 	distl1;
	float 	dp;
	float 	denom;
	float 	t;
	D3DXVECTOR3	s;			// �����ƕ��ʂƂ̌�_
	D3DXVECTOR3 	st0;		// ��_���O�p�`�̒��_0
	D3DXVECTOR3 	st1;		// ��_���O�p�`�̒��_1
	D3DXVECTOR3 	st2;		// ��_���O�p�`�̒��_2
	D3DXVECTOR3 	t01;		// �O�p�`�̒��_0�����_1
	D3DXVECTOR3 	t12;		// �O�p�`�̒��_1�����_2
	D3DXVECTOR3 	t20;		// �O�p�`�̒��_2�����_0
	D3DXVECTOR3	m;

	// �����̎n�_���O�p�n�̗����ɂ���΁A������Ȃ�
	tls = segment->start - triangle->p0;
	distl0 = D3DXVec3Dot(&tls, &triangle->normal);	// �����̎n�_�ƕ��ʂ̋���
	if (distl0 <= epsilon) return false;

	// �����̏I�_���O�p�n�̕\���ɂ���΁A������Ȃ�
	tle = segment->end - triangle->p0;
	distl1 = D3DXVec3Dot(&tle, &triangle->normal);	// �����̏I�_�ƕ��ʂ̋���
	if (distl1 >= -epsilon) return false;

	// �����ƕ��ʂƂ̌�_s�����
	denom = distl0 - distl1;
	t = distl0 / denom;
	LayV = segment->end - segment->start;	// �����̕����x�N�g�����擾
	s = t * LayV + segment->start;

	// ��_���O�p�`�̓����ɂ��邩�ǂ����𒲂ׂ�B
	// �O�p�`�̓����ɂ���ꍇ�A��_����e���_�ւ̃x�N�g���Ɗe�Ӄx�N�g���̊O�ρi�O�g�j���A�S�Ė@���Ɠ�������������
	// ��ł���������v���Ȃ���΁A������Ȃ��B
	st0 = triangle->p0 - s;
	t01 = triangle->p1 - triangle->p0;
	D3DXVec3Cross(&m, &st0, &t01);
	dp = D3DXVec3Dot(&m, &triangle->normal);
	if (dp <= epsilon) return false;

	st1 = triangle->p1 - s;
	t12 = triangle->p2 - triangle->p1;
	D3DXVec3Cross(&m, &st1, &t12);
	dp = D3DXVec3Dot(&m, &triangle->normal);
	if (dp <= epsilon) return false;

	st2 = triangle->p2 - s;
	t20 = triangle->p0 - triangle->p2;
	D3DXVec3Cross(&m, &st2, &t20);
	dp = D3DXVec3Dot(&m, &triangle->normal);
	if (dp <= epsilon) return false;

	inter = &s;	// ��_���R�s�[

	return true;
}
