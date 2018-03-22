#pragma once
//ヘッダーファイルのインクルード

#include <d3d11_1.h>
#include <vector> 
#include <memory>
#include "../../NYLibrary/Collision/Collision.h"
#include "InkObj2.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "../InkSegmentCollider/InkSegmentCollider.h"

/// <summary>
/// インク一滴分
/// </summary>
class InkParticle : public NYLibrary::GameObjectNode
{
public:
	//コンストラクタ
	InkParticle();
	//デストラクタ
	~InkParticle();
	//生成(既にメモリは確保されている)
	void Create(const D3DXVECTOR3& position,const D3DXVECTOR3& nDirection, const D3DXVECTOR4& color, const int index);
	//更新処理
	void Update();
	//描画するか
	bool IsValidity() { return isValidity; }
	//削除(メモリは解放しない)
	void Destroy() {
		SetComponentActive(false);
		isValidity = false;
		birthFrame = 0;
	}

private:
	//移動
	void Move();
	//クォータニオン作成
	void CalcQuaternion();
	//生き残っているか確認
	void LifeCheck();
	//移動ベクトル
	D3DXVECTOR3 direction;
	//速度	
	static const float SPEED;
	//表示してからの時間
	int birthFrame;
	//動くか
	bool isValidity;
	//当たり判定用の線
	std::unique_ptr<NYLibrary::InkSegment> colisitionSegment;
	//落下速度
	static const float GRAVITY;
	//ローカルの大きさ
	static const float ROCAL_SIZE;
	//インク有効時間
	static const int LIFE_FRAME = 120;
public:
	// コピーコンストラクタ禁止
	InkParticle(const InkParticle&) = delete;
	// 代入禁止
	InkParticle& operator=(const InkParticle&) = delete;
};

/// <summary>
/// インクの管理
/// </summary>
class InkParticleManager : public NYLibrary::GameObjectNodeEmpty
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
	void Shoot(const D3DXVECTOR3& emitPosition, D3DXVECTOR3& nDirection, const D3DXVECTOR4& color);
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
	static const int MAX_PARTICLE = 100;
private:
	//インクパーティクルの更新処理
	void InkParticleUpdate();
	////線の更新処理
	//void SegmentsUpdate();
	//座標の更新処理
	void InkDataUpdate();
	//インターバル更新処理
	void IntervalUpdate();
	//方向をずらす
	D3DXVECTOR3 ShiftDirection(D3DXVECTOR3& direction);
	//float一つ分のずらしを算出
	float RandShiftDirection();
private:
	//方向のずらし最大量
	static const int MAX_SHIFT_DIRECTION = 100;
	//インクのパーティクル
	std::shared_ptr<InkParticle> inkParticle[MAX_PARTICLE];
	//インクの描画者
	std::unique_ptr<InkObj2> renderer;
	//インクを出してからまた出すまでのインターバル
	const int INTERVAL_FRAME = 140;
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