///// <summary>
///// �R�c�I�u�W�F�N�g�N���X
///// </summary>
//#pragma once
//
//#include <Model.h>
//#include <Effects.h>
//
//#include "Camera.h"
//#include <map>
//
//
//class WorldMatrixOrder {
//public:
//	enum ORDER {
//		SCALEM_ROTOMAT_TRANSMAT,
//		SCALEM_TRANSMAT_ROTOMAT,
//		TRANSMAT_ROTOMAT_SCALEM,
//		TRANSMAT_SCALEM_ROTOMAT,
//		ROTOMAT_TRANSMAT_SCALEM,
//		ROTOMAT_SCALEM_TRANSMAT
//	};
//	virtual void Calculation(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix scalem, DirectX::SimpleMath::Matrix rotm, DirectX::SimpleMath::Matrix transm) {};
//};
//
//
//class SCALEM_ROTOMAT_TRANSMAT :public WorldMatrixOrder {
//	void Calculation(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix scalem, DirectX::SimpleMath::Matrix rotm, DirectX::SimpleMath::Matrix transm) override {
//		world = scalem * rotm * transm;
//	}
//};
//
//class SCALEM_TRANSMAT_ROTOMAT :public WorldMatrixOrder {
//	void Calculation(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix scalem, DirectX::SimpleMath::Matrix rotm, DirectX::SimpleMath::Matrix transm) override {
//		world = scalem * transm * rotm;
//	}
//};
//
//class TRANSMAT_ROTOMAT_SCALEM :public WorldMatrixOrder {
//	void Calculation(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix scalem, DirectX::SimpleMath::Matrix rotm, DirectX::SimpleMath::Matrix transm) override {
//		world = scalem * transm * rotm;
//	}
//};
//
//class TRANSMAT_SCALEM_ROTOMAT :public WorldMatrixOrder {
//	void Calculation(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix scalem, DirectX::SimpleMath::Matrix rotm, DirectX::SimpleMath::Matrix transm) override {
//		world = transm * scalem * rotm;
//	}
//};
//
//class ROTOMAT_TRANSMAT_SCALEM :public WorldMatrixOrder {
//	void Calculation(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix scalem, DirectX::SimpleMath::Matrix rotm, DirectX::SimpleMath::Matrix transm) override {
//		world = rotm * transm * scalem;
//	}
//};
//
//class ROTOMAT_SCALEM_TRANSMAT :public WorldMatrixOrder {
//	void Calculation(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix scalem, DirectX::SimpleMath::Matrix rotm, DirectX::SimpleMath::Matrix transm) override {
//		world = rotm * transm * scalem;
//	}
//};
//
//class WorldMatrixOrderFactory {
//public:
//	std::unique_ptr<WorldMatrixOrder> Set(WorldMatrixOrder::ORDER ORDER) {
//		switch (ORDER)
//		{
//		case WorldMatrixOrder::ORDER::SCALEM_ROTOMAT_TRANSMAT:
//			order = std::make_unique<SCALEM_ROTOMAT_TRANSMAT>();
//			break;
//		case WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT:
//			order = std::make_unique<SCALEM_TRANSMAT_ROTOMAT>();
//			break;
//		case WorldMatrixOrder::ORDER::TRANSMAT_ROTOMAT_SCALEM:
//			order = std::make_unique<TRANSMAT_ROTOMAT_SCALEM>();
//			break;
//		case WorldMatrixOrder::ORDER::TRANSMAT_SCALEM_ROTOMAT:
//			order = std::make_unique<TRANSMAT_SCALEM_ROTOMAT>();
//			break;
//		case WorldMatrixOrder::ORDER::ROTOMAT_TRANSMAT_SCALEM:
//			order = std::make_unique<ROTOMAT_TRANSMAT_SCALEM>();
//			break;
//		case WorldMatrixOrder::ORDER::ROTOMAT_SCALEM_TRANSMAT:
//			order = std::make_unique<ROTOMAT_SCALEM_TRANSMAT>();
//			break;
//		default:
//			break;
//		}
//		return move(order);
//	}
//private:
//	std::unique_ptr<WorldMatrixOrder> order;
//};
//
//
//
//
//// 3D�I�u�W�F�N�g
//class Obj3d
//{
//	/// <summary>
//	/// static member
//	/// </summary>
//public:
//	static const std::wstring RESOURCE_DIRECTORY;
////	static const std::wstring RESOURCE_EXT;
//
//	// �ݒ�
//	struct CommonDef
//	{
//		ID3D11Device* pDevice;
//		ID3D11DeviceContext* pDeviceContext;
//		Camera* pCamera;
//
//		CommonDef()
//		{
//			pDevice = nullptr;
//			pDeviceContext = nullptr;
//			pCamera = nullptr;
//		}
//	};
//	// ���p�f�[�^
//	struct Common
//	{
//		ID3D11Device*	device;
//		// �f�o�C�X�R���e�L�X�g�ւ̃|�C���^
//		ID3D11DeviceContext*	deviceContext;
//		// �`��X�e�[�g�ւ̃|�C���^
//		std::unique_ptr<DirectX::CommonStates>	states;
//		// ���p�̃G�t�F�N�g�t�@�N�g��
//		std::unique_ptr<DirectX::EffectFactory>	effectFactory;
//		// ���p�̃J�����i�`�掞�Ɏg�p�j
//		Camera* camera;
//		// �ǂݍ��ݍς݃��f���R���e�i
//		std::map<std::wstring, std::unique_ptr<DirectX::Model>> modelarray;
//		// ���Z�`��X�e�[�g
//		ID3D11BlendState* blendStateSubtract;
//	};
//	// �ÓI�����o�֐�
//	// �V�X�e��������
//	static void InitializeCommon(CommonDef def);
//	// �S���f���j��
//	static void UnloadAll();
//
//	// ���Z�`��ݒ���Z�b�g
//	static void SetSubtractive();
//
//	//getter
//	static Camera* GetCamera() { return s_Common.camera; }
//
//private:
//	// �ÓI�����o�ϐ��i�S�I�u�W�F�N�g�ŋ��L�j
//	static Common s_Common;
//
//	/// <summary>
//	/// non-static member
//	/// </summary>
//public:
//	Obj3d();
//	// �t�@�C�����烂�f����ǂݍ���
//	void LoadModel(const wchar_t*filename = nullptr);
//	// �s��̍X�V
//	void Update();
//	// �`��
//	void Draw();
//	// ���Z�`��ł̕`��i�e�p�j
//	void DrawSubtractive();
//	void DrawBillboard();
//	void DrawBillboardConstrainY();
//	// �I�u�W�F�N�g�̃��C�e�B���O�𖳌��ɂ���
//	void DisableLighting();
//
//	void EnableAlpha();
//
//	//setter
//	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans; }
//	void SetRot(const DirectX::SimpleMath::Vector3& rot) { m_Rot = rot; m_UseQuternion = false; }
//	void SetRotQ(const DirectX::SimpleMath::Quaternion& rotq) { m_RotQ = rotq; m_UseQuternion = true; }
//	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_Scale = scale; }
//	void SetLocalWorld(const DirectX::SimpleMath::Matrix& mat) { m_World = mat; }
//	void SetObjParent(Obj3d* pParent) { m_pParent = pParent; }
//	void SetOrder(std::unique_ptr<WorldMatrixOrder>& order) { m_order = std::move(order); }
//	void ChangeOrder(WorldMatrixOrder::ORDER order);//���[���h�s��̊|���Z�����ւ���
//	//getter
//	const DirectX::SimpleMath::Vector3& GetTrans() { return m_Trans; }
//	const DirectX::SimpleMath::Vector3& GetRot() { return m_Rot; }
//	const DirectX::SimpleMath::Quaternion& GetRotQ() { return m_RotQ; }
//	const DirectX::SimpleMath::Vector3& GetScale() { return m_Scale; }
//	const DirectX::SimpleMath::Matrix& GetLocalWorld() { return m_World; }
//	const std::unique_ptr<WorldMatrixOrder>& GetOrder() { return m_order; }
//	WorldMatrixOrderFactory& GetWorldMatrixOrderFactory() { return WorldMatrixOrderFactory; }
//
//private:
//	// ���f���f�[�^�ւ̃|�C���^
//	const DirectX::Model* m_pModel;
//	// ���s�ړ�
//	DirectX::SimpleMath::Vector3 m_Trans;
//	// ��]
//	bool m_UseQuternion;
//	DirectX::SimpleMath::Vector3 m_Rot;
//	DirectX::SimpleMath::Quaternion m_RotQ;
//	// �X�P�[�����O
//	DirectX::SimpleMath::Vector3 m_Scale;
//	// ���[���h�s��
//	DirectX::SimpleMath::Matrix m_World;
//	// �e�̃��[���h�s��ւ̃|�C���^
//	Obj3d* m_pParent;
//	// ���[���h�s��̊|���Z����(�t�@�N�g���[���]�b�g)
//	std::unique_ptr<WorldMatrixOrder> m_order;
//	//���[���h�s��̊|���Z�����t�@�N�g���[
//	WorldMatrixOrderFactory WorldMatrixOrderFactory;
//};
//
