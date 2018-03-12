/// <summary>
/// オブジェクトの行列クラス
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
		//コンストラクタ
		MatrixObject();
		//デストラクタ
		virtual ~MatrixObject() = default;


		//ワールド行列作成
		void Calc();



		//ワールド座標設定
		virtual void SetPosition(const D3DXVECTOR3& position) { transfer = position; }
		//ワールド座標取得
		virtual const D3DXVECTOR3& GetPosition() { return transfer; }
		//ワールド座標設定
		virtual void SetPositionX(float positionX) { transfer.x = positionX; }
		//ワールド座標取得
		virtual float GetPositionX() { return transfer.x; }
		//ワールド座標設定
		virtual void SetPositionY(float positionY) { transfer.y = positionY; }
		//ワールド座標取得
		virtual float GetPositionY() { return transfer.y; }
		//ワールド座標設定
		virtual void SetPositionZ(float positionZ) { transfer.z = positionZ; }
		//ワールド座標取得
		virtual float GetPositionZ() { return transfer.z; }




		//スケール設定
		virtual void SetScale(const D3DXVECTOR3& scale) { this->scale = scale; }
		//スケール取得
		virtual const D3DXVECTOR3& GetScale() { return  this->scale; }
		virtual void SetScale(const float scale) { this->scale.x = scale; this->scale.y = scale; this->scale.z = scale; }
		//スケール設定
		virtual void SetScaleX(float scaleX) { this->scale.x = scaleX; }
		//スケール取得
		virtual float GetScaleX() { return  this->scale.x; }
		//スケール設定
		virtual void SetScaleY(float scaleY) { this->scale.y = scaleY; }
		//スケール取得
		virtual float GetScaleY() { return  this->scale.y; }
		//スケール設定
		virtual void SetScaleZ(float scaleZ) { this->scale.z = scaleZ; }
		//スケール取得
		virtual float GetScaleZ() { return  this->scale.z; }



		//オイラー角設定
		virtual void SetRotation(const D3DXVECTOR3& eulerAangle) { this->eulerAangle = eulerAangle; IsUseQuternion = false; }
		//オイラー角取得
		virtual const D3DXVECTOR3& GetRotation() { return  this->eulerAangle; }
		//オイラー角設定
		virtual void SetRotationX(float eulerAangleX) { this->eulerAangle.x = eulerAangleX; IsUseQuternion = false; }
		//オイラー角取得
		virtual const float GetRotationX() { return  this->eulerAangle.x; }
		//オイラー角設定
		virtual void SetRotationY(const float eulerAangleY) { this->eulerAangle.y = eulerAangleY; IsUseQuternion = false; }
		//オイラー角取得
		virtual float GetRotationY() { return  this->eulerAangle.y; }
		//オイラー角設定
		virtual void SetRotationZ(float eulerAangleZ) { this->eulerAangle.z = eulerAangle.z; IsUseQuternion = false; }
		//オイラー角取得
		virtual float GetRotationZ() { return  this->eulerAangle.z; }


		//クォータニオン設定
		virtual void SetQuaternion(const D3DXQUATERNION& quaternion) { this->quaternion = quaternion; IsUseQuternion = true; }
		//クォータニオン取得
		virtual const D3DXQUATERNION& GetQuaternion() { return  this->quaternion; }
		////ワールド座標設定
		//virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) { transfer = Math::VectorToD3DXVECTOR3(position); }
		////ワールド座標取得
		//virtual DirectX::SimpleMath::Vector3& GetPositionMath() { return Math::D3DXVECTOR3ToVector(transfer); }
		////スケール設定
		//virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) { this->scale = Math::VectorToD3DXVECTOR3(scale); }
		////スケール取得
		//virtual const DirectX::SimpleMath::Vector3& GetScaleMath() { return Math::D3DXVECTOR3ToVector(this->scale); }
		////回転設定
		//virtual void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { this->eulerAangle = Math::VectorToD3DXVECTOR3(rotation); }
		////回転取得
		//virtual const DirectX::SimpleMath::Vector3& GetRotationMath() { return Math::D3DXVECTOR3ToVector(this->eulerAangle); IsUseQuternion = false; }
		////クォータニオン設定
		//virtual void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { this->quaternion = Math::QuaterniontoD3DXQUATERNION(quaternion); IsUseQuternion = true; }
		////クォータニオン取得
		//virtual const DirectX::SimpleMath::Quaternion& GetQuaternionMath() { return  Math::D3DXQUATERNIONtoQuaternion(this->quaternion); }
		//ワールド行列取得
		const D3DXMATRIX& GetWorldMatrix() { return worldMatrix; }
		//ワールド・ビュー・プロジェクション行列
		const D3DXMATRIX& GetWVP() { return wvp; }
		//移動行列取得
		const D3DXMATRIX& GetTransferMatrix() { return this->transferMatrix; }
		//回転行列取得
		const D3DXMATRIX& GetRotationMatrix() { return this->rotationMatrix; }

		void SetOrder(std::unique_ptr<WorldMatrixOrder>& order) { this->order = std::move(order); }
		void ChangeOrder(WorldMatrixOrder::ORDER order);//ワールド行列の掛け算を入れ替える

	private:
		//全行列作成
		void CreateAllMatrix();
		//ワールド行列作成
		void CreateTransferMatrix();
		//スケール行列作成
		void CreateScaleMatrix();
		//回転行列作成
		void CreateRotationMatrix();
	private:
		D3DXMATRIX worldMatrix;//ワールド行列
		D3DXVECTOR3 transfer;//移動量
		D3DXVECTOR3 scale;//サイズ
		D3DXVECTOR3 eulerAangle;//回転
		D3DXQUATERNION quaternion;//クォータニオン
		D3DXMATRIX transferMatrix;//移動行列
		D3DXMATRIX scaleMatrix;//スケール行列
		D3DXMATRIX rotationMatrix;//回転行列
		D3DXMATRIX wvp;//ワールドビュープロジェクション行列
		bool IsUseQuternion = false;//クォータニオンを使用するか
		// ワールド行列の掛け算順序(ファクトリーメゾット)
		std::unique_ptr<WorldMatrixOrder> order;
		//ワールド行列の掛け算順序ファクトリー
		WorldMatrixOrderFactory worldMatrixOrderFactory;


	};
};