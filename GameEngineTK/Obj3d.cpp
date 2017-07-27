#include "Obj3d.h"
#include "VertexTypes.h"
#include <CommonStates.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// �萔
// ���\�[�X�f�B���N�g���p�X
const wstring Obj3d::RESOURCE_DIRECTORY = L"Resources/";
//// �t�@�C���g���q
//const wstring Obj3d::RESOURCE_EXT = L"";
// �ÓI�����o�ϐ��̎���
Obj3d::Common Obj3d::s_Common;

void Obj3d::InitializeCommon(CommonDef def)
{
	s_Common.device = def.pDevice;
	s_Common.deviceContext = def.pDeviceContext;
	s_Common.camera = def.pCamera;
	

//	m_order = WorldMatrixOrderFactory.Set(WorldMatrixOrder::ORDER::ROTOMAT_SCALEM_TRANSMAT);

	// �G�t�F�N�g�t�@�N�g������
	s_Common.effectFactory = std::make_unique<EffectFactory>(def.pDevice);
	// �e�N�X�`���ǂݍ��݃p�X�w��
	s_Common.effectFactory->SetDirectory(RESOURCE_DIRECTORY.c_str());

	// �ėp�X�e�[�g����
	s_Common.states = std::make_unique<CommonStates>(def.pDevice);

	// ���Z�`��p�̃u�����h�X�e�[�g���쐬
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT ret = def.pDevice->CreateBlendState(&desc, &s_Common.blendStateSubtract);
}

void Obj3d::UnloadAll()
{
	s_Common.modelarray.clear();
}

void Obj3d::SetSubtractive()
{
	// ���Z�`���ݒ�
	s_Common.deviceContext->OMSetBlendState(s_Common.blendStateSubtract, nullptr, 0xffffff);
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Obj3d::Obj3d()
	: m_pParent(nullptr)
	, m_pModel(nullptr)
	, m_UseQuternion(false)
{
	// �X�P�[����1�{���f�t�H���g
	m_Scale = Vector3(1, 1, 1);
	//���[���h�s��̊|���Z�����t�@�N�g���[�̏�����
	m_order = WorldMatrixOrderFactory.Set(WorldMatrixOrder::ORDER::ROTOMAT_SCALEM_TRANSMAT);

}

/// <summary>
/// �t�@�C�����烂�f����ǂݍ���
/// </summary>
/// <param name="filename">CMO�t�@�C����</param>
void Obj3d::LoadModel(const wchar_t*filename)
{
	assert(s_Common.effectFactory);

	// �w��t�@�C����ǂݍ��ݍς݂łȂ����H
	if (s_Common.modelarray.count(filename) == 0)
	{
		// �t���p�X�ɕ⊮
		wstring fullpath_bin = RESOURCE_DIRECTORY + filename;
		// ���f����ǂݍ��݁A�R���e�i�ɓo�^�i�L�[�̓t�@�C�����j
		s_Common.modelarray[filename] = Model::CreateFromCMO(s_Common.device, fullpath_bin.c_str(), *s_Common.effectFactory);
	}
	m_pModel = s_Common.modelarray[filename].get();


}

/**
*	@brief �I�u�W�F�N�g�̃��C�e�B���O�𖳌��ɂ���
*/
void Obj3d::DisableLighting()
{
	if (m_pModel)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// ���Ȕ������ő�l��
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// �G�t�F�N�g�Ɋ܂ޑS�Ă̕��s�������ɂ��ď�������
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ���C�g�𖳌��ɂ���
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

void Obj3d::EnableAlpha()
{
	if (m_pModel)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect>& ieff = meshpart->effect;
				meshpart->ModifyEffect(s_Common.device, ieff, true);
			}
		}
	}
}


/// <summary>
/// ���[���h�s��̊|���Z�����ւ���
/// </summary>
/// <param name="order"></param>
void Obj3d::ChangeOrder(WorldMatrixOrder::ORDER order)
{
	m_order.reset();
	m_order = this->WorldMatrixOrderFactory.Set(order);
}

void Obj3d::Update()
{
	Matrix scalem;
	Matrix rotm;
	Matrix transm;

	scalem = Matrix::CreateScale(m_Scale);

	if (m_UseQuternion)
	{
		rotm = Matrix::CreateFromQuaternion(m_RotQ);
	}
	else
	{
		rotm = Matrix::CreateFromYawPitchRoll(m_Rot.y, m_Rot.x, m_Rot.z);
	}

	transm = Matrix::CreateTranslation(m_Trans);

	// ���[���h�s���SRT�̏��ɍ���
	m_World = Matrix::Identity;

	m_order->Calculation(m_World, scalem, rotm,transm);

	// �e�s�񂪂����
	if (m_pParent)
	{
		// �e�s����|����
		m_World = m_World * m_pParent->GetLocalWorld();
	}
}

void Obj3d::Draw()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProjection();

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, m_World, view, projection);
	}
}

void Obj3d::DrawSubtractive()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
	//	const Matrix& projection = s_Common.camera->GetProj();

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// ���Z�`��p�̐ݒ�֐���n���ĕ`��
	//	m_pModel->Draw(s_Common.deviceContext, *s_Common.states, m_World, view, projection, false, Obj3d::SetSubtractive);
	}
}

/// <summary>
///  �r���{�[�h�`��
/// </summary>
void Obj3d::DrawBillboard()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProjection();

		// �r���{�[�h�s������[���h�s��ɍ���
		Matrix world = s_Common.camera->GetBillboard() * m_World;

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// ���Z�`��p�̐ݒ�֐���n���ĕ`��
		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, world, view, projection);
	}
}

/// <summary>
///  �r���{�[�h�`��
/// </summary>
void Obj3d::DrawBillboardConstrainY()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProjection();

		// �r���{�[�h�s������[���h�s��ɍ���
		Matrix world = s_Common.camera->GetBillboardConstrainY() * m_World;

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// ���Z�`��p�̐ݒ�֐���n���ĕ`��
		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, world, view, projection);
	}
}
