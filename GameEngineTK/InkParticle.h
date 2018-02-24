#pragma once
//ヘッダーファイルのインクルード

#include <d3d11_1.h>
#include <vector> 
#include <memory>
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <cassert>
#include"AStar/Math.h"
#include "Collision.h"
#include "InkObj2.h"
#include "MatrixObject.h"



/// <summary>
/// インク一滴分
/// </summary>
class InkParticle
{
public:
	//コンストラクタ
	InkParticle();
	//デストラクタ
	~InkParticle();
	//生成(既にメモリは確保されている)
	void Create(const DirectX::SimpleMath::Vector3& position,const DirectX::SimpleMath::Vector3& nDirection, const DirectX::SimpleMath::Vector4& color, const int index);
	//更新処理
	void Update();
	//描画するか
	bool IsValidity() { return isValidity; }
	//削除(メモリは解放しない)
	void Destroy() {
		assert(!isValidity, ("このインクは既に無効状態です"));
		isValidity = false;}
	//当たり判定用の線を取得
	InkSegment* GetSegment() {
		return colisitionSegment.get();
	}
	DirectX::SimpleMath::Matrix& GetWVP() {
		auto a =  matrixObject->GetWVP();
		return a;
	}
	////座標を取得
	//const Vector3 GetPosition() {
	//	return  position;
	//}
	////大きさを取得
	//const Vector3 GetScale() {
	//	return scale;
	//}
private:
	//座標
	//Vector3 position;
	//移動ベクトル
	DirectX::SimpleMath::Vector3 direction;
	////大きさ
	//Vector3 scale;
	////回転
	//Vector3 rotation;
	//速度	
	static const float speed;
	//重力
	static const float gravity;
	//表示してからの時間
	int birthFrame;
	//動くか
	bool isValidity;
	//当たり判定用の線
	std::unique_ptr<InkSegment> colisitionSegment;
	//行列管理
	std::unique_ptr<MatrixObject> matrixObject;
public:
	// コピーコンストラクタ禁止
	InkParticle(const InkParticle&) = delete;
	// 代入禁止
	InkParticle& operator=(const InkParticle&) = delete;
};

/// <summary>
/// インクの管理
/// </summary>
class InkParticleManager
{
public:
	//コンストラクタ
	InkParticleManager();
	//デストラクタ
	~InkParticleManager();
	//初期化
	void Initialize();
	//更新処理
	void Update();
	//描画
	void Render();
	//インクを発射する
	void Shoot(const DirectX::SimpleMath::Vector3& emitPosition, DirectX::SimpleMath::Vector3& nDirection, const DirectX::SimpleMath::Vector4& color);
	////座標を取得
	//const std::vector<InkParticleRenderer::ConstantInkData> GetConstantInkData() {
	//	return inkParticledata;
	//}
	//当たり判定用の球を取得
	const std::vector<InkSegment*> GetSegment() {
		return std::vector<InkSegment*>();
	}
	//インクの削除
	void Destroy(int inkSegmentIndex);
	//パーティクル最大数
	static const int MAX_PARTICLE = 50;
private:
	//インクパーティクルの更新処理
	void InkParticleUpdate();
	//線の更新処理
	void SegmentsUpdate();
	//座標の更新処理
	void InkDataUpdate();
	//インターバル更新処理
	void IntervalUpdate();
	//方向をずらす
	DirectX::SimpleMath::Vector3 ShiftDirection(DirectX::SimpleMath::Vector3 direction);
	//float一つ分のずらしを算出
	float RandShiftDirection();
private:
	//方向のずらし最大量
	static const int MAX_SHIFT_DIRECTION = 3;
	//インクのパーティクル
	InkParticle* inkParticle[MAX_PARTICLE];
	//インクの描画者
	std::unique_ptr<InkObj2> renderer;
	//インクを出してからまた出すまでのインターバル
	const int INTERVAL_FRAME = 30;
	//前回インクを出してからの経過時間
	int intervalFrameCnt;
	//当たり判定用の線
	std::vector<InkSegment*> segments;
	//座標を取得
	std::vector<InkObj2::InkData> inkParticledata;
	//発射したか
	bool isShoot;
public:
	// コピーコンストラクタ禁止
	InkParticleManager(const InkParticleManager&) = delete;
	// 代入禁止
	InkParticleManager& operator=(const InkParticleManager&) = delete;
};