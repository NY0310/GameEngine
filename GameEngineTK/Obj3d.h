///// <summary>
///// ３Ｄオブジェクトクラス
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
//// 3Dオブジェクト
//class Obj3d
//{
//	/// <summary>
//	/// static member
//	/// </summary>
//public:
//	static const std::wstring RESOURCE_DIRECTORY;
////	static const std::wstring RESOURCE_EXT;
//
//	// 設定
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
//	// 共用データ
//	struct Common
//	{
//		ID3D11Device*	device;
//		// デバイスコンテキストへのポインタ
//		ID3D11DeviceContext*	deviceContext;
//		// 描画ステートへのポインタ
//		std::unique_ptr<DirectX::CommonStates>	states;
//		// 共用のエフェクトファクトリ
//		std::unique_ptr<DirectX::EffectFactory>	effectFactory;
//		// 共用のカメラ（描画時に使用）
//		Camera* camera;
//		// 読み込み済みモデルコンテナ
//		std::map<std::wstring, std::unique_ptr<DirectX::Model>> modelarray;
//		// 減算描画ステート
//		ID3D11BlendState* blendStateSubtract;
//	};
//	// 静的メンバ関数
//	// システム初期化
//	static void InitializeCommon(CommonDef def);
//	// 全モデル破棄
//	static void UnloadAll();
//
//	// 減算描画設定をセット
//	static void SetSubtractive();
//
//	//getter
//	static Camera* GetCamera() { return s_Common.camera; }
//
//private:
//	// 静的メンバ変数（全オブジェクトで共有）
//	static Common s_Common;
//
//	/// <summary>
//	/// non-static member
//	/// </summary>
//public:
//	Obj3d();
//	// ファイルからモデルを読み込む
//	void LoadModel(const wchar_t*filename = nullptr);
//	// 行列の更新
//	void Update();
//	// 描画
//	void Draw();
//	// 減算描画での描画（影用）
//	void DrawSubtractive();
//	void DrawBillboard();
//	void DrawBillboardConstrainY();
//	// オブジェクトのライティングを無効にする
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
//	void ChangeOrder(WorldMatrixOrder::ORDER order);//ワールド行列の掛け算を入れ替える
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
//	// モデルデータへのポインタ
//	const DirectX::Model* m_pModel;
//	// 平行移動
//	DirectX::SimpleMath::Vector3 m_Trans;
//	// 回転
//	bool m_UseQuternion;
//	DirectX::SimpleMath::Vector3 m_Rot;
//	DirectX::SimpleMath::Quaternion m_RotQ;
//	// スケーリング
//	DirectX::SimpleMath::Vector3 m_Scale;
//	// ワールド行列
//	DirectX::SimpleMath::Matrix m_World;
//	// 親のワールド行列へのポインタ
//	Obj3d* m_pParent;
//	// ワールド行列の掛け算順序(ファクトリーメゾット)
//	std::unique_ptr<WorldMatrixOrder> m_order;
//	//ワールド行列の掛け算順序ファクトリー
//	WorldMatrixOrderFactory WorldMatrixOrderFactory;
//};
//
