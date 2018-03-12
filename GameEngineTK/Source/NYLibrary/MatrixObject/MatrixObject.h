/// <summary>
/// �I�u�W�F�N�g�̍s��N���X
/// </summary>
#pragma once
//#include <d3d11_1.h>
//#include <SimpleMath.h>
#include "../Math/Math.h"
#include "../Camera/FollowCamera.h"

namespace NYLibrary
{
	class WorldMatrixOrder {
	public:
		enum ORDER {
			SCALEM_ROTOMAT_TRANSMAT,
			SCALEM_TRANSMAT_ROTOMAT,
			TRANSMAT_ROTOMAT_SCALEM,
			TRANSMAT_SCALEM_ROTOMAT,
			ROTOMAT_TRANSMAT_SCALEM,
			ROTOMAT_SCALEM_TRANSMAT
		};
		virtual void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) {};
	};


	class SCALEM_ROTOMAT_TRANSMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = scalem * rotm * transm;
		}
	};

	class SCALEM_TRANSMAT_ROTOMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = scalem * transm * rotm;
		}
	};

	class TRANSMAT_ROTOMAT_SCALEM :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = scalem * transm * rotm;
		}
	};

	class TRANSMAT_SCALEM_ROTOMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = transm * scalem * rotm;
		}
	};

	class ROTOMAT_TRANSMAT_SCALEM :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = rotm * transm * scalem;
		}
	};

	class ROTOMAT_SCALEM_TRANSMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = rotm * transm * scalem;
		}
	};

	class WorldMatrixOrderFactory {
	public:
		std::unique_ptr<WorldMatrixOrder> Set(WorldMatrixOrder::ORDER ORDER) {
			switch (ORDER)
			{
			case WorldMatrixOrder::ORDER::SCALEM_ROTOMAT_TRANSMAT:
				order = std::make_unique<SCALEM_ROTOMAT_TRANSMAT>();
				break;
			case WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT:
				order = std::make_unique<SCALEM_TRANSMAT_ROTOMAT>();
				break;
			case WorldMatrixOrder::ORDER::TRANSMAT_ROTOMAT_SCALEM:
				order = std::make_unique<TRANSMAT_ROTOMAT_SCALEM>();
				break;
			case WorldMatrixOrder::ORDER::TRANSMAT_SCALEM_ROTOMAT:
				order = std::make_unique<TRANSMAT_SCALEM_ROTOMAT>();
				break;
			case WorldMatrixOrder::ORDER::ROTOMAT_TRANSMAT_SCALEM:
				order = std::make_unique<ROTOMAT_TRANSMAT_SCALEM>();
				break;
			case WorldMatrixOrder::ORDER::ROTOMAT_SCALEM_TRANSMAT:
				order = std::make_unique<ROTOMAT_SCALEM_TRANSMAT>();
				break;
			default:
				break;
			}
			return move(order);
		}
	private:
		std::unique_ptr<WorldMatrixOrder> order;
	};


	class MatrixObject {
	public:
		//�R���X�g���N�^
		MatrixObject();
		//�f�X�g���N�^
		virtual ~MatrixObject() = default;


		//���[���h�s��쐬
		void Calc();



		//���[���h���W�ݒ�
		virtual void SetPosition(const D3DXVECTOR3& position) { transfer = position; }
		//���[���h���W�擾
		virtual const D3DXVECTOR3& GetPosition() { return transfer; }
		//���[���h���W�ݒ�
		virtual void SetPositionX(float positionX) { transfer.x = positionX; }
		//���[���h���W�擾
		virtual float GetPositionX() { return transfer.x; }
		//���[���h���W�ݒ�
		virtual void SetPositionY(float positionY) { transfer.y = positionY; }
		//���[���h���W�擾
		virtual float GetPositionY() { return transfer.y; }
		//���[���h���W�ݒ�
		virtual void SetPositionZ(float positionZ) { transfer.z = positionZ; }
		//���[���h���W�擾
		virtual float GetPositionZ() { return transfer.z; }




		//�X�P�[���ݒ�
		virtual void SetScale(const D3DXVECTOR3& scale) { this->scale = scale; }
		//�X�P�[���擾
		virtual const D3DXVECTOR3& GetScale() { return  this->scale; }
		virtual void SetScale(const float scale) { this->scale.x = scale; this->scale.y = scale; this->scale.z = scale; }
		//�X�P�[���ݒ�
		virtual void SetScaleX(float scaleX) { this->scale.x = scaleX; }
		//�X�P�[���擾
		virtual float GetScaleX() { return  this->scale.x; }
		//�X�P�[���ݒ�
		virtual void SetScaleY(float scaleY) { this->scale.y = scaleY; }
		//�X�P�[���擾
		virtual float GetScaleY() { return  this->scale.y; }
		//�X�P�[���ݒ�
		virtual void SetScaleZ(float scaleZ) { this->scale.z = scaleZ; }
		//�X�P�[���擾
		virtual float GetScaleZ() { return  this->scale.z; }



		//�I�C���[�p�ݒ�
		virtual void SetRotation(const D3DXVECTOR3& eulerAangle) { this->eulerAangle = eulerAangle; IsUseQuternion = false; }
		//�I�C���[�p�擾
		virtual const D3DXVECTOR3& GetRotation() { return  this->eulerAangle; }
		//�I�C���[�p�ݒ�
		virtual void SetRotationX(float eulerAangleX) { this->eulerAangle.x = eulerAangleX; IsUseQuternion = false; }
		//�I�C���[�p�擾
		virtual const float GetRotationX() { return  this->eulerAangle.x; }
		//�I�C���[�p�ݒ�
		virtual void SetRotationY(const float eulerAangleY) { this->eulerAangle.y = eulerAangleY; IsUseQuternion = false; }
		//�I�C���[�p�擾
		virtual float GetRotationY() { return  this->eulerAangle.y; }
		//�I�C���[�p�ݒ�
		virtual void SetRotationZ(float eulerAangleZ) { this->eulerAangle.z = eulerAangle.z; IsUseQuternion = false; }
		//�I�C���[�p�擾
		virtual float GetRotationZ() { return  this->eulerAangle.z; }


		//�N�H�[�^�j�I���ݒ�
		virtual void SetQuaternion(const D3DXQUATERNION& quaternion) { this->quaternion = quaternion; IsUseQuternion = true; }
		//�N�H�[�^�j�I���擾
		virtual const D3DXQUATERNION& GetQuaternion() { return  this->quaternion; }
		////���[���h���W�ݒ�
		//virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) { transfer = Math::VectorToD3DXVECTOR3(position); }
		////���[���h���W�擾
		//virtual DirectX::SimpleMath::Vector3& GetPositionMath() { return Math::D3DXVECTOR3ToVector(transfer); }
		////�X�P�[���ݒ�
		//virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) { this->scale = Math::VectorToD3DXVECTOR3(scale); }
		////�X�P�[���擾
		//virtual const DirectX::SimpleMath::Vector3& GetScaleMath() { return Math::D3DXVECTOR3ToVector(this->scale); }
		////��]�ݒ�
		//virtual void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { this->eulerAangle = Math::VectorToD3DXVECTOR3(rotation); }
		////��]�擾
		//virtual const DirectX::SimpleMath::Vector3& GetRotationMath() { return Math::D3DXVECTOR3ToVector(this->eulerAangle); IsUseQuternion = false; }
		////�N�H�[�^�j�I���ݒ�
		//virtual void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { this->quaternion = Math::QuaterniontoD3DXQUATERNION(quaternion); IsUseQuternion = true; }
		////�N�H�[�^�j�I���擾
		//virtual const DirectX::SimpleMath::Quaternion& GetQuaternionMath() { return  Math::D3DXQUATERNIONtoQuaternion(this->quaternion); }
		//���[���h�s��擾
		const D3DXMATRIX& GetWorldMatrix() { return worldMatrix; }
		//���[���h�E�r���[�E�v���W�F�N�V�����s��
		const D3DXMATRIX& GetWVP() { return wvp; }
		//�ړ��s��擾
		const D3DXMATRIX& GetTransferMatrix() { return this->transferMatrix; }
		//��]�s��擾
		const D3DXMATRIX& GetRotationMatrix() { return this->rotationMatrix; }

		void SetOrder(std::unique_ptr<WorldMatrixOrder>& order) { this->order = std::move(order); }
		void ChangeOrder(WorldMatrixOrder::ORDER order);//���[���h�s��̊|���Z�����ւ���

	private:
		//�S�s��쐬
		void CreateAllMatrix();
		//���[���h�s��쐬
		void CreateTransferMatrix();
		//�X�P�[���s��쐬
		void CreateScaleMatrix();
		//��]�s��쐬
		void CreateRotationMatrix();
	private:
		D3DXMATRIX worldMatrix;//���[���h�s��
		D3DXVECTOR3 transfer;//�ړ���
		D3DXVECTOR3 scale;//�T�C�Y
		D3DXVECTOR3 eulerAangle;//��]
		D3DXQUATERNION quaternion;//�N�H�[�^�j�I��
		D3DXMATRIX transferMatrix;//�ړ��s��
		D3DXMATRIX scaleMatrix;//�X�P�[���s��
		D3DXMATRIX rotationMatrix;//��]�s��
		D3DXMATRIX wvp;//���[���h�r���[�v���W�F�N�V�����s��
		bool IsUseQuternion = false;//�N�H�[�^�j�I�����g�p���邩
		// ���[���h�s��̊|���Z����(�t�@�N�g���[���]�b�g)
		std::unique_ptr<WorldMatrixOrder> order;
		//���[���h�s��̊|���Z�����t�@�N�g���[
		WorldMatrixOrderFactory worldMatrixOrderFactory;


	};
};