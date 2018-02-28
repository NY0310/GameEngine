////--------------------------------------------------------------------------------------
//// �t�@�C����: LandShape
//// �쐬��:
//// �쐬��:
//// ����:�n�`������
////--------------------------------------------------------------------------------------
//
//#include <fstream>
//#include <algorithm>
//#include <DirectXColors.h>
//#include "LandShape.h"
//#include "CollisionNode.h"
//
//using namespace std;
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//
//
//// �ÓI�����o�ϐ��̎���
//std::unique_ptr<LandShapeCommon> LandShape::s_pCommon;
//std::map<std::wstring, std::unique_ptr<LandShapeData>> LandShape::s_dataarray;
//
//LandShapeCommon::LandShapeCommon(LandShapeCommonDef def)
//{
//	// �J����
//	m_pCamera = def.pCamera;
//	// �`��X�e�[�g
//	m_pStates.reset(new CommonStates(def.pDevice));
//	//// �G�t�F�N�g�t�@�N�g��
//	//m_pEffectFactory.reset(new EffectFactory(def.pDevice));
//	//// �v���~�e�B�u�o�b�`
//	//m_pPrimitiveBatch.reset(new PrimitiveBatch<VertexPositionNormal>(def.pDeviceContext, BatchSize * 3, BatchSize));
//	//// �G�t�F�N�g
//	//m_pEffect.reset(new BasicEffect(def.pDevice));
//	//// ���C�e�B���O�L��
//	//m_pEffect->SetLightingEnabled(true);
//	//// �}�e���A���J���[�ݒ�
//	//m_pEffect->SetAmbientLightColor(Vector3(0, 0.0f, 0));
//	//m_pEffect->SetDiffuseColor(Vector3(1.0f, 1.0f, 1.0f));
//	//// ���C�g0�i�O���[���j
//	//m_pEffect->SetLightEnabled(0, true);
//	//m_pEffect->SetLightDiffuseColor(0, Vector3(0.1f, 0.6f, 0.1f));
//	//m_pEffect->SetLightDirection(0, Vector3(0, -1.0f, 0));
//	//// ���C�g1�i�s���N�j
//	//m_pEffect->SetLightEnabled(1, true);
//	//m_pEffect->SetLightDiffuseColor(1, Vector3(0.5f, 0.2f, 0.3f));
//	//m_pEffect->SetLightDirection(1, Vector3(-1, 0, -2));
//	//// ���C�g2�i���F�j
//	//m_pEffect->SetLightEnabled(2, true);
//	//m_pEffect->SetLightDiffuseColor(2, Vector3(0.3f, 0.3f, 0.6f));
//	//m_pEffect->SetLightDirection(2, Vector3(1, 0, 2));
//	//// �t�H�O�i�D�F�j �����ߊ����������邽�߂Ɏg��
//	//m_pEffect->SetFogEnabled(true);
//	//m_pEffect->SetFogColor(Colors::White);
//	//m_pEffect->SetFogStart(2.f);
//	//m_pEffect->SetFogEnd(10.f);
//
//
//	//void const* shaderByteCode;
//	//size_t byteCodeLength;
//
//	//// �V�F�[�_�[�̎擾
//	//m_pEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
//
//	//// ���̓��C�A�E�g�̍쐬
//	//def.pDevice->CreateInputLayout(VertexPositionNormal::InputElements,
//	//	VertexPositionNormal::InputElementCount,
//	//	shaderByteCode, byteCodeLength,
//	//	&m_pInputLayout);
//	//// �f�o�C�X�R���e�L�X�g
//	//m_pDeviceContext = def.pDeviceContext;
//}
//
//LandShapeCommon::~LandShapeCommon()
//{
//	// ���̓��C�A�E�g�̉��
//	if (m_pInputLayout)
//	{
//		m_pInputLayout->Release();
//		m_pInputLayout = nullptr;
//	}
//}
//
///**
//*	@brief �G�t�F�N�g�t�@�N�g���[����
//*/
//void LandShape::InitializeCommon(LandShapeCommonDef def)
//{
//	// ���ɏ������ς�
//	if (s_pCommon)	return;
//
//	// ���ʃf�[�^����
//	s_pCommon.reset(new LandShapeCommon(def));
//}
//
////--------------------------------------------------------------------------------------
//// �R���X�g���N�^
////--------------------------------------------------------------------------------------
//LandShape::LandShape()
//	: m_pData(nullptr)
//{
//
//}
//
////--------------------------------------------------------------------------------------
//// ������
////--------------------------------------------------------------------------------------
//void LandShape::Initialize(const wstring& filename_bin, const wstring& filename_cmo)
//{
//	// �t�@�C�������󔒂łȂ����
//	if (filename_bin.size() > 0)
//	{
//		// �t���p�X�ɕ⊮
//		wstring fullpath_bin = L"LandShape/" + filename_bin + L".landshape";
//
//		std::map<std::wstring, std::unique_ptr<LandShapeData>>::iterator it;
//		it = s_dataarray.find(fullpath_bin);
//		if (s_dataarray.count(fullpath_bin) == 0)
//		{
//			// ���f���̓ǂݍ���
//			s_dataarray[fullpath_bin] = LandShapeData::CreateFromFile(fullpath_bin.c_str());
//		}
//		// �n�`�f�[�^���Z�b�g
//		m_pData = s_dataarray[fullpath_bin].get();
//	}
//
//	// �t�@�C�������󔒂łȂ����
//	if (filename_cmo.size() > 0)
//	{
//		// �t���p�X�ɕ⊮
//		wstring fullpath_cmo =/* L"Resources/" +*/ filename_cmo + L".cmo";
//		// �I�u�W�F�N�g������
//		m_Obj.LoadModel(fullpath_cmo.c_str());
//	}
//}
//
////--------------------------------------------------------------------------------------
//// ���[���h�s��̌v�Z
////--------------------------------------------------------------------------------------
//void LandShape::Update()
//{
//	m_Obj.Update();
//	// �t�s����v�Z
//	const Matrix& localworld = m_Obj.GetLocalWorld();
//	m_WorldLocal = localworld.Invert();
//}
//
////--------------------------------------------------------------------------------------
//// �n�`�f�[�^�̕`��
////--------------------------------------------------------------------------------------
//void LandShape::Draw()
//{
//	////�f�o�b�N�\�����I�t�Ȃ�
//	//if (CollisionNode::GetDebugVisible() == false)
//	//{
//	//	// ���f���`��
//	//	m_Obj.Draw();
//	//}
//	//else if (m_pData)
//	//{
//	//	// �f�o�b�O�`��
//	//	const Matrix& view = s_pCommon->m_pCamera->GetView();
//	//	const Matrix& projection = s_pCommon->m_pCamera->GetProjection();
//
//	//	// �쐬�����s����G�t�F�N�g�ɃZ�b�g
//	//	s_pCommon->m_pEffect->SetWorld(m_Obj.GetLocalWorld());
//	//	s_pCommon->m_pEffect->SetView(view);
//	//	s_pCommon->m_pEffect->SetProjection(projection);
//
//	//	// �G�t�F�N�g�̐ݒ�i�e�s���e�N�X�`���Ȃǂ�ݒ肵�Ă���j
//	//	s_pCommon->m_pEffect->Apply(s_pCommon->m_pDeviceContext);
//
//	//	// �[�x�X�e���V�� �X�e�[�g��ݒ肷��
//	//	s_pCommon->m_pDeviceContext->OMSetDepthStencilState(s_pCommon->m_pStates->DepthDefault(), 0);
//
//	//	// �u�����f�B���O �X�e�[�g��ݒ肷��
//	//	s_pCommon->m_pDeviceContext->OMSetBlendState(s_pCommon->m_pStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);
//
//	//	// ���X�^���C�U �X�e�[�g��ݒ肷�� ���v�����\��
//	//	s_pCommon->m_pDeviceContext->RSSetState(s_pCommon->m_pStates->CullNone());
//
//	//	// �T���v���[�X�e�[�g��ݒ肷��
//	//	auto samplerState = s_pCommon->m_pStates->PointWrap();
//	//	s_pCommon->m_pDeviceContext->PSSetSamplers(0, 1, &samplerState);
//
//	//	// ���̓��C�A�E�g��ݒ肷��
//	//	s_pCommon->m_pDeviceContext->IASetInputLayout(s_pCommon->m_pInputLayout);
//
//	//	// �`��J�n
//	//	s_pCommon->m_pPrimitiveBatch->Begin();
//
//	//	const uint16_t* pIndex = &m_pData->m_Indices[0];
//	//	int numIndex = m_pData->m_Indices.size();
//
//	//	const VertexPositionNormal* pVertex = (VertexPositionNormal*)&m_pData->m_Vertices[0];
//	//	int numVertex = m_pData->m_Vertices.size();
//
//	//	// �O�p�`�v���~�e�B�u�̕`��
//	//	s_pCommon->m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pIndex, numIndex, pVertex, numVertex);
//
//	//	// �`��I��
//	//	s_pCommon->m_pPrimitiveBatch->End();
//	//}
//}
//
//void LandShape::DisableLighting()
//{
//	//	m_Obj.DisableLighting();
//}
//
////--------------------------------------------------------------------------------------
//// �n�`�Ƌ��̌�������
//// sphere : ���苅
//// reject : �����o���x�N�g��
////--------------------------------------------------------------------------------------
//bool LandShape::IntersectSphere(const Sphere& sphere, Vector3* reject)
//{
//
//	if (m_pData == nullptr) return false;
//
//	// �q�b�g�t���O��������
//	bool hit = false;
//	// �傫�������ŏ�����
//	float over_length = 1.0e5;
//	Vector3 l_inter;
//	Vector3 l_normal;
//	//	Vector3 l_down;
//	// �X�P�[�����擾
//	float scale = GetScale();
//
//	// �����R�s�[
//	Sphere localsphere = sphere;
//
//	// �X�P�[��0�̏ꍇ�A���肵�Ȃ�
//	if (scale <= 1.0e-10) return false;
//
//	// ���̒��S�_�����[���h���W���烂�f�����W�n�Ɉ�������
//	localsphere.Center = Vector3::Transform(sphere.Center, m_WorldLocal);
//	// ���a�����[���h�����[���h���W�n���烂�f�����W�n�ɕϊ�
//	localsphere.Radius = sphere.Radius / scale;
//
//	// �O�p�`�̐�
//	int nTri = m_pData->m_Triangles.size();
//	// �S�Ă̎O�p�`�ɂ���
//	for (int i = 0; i < nTri; i++)
//	{
//		float temp_over_length;
//		Vector3 temp_inter;
//
//		const Triangle& tri = m_pData->m_Triangles[i];
//
//		// �O�p�`�Ƌ��̓����蔻��
//		if (CheckSphere2Triangle(localsphere, tri, &temp_inter))
//		{// �q�b�g����
//			hit = true;
//			// �Փ˓_���狅�̒��S�ւ̃x�N�g��
//			Vector3 sub = localsphere.Center - temp_inter;
//			// ���̒��S���O�p�`�ɂ߂肱��ł��鋗�����v�Z
//			temp_over_length = sub.Dot(-tri.Normal);
//
//			// �߂肱�݋�������܂łōŏ��Ȃ�
//			if (temp_over_length < over_length)
//			{
//				// �q�b�g���W�A�@���A�߂肱�݋������L�^
//				l_inter = temp_inter;
//				l_normal = tri.Normal;
//				over_length = temp_over_length;
//			}
//		}
//	}
//
//	if (hit)
//	{
//		// ���������f�����W�n���烏�[���h���W�n�ł̒����ɕϊ�
//		over_length *= scale;
//
//		// ���[���h�s����擾
//		const Matrix& localworld = m_Obj.GetLocalWorld();
//
//		// �r�˃x�N�g���̌v�Z
//		if (reject != nullptr)
//		{
//			// �n�`�̖@�����������f�����W�n���烏�[���h���W�n�ɕϊ�
//			*reject = Vector3::TransformNormal(l_normal, localworld);
//			reject->Normalize();
//			// �߂荞�ݕ����������o���x�N�g�����v�Z
//			const float extra = 0.05f;
//			float reject_distance = sphere.Radius + over_length + extra;
//			*reject = (*reject) * reject_distance;
//		}
//	}
//
//	return hit;
//}
//
////--------------------------------------------------------------------------------------
//// �n�`�Ɛ����̌�������
//// segment : ����
//// �i�o�́jinter : ��_�i�|���S���̕��ʏ�ŁA�_�Ƃ̍Đڋߓ_�̍��W��Ԃ��j
////--------------------------------------------------------------------------------------
//bool LandShape::IntersectSegment(const Segment& segment, Vector3* inter)
//{
//
//
//	if (m_pData == nullptr) return false;
//
//	// �q�b�g�t���O��������
//	bool hit = false;
//	// �傫�������ŏ�����
//	float distance = 1.0e5;
//	// �p�x����p�ɒn�ʂƂ݂Ȃ��p�x�̌��E�l<�x>
//	const float limit_angle = XMConvertToRadians(30.0f);
//	Vector3 l_inter;
//
//	// �R�s�[
//	Segment localSegment = segment;
//	// ���������[���h���W���烂�f�����W�n�Ɉ�������
//	localSegment.Start = Vector3::Transform(localSegment.Start, m_WorldLocal);
//	localSegment.End = Vector3::Transform(localSegment.End, m_WorldLocal);
//	// �����̕����x�N�g�����擾
//	Vector3 segmentNormal = localSegment.End - localSegment.Start;
//	segmentNormal.Normalize();
//
//	// �O�p�`�̐�
//	int nTri = m_pData->m_Triangles.size();
//	// �S�Ă̎O�p�`�ɂ���
//	for (int i = 0; i < nTri; i++)
//	{
//		float temp_distance;
//		Vector3 temp_inter;
//
//		// ������x�N�g���Ɩ@���̓���
//		// �������P�̃x�N�g���Q���m�̓��ς́A�R�T�C���i�x�N�g���̓��ς̒�`���j
//		float cosine = -segmentNormal.Dot(m_pData->m_Triangles[i].Normal);
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
//		if (cosine < limit_cosine) continue;
//		//--�����ł����܂�--
//
//		// �����ƎO�p�`�i�|���S���j�̌�������
//		if (CheckSegment2Triangle(localSegment, m_pData->m_Triangles[i], &temp_inter))
//		{
//			hit = true;
//			// �����̎n�_�ƏՓ˓_�̋������v�Z�i�߂肱�݋����j
//			temp_distance = Vector3::Distance(localSegment.Start, temp_inter);
//			// �߂肱�݋�������܂łōŏ��Ȃ�
//			if (temp_distance < distance)
//			{
//				// �Փ˓_�̍��W�A�߂肱�݋������L�^
//				l_inter = temp_inter;
//				distance = temp_distance;
//			}
//		}
//	}
//
//	if (hit && inter != nullptr)
//	{
//		// �Փ˓_�̍��W�����f�����W�n���烏�[���h���W�n�ɕϊ�
//		const Matrix& localworld = m_Obj.GetLocalWorld();
//		*inter = Vector3::Transform(l_inter, localworld);
//	}
//
//	return hit;
//}