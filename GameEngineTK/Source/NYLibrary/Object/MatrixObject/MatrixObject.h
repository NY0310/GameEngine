/// <summary>
/// �I�u�W�F�N�g�̍s��N���X
/// </summary>
#pragma once
#include "../../Camera/FollowCamera.h"
#include "../Object.h"

namespace NYLibrary
{
	class WorldMatrixOrder {
	public:
		enum ORDER {
			SCALEMAT_ROTOMAT_TRANSMAT,
			SCALEMAT_TRANSMAT_ROTOMAT,
			TRANSMAT_ROTOMAT_SCALEM,
			TRANSMAT_SCALEMAT_ROTOMAT,
			ROTOMAT_TRANSMAT_SCALEMAT,
			ROTOMAT_SCALEMAT_TRANSMAT
		};
		virtual void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) {};
	};


	class SCALEM_ROTOMAT_TRANSMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = scalem * rotm;
			world._41 = transm._41;
			world._42 = transm._42;
			world._43 = transm._43;

		}
	};

	class SCALEMAT_TRANSMAT_ROTOMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = scalem * transm * rotm;
		}
	};

	class TRANSMAT_ROTOMAT_SCALEMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = transm * rotm * scalem;
		}
	};

	class TRANSMAT_SCALEMAT_ROTOMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = transm * scalem * rotm;
		}
	};

	class ROTOMAT_TRANSMAT_SCALEMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = rotm * transm * scalem;
		}
	};

	class ROTOMAT_SCALEMAT_TRANSMAT :public WorldMatrixOrder {
		void Calculation(D3DXMATRIX& world, D3DXMATRIX scalem, D3DXMATRIX rotm, D3DXMATRIX transm) override {
			world = rotm * scalem;
			world._41 = transm._41;
			world._42 = transm._42;
			world._43 = transm._43;
		}
	};

	class WorldMatrixOrderFactory {
	public:
		std::weak_ptr<WorldMatrixOrder> Set(WorldMatrixOrder::ORDER ORDER) {
			switch (ORDER)
			{
			case WorldMatrixOrder::ORDER::SCALEMAT_ROTOMAT_TRANSMAT:
				order = std::make_unique<SCALEM_ROTOMAT_TRANSMAT>();
				break;
			case WorldMatrixOrder::ORDER::SCALEMAT_TRANSMAT_ROTOMAT:
				order = std::make_unique<SCALEMAT_TRANSMAT_ROTOMAT>();
				break;
			case WorldMatrixOrder::ORDER::TRANSMAT_ROTOMAT_SCALEM:
				order = std::make_unique<TRANSMAT_ROTOMAT_SCALEMAT>();
				break;
			case WorldMatrixOrder::ORDER::TRANSMAT_SCALEMAT_ROTOMAT:
				order = std::make_unique<TRANSMAT_SCALEMAT_ROTOMAT>();
				break;
			case WorldMatrixOrder::ORDER::ROTOMAT_TRANSMAT_SCALEMAT:
				order = std::make_unique<ROTOMAT_TRANSMAT_SCALEMAT>();
				break;
			case WorldMatrixOrder::ORDER::ROTOMAT_SCALEMAT_TRANSMAT:
				order = std::make_unique<ROTOMAT_SCALEMAT_TRANSMAT>();
				break;
			default:
				break;
			}
			return order;
		}
	private:
		std::shared_ptr<WorldMatrixOrder> order;
	};

	class MatrixObject : virtual public Object{
	public:
		//�R���X�g���N�^
		MatrixObject();
		//�f�X�g���N�^
		virtual ~MatrixObject() = default;

		//������
		void MatrixInitialize();
		//���[���h�s��쐬
		void Calc();
		//�e�̃��[���h�s��ɉe����^����
		void Calc(const D3DXMATRIX & worldMatrix);



		//���[���h���W�ݒ�
		virtual void SetPosition(const D3DXVECTOR3& position) { transfer = position; isCalcTransferMatrix = true; }
		//���[���h���W�擾
		virtual const D3DXVECTOR3& GetPosition() { return transfer; }
		//���[���h���WX�ݒ�
		virtual void SetPositionX(float positionX) { transfer.x = positionX; isCalcTransferMatrix = true;}
		//���[���h���WX�擾
		virtual float GetPositionX() { return transfer.x; }
		//���[���h���WY�ݒ�
		virtual void SetPositionY(float positionY) { transfer.y = positionY; isCalcTransferMatrix = true; }
		//���[���h���WY�擾
		virtual float GetPositionY() { return transfer.y;}
		//���[���h���WZ�ݒ�
		virtual void SetPositionZ(float positionZ) { transfer.z = positionZ; isCalcTransferMatrix = true;}
		//���[���h���WZ�擾
		virtual float GetPositionZ() { return transfer.z; }




		//�X�P�[��XYZ�ݒ�
		virtual void SetScale(const D3DXVECTOR3& scale) { this->scale = scale; isCalcScaleMatrix = true;}
		//�X�P�[��XYZ�擾
		virtual const D3DXVECTOR3& GetScale() { return  this->scale; }
		//�X�P�[��XYZ�ݒ�
		virtual void SetScale(const float scale) { this->scale.x = scale; this->scale.y = scale; this->scale.z = scale; isCalcScaleMatrix = true;}
		//�X�P�[��X�ݒ�
		virtual void SetScaleX(float scaleX) { this->scale.x = scaleX; isCalcScaleMatrix = true;}
		//�X�P�[��X�擾
		virtual float GetScaleX() { return  this->scale.x; }
		//�X�P�[��Y�ݒ�
		virtual void SetScaleY(float scaleY) { this->scale.y = scaleY; isCalcScaleMatrix = true;}
		//�X�P�[��Y�擾
		virtual float GetScaleY() { return  this->scale.y; }
		//�X�P�[��Z�ݒ�
		virtual void SetScaleZ(float scaleZ) { this->scale.z = scaleZ; isCalcScaleMatrix = true;}
		//�X�P�[��Z�擾
		virtual float GetScaleZ() { return  this->scale.z; }



		//�I�C���[�p�ݒ�
		virtual void SetRotation(const D3DXVECTOR3& eulerAangle) { this->eulerAangle = eulerAangle; isUseQuternion = false;  isCalcRotationMatrix = true; }
		//�I�C���[�p�擾
		virtual const D3DXVECTOR3& GetRotation() { return  this->eulerAangle; }
		//�I�C���[�pX�ݒ�
		virtual void SetRotationX(float eulerAangleX) { this->eulerAangle.x = eulerAangleX; isUseQuternion = false; isCalcRotationMatrix = true;}
		//�I�C���[�pX�擾
		virtual const float GetRotationX() { return  this->eulerAangle.x; }
		//�I�C���[�pY�ݒ�
		virtual void SetRotationY(const float eulerAangleY) { this->eulerAangle.y = eulerAangleY; isUseQuternion = false; isCalcRotationMatrix = true;}
		//�I�C���[�pY�擾
		virtual float GetRotationY() { return  this->eulerAangle.y; }
		//�I�C���[�pZ�ݒ�
		virtual void SetRotationZ(float eulerAangleZ) { this->eulerAangle.z = eulerAangle.z; isUseQuternion = false; isCalcRotationMatrix = true;}
		//�I�C���[�pZ�擾
		virtual float GetRotationZ() { return  this->eulerAangle.z; }


		//�N�H�[�^�j�I���ݒ�
		virtual void SetQuaternion(const D3DXQUATERNION& quaternion) { this->quaternion = quaternion; isUseQuternion = true; isCalcRotationMatrix = true;}
		//�N�H�[�^�j�I���擾
		virtual const D3DXQUATERNION& GetQuaternion() { return  this->quaternion; }


		//���[���h�s��擾
		const D3DXMATRIX& GetWorldMatrix() { return worldMatrix; }
		//���[���h�E�r���[�E�v���W�F�N�V�����s��
		const D3DXMATRIX& GetWVP() { return wvp; }
		//�ړ��s��ݒ�
		void SetTransferMatrix(const D3DXMATRIX& transMatrix) { transferMatrix = transMatrix; isCalcTransferMatrix = false; }
		//�ړ��s��擾
		const D3DXMATRIX& GetTransferMatrix() { return this->transferMatrix; }
		//��]�s��ݒ�
		void SetRotationMatrix(const D3DXMATRIX& rotationMatrix) { this->rotationMatrix = rotationMatrix; isCalcRotationMatrix = false; }
		//��]�s��擾
		const D3DXMATRIX& GetRotationMatrix() { return this->rotationMatrix; }
		//�g��s��ݒ�
		void SetScaleMatrix(const D3DXMATRIX& scaleMatrix) { this->scaleMatrix = scaleMatrix; isCalcRotationMatrix = false; }
		//�g��s��擾
		const D3DXMATRIX& GetScaleMarix() { return scaleMatrix; }


		//�N�H�[�^�j�I�����g�p���邩
		void SetIsUseQuternion(bool isUseQuternion) { this->isUseQuternion = isUseQuternion; }
		//�N�H�[�^�j�I�����g�p���邩�擾����
		bool GetIsUseQuternion() { return isUseQuternion; }


		//�ړ��s��v�Z���s�����ݒ�
		void SetIsCalcTransferMatrix(bool isCalcTransferMatrix) { this->isCalcTransferMatrix = isCalcTransferMatrix; }
		//�ړ��s��v�Z���s�����擾
		bool GetIsCalcTransferMatrix() { this->isCalcTransferMatrix; }
		//�g��s��v�Z���s�����ݒ�
		void SetIsCalcScaleMatrix(bool isCalcScaleMatrix) { this->isCalcScaleMatrix = isCalcScaleMatrix; }
		//�g��s��v�Z���s�����擾
		bool GetIsCalcScaleMatrix() { return  isCalcScaleMatrix; }
		//��]�s��v�Z���s�����ݒ�
		void SetIsCalcRotationMatrix(bool isCalcRotationMatrix) { this->isCalcRotationMatrix = isCalcRotationMatrix; }
		//��]�s��v�Z���s�����擾
		bool GetIsCalcRotationMatrix() {return isCalcRotationMatrix; }


		//���[���h�s��̊|���Z������ݒ肷��
		void ChangeOrder(WorldMatrixOrder::ORDER order);

		//�r���{�[�h�ɂ��邩�ݒ�
		void SetIsBillBoard(bool isBillBoard) {
			this->isBillBoard = isBillBoard;
		}
		//�r���{�[�h�ɂ��邩�擾
		bool SetIsBillBoard() {
			return isBillBoard;
		}

		//�e�ɉe������邩�ݒ�
		void SetParantMatrixInfluence(bool isParantMatrixInfluence) {
			this->isParantMatrixInfluence = isParantMatrixInfluence;
		}
		//�e�ɉe������邩�擾
		bool GetParantMatrixInfluence() {
			return isParantMatrixInfluence;
		}
	private:
		//�S�s��쐬
		void CalcAllMatrix();
		//���[���h�s��쐬
		void CalcWorldMatrix();
		void CalcWVP();
		//�ړ��s��쐬
		void CalcTransferMatrix();
		//�X�P�[���s��쐬
		void CalcScaleMatrix();
		//��]�s��쐬
		void CalcRotationMatrix();
	private:
		D3DXMATRIX worldMatrix;//���[���h�s��
		D3DXVECTOR3 transfer;//�ړ�
		D3DXVECTOR3 scale;//�T�C�Y
		float localSize;//���[�J���ł̃T�C�Y
		D3DXVECTOR3 eulerAangle;//��]
		D3DXQUATERNION quaternion;//�N�H�[�^�j�I��
		D3DXMATRIX transferMatrix;//�ړ��s��
		D3DXMATRIX scaleMatrix;//�X�P�[���s��
		D3DXMATRIX rotationMatrix;//��]�s��
		D3DXMATRIX wvp;//���[���h�r���[�v���W�F�N�V�����s��
		bool isUseQuternion;//�N�H�[�^�j�I�����g�p���邩
		std::weak_ptr<WorldMatrixOrder> order;// ���[���h�s��̊|���Z����(�t�@�N�g���[���\�b�h)
		WorldMatrixOrderFactory* worldMatrixOrderFactory;//���[���h�s��̊|���Z�����t�@�N�g���[
		bool isCalcTransferMatrix;//�ړ��s��v�Z���s����
		bool isCalcScaleMatrix;//�g��s��v�Z���s����
		bool isCalcRotationMatrix;//��]�s��v�Z���s����
		bool isBillBoard;//�u���{�[�h���g�p���邩
		bool isParantMatrixInfluence;//�e�ɉe������邩
	};
};