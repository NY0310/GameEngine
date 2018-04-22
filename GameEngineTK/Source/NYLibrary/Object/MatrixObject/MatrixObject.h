/// <summary>
/// オブジェクトの行列クラス
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
		//コンストラクタ
		MatrixObject();
		//デストラクタ
		virtual ~MatrixObject() = default;

		//初期化
		void MatrixInitialize();
		//ワールド行列作成
		void Calc();
		//親のワールド行列に影響を与える
		void Calc(const D3DXMATRIX & worldMatrix);



		//ワールド座標設定
		virtual void SetPosition(const D3DXVECTOR3& position) { transfer = position; isCalcTransferMatrix = true; }
		//ワールド座標取得
		virtual const D3DXVECTOR3& GetPosition() { return transfer; }
		//ワールド座標X設定
		virtual void SetPositionX(float positionX) { transfer.x = positionX; isCalcTransferMatrix = true;}
		//ワールド座標X取得
		virtual float GetPositionX() { return transfer.x; }
		//ワールド座標Y設定
		virtual void SetPositionY(float positionY) { transfer.y = positionY; isCalcTransferMatrix = true; }
		//ワールド座標Y取得
		virtual float GetPositionY() { return transfer.y;}
		//ワールド座標Z設定
		virtual void SetPositionZ(float positionZ) { transfer.z = positionZ; isCalcTransferMatrix = true;}
		//ワールド座標Z取得
		virtual float GetPositionZ() { return transfer.z; }




		//スケールXYZ設定
		virtual void SetScale(const D3DXVECTOR3& scale) { this->scale = scale; isCalcScaleMatrix = true;}
		//スケールXYZ取得
		virtual const D3DXVECTOR3& GetScale() { return  this->scale; }
		//スケールXYZ設定
		virtual void SetScale(const float scale) { this->scale.x = scale; this->scale.y = scale; this->scale.z = scale; isCalcScaleMatrix = true;}
		//スケールX設定
		virtual void SetScaleX(float scaleX) { this->scale.x = scaleX; isCalcScaleMatrix = true;}
		//スケールX取得
		virtual float GetScaleX() { return  this->scale.x; }
		//スケールY設定
		virtual void SetScaleY(float scaleY) { this->scale.y = scaleY; isCalcScaleMatrix = true;}
		//スケールY取得
		virtual float GetScaleY() { return  this->scale.y; }
		//スケールZ設定
		virtual void SetScaleZ(float scaleZ) { this->scale.z = scaleZ; isCalcScaleMatrix = true;}
		//スケールZ取得
		virtual float GetScaleZ() { return  this->scale.z; }



		//オイラー角設定
		virtual void SetRotation(const D3DXVECTOR3& eulerAangle) { this->eulerAangle = eulerAangle; isUseQuternion = false;  isCalcRotationMatrix = true; }
		//オイラー角取得
		virtual const D3DXVECTOR3& GetRotation() { return  this->eulerAangle; }
		//オイラー角X設定
		virtual void SetRotationX(float eulerAangleX) { this->eulerAangle.x = eulerAangleX; isUseQuternion = false; isCalcRotationMatrix = true;}
		//オイラー角X取得
		virtual const float GetRotationX() { return  this->eulerAangle.x; }
		//オイラー角Y設定
		virtual void SetRotationY(const float eulerAangleY) { this->eulerAangle.y = eulerAangleY; isUseQuternion = false; isCalcRotationMatrix = true;}
		//オイラー角Y取得
		virtual float GetRotationY() { return  this->eulerAangle.y; }
		//オイラー角Z設定
		virtual void SetRotationZ(float eulerAangleZ) { this->eulerAangle.z = eulerAangle.z; isUseQuternion = false; isCalcRotationMatrix = true;}
		//オイラー角Z取得
		virtual float GetRotationZ() { return  this->eulerAangle.z; }


		//クォータニオン設定
		virtual void SetQuaternion(const D3DXQUATERNION& quaternion) { this->quaternion = quaternion; isUseQuternion = true; isCalcRotationMatrix = true;}
		//クォータニオン取得
		virtual const D3DXQUATERNION& GetQuaternion() { return  this->quaternion; }


		//ワールド行列取得
		const D3DXMATRIX& GetWorldMatrix() { return worldMatrix; }
		//ワールド・ビュー・プロジェクション行列
		const D3DXMATRIX& GetWVP() { return wvp; }
		//移動行列設定
		void SetTransferMatrix(const D3DXMATRIX& transMatrix) { transferMatrix = transMatrix; isCalcTransferMatrix = false; }
		//移動行列取得
		const D3DXMATRIX& GetTransferMatrix() { return this->transferMatrix; }
		//回転行列設定
		void SetRotationMatrix(const D3DXMATRIX& rotationMatrix) { this->rotationMatrix = rotationMatrix; isCalcRotationMatrix = false; }
		//回転行列取得
		const D3DXMATRIX& GetRotationMatrix() { return this->rotationMatrix; }
		//拡大行列設定
		void SetScaleMatrix(const D3DXMATRIX& scaleMatrix) { this->scaleMatrix = scaleMatrix; isCalcRotationMatrix = false; }
		//拡大行列取得
		const D3DXMATRIX& GetScaleMarix() { return scaleMatrix; }


		//クォータニオンを使用するか
		void SetIsUseQuternion(bool isUseQuternion) { this->isUseQuternion = isUseQuternion; }
		//クォータニオンを使用するか取得する
		bool GetIsUseQuternion() { return isUseQuternion; }


		//移動行列計算を行うか設定
		void SetIsCalcTransferMatrix(bool isCalcTransferMatrix) { this->isCalcTransferMatrix = isCalcTransferMatrix; }
		//移動行列計算を行うか取得
		bool GetIsCalcTransferMatrix() { this->isCalcTransferMatrix; }
		//拡大行列計算を行うか設定
		void SetIsCalcScaleMatrix(bool isCalcScaleMatrix) { this->isCalcScaleMatrix = isCalcScaleMatrix; }
		//拡大行列計算を行うか取得
		bool GetIsCalcScaleMatrix() { return  isCalcScaleMatrix; }
		//回転行列計算を行うか設定
		void SetIsCalcRotationMatrix(bool isCalcRotationMatrix) { this->isCalcRotationMatrix = isCalcRotationMatrix; }
		//回転行列計算を行うか取得
		bool GetIsCalcRotationMatrix() {return isCalcRotationMatrix; }


		//ワールド行列の掛け算順序を設定する
		void ChangeOrder(WorldMatrixOrder::ORDER order);

		//ビルボードにするか設定
		void SetIsBillBoard(bool isBillBoard) {
			this->isBillBoard = isBillBoard;
		}
		//ビルボードにするか取得
		bool SetIsBillBoard() {
			return isBillBoard;
		}

		//親に影響されるか設定
		void SetParantMatrixInfluence(bool isParantMatrixInfluence) {
			this->isParantMatrixInfluence = isParantMatrixInfluence;
		}
		//親に影響されるか取得
		bool GetParantMatrixInfluence() {
			return isParantMatrixInfluence;
		}
	private:
		//全行列作成
		void CalcAllMatrix();
		//ワールド行列作成
		void CalcWorldMatrix();
		void CalcWVP();
		//移動行列作成
		void CalcTransferMatrix();
		//スケール行列作成
		void CalcScaleMatrix();
		//回転行列作成
		void CalcRotationMatrix();
	private:
		D3DXMATRIX worldMatrix;//ワールド行列
		D3DXVECTOR3 transfer;//移動
		D3DXVECTOR3 scale;//サイズ
		float localSize;//ローカルでのサイズ
		D3DXVECTOR3 eulerAangle;//回転
		D3DXQUATERNION quaternion;//クォータニオン
		D3DXMATRIX transferMatrix;//移動行列
		D3DXMATRIX scaleMatrix;//スケール行列
		D3DXMATRIX rotationMatrix;//回転行列
		D3DXMATRIX wvp;//ワールドビュープロジェクション行列
		bool isUseQuternion;//クォータニオンを使用するか
		std::weak_ptr<WorldMatrixOrder> order;// ワールド行列の掛け算順序(ファクトリーメソッド)
		WorldMatrixOrderFactory* worldMatrixOrderFactory;//ワールド行列の掛け算順序ファクトリー
		bool isCalcTransferMatrix;//移動行列計算を行うか
		bool isCalcScaleMatrix;//拡大行列計算を行うか
		bool isCalcRotationMatrix;//回転行列計算を行うか
		bool isBillBoard;//ブルボードを使用するか
		bool isParantMatrixInfluence;//親に影響されるか
	};
};