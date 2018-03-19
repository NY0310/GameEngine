#include "InkParticle.h"

using namespace std;
using namespace DirectX::Colors;
using namespace DirectX::SimpleMath;

//速度	
const float InkParticle::SPEED = 0.5f;
//落下速度
const float InkParticle::GRAVITY = 0.01f;
//ローカルの大きさ
const float InkParticle::ROCAL_SIZE = 10.0f;


/// <summary>
/// コンストラクタ
/// </summary>
InkParticle::InkParticle()
{
	colisitionSegment = make_unique<InkSegment>();
	SetScale(D3DXVECTOR3(0.4, 0.4, 0.4));
	birthFrame = 0;
	isValidity = false;
	SetLocalSize(ROCAL_SIZE);
	AddComponent<InkSegment>();
}

/// <summary>
/// デストラクタ
/// </summary>
InkParticle::~InkParticle()
{
	colisitionSegment.release();
}

/// <summary>
/// 生成(既にメモリは確保されている)
/// </summary>
/// <param name="position">発射位置</param>
/// <param name="nDirection">進行ベクトル</param>
/// <param name="color">色</param>
/// <param name="index">インデックス</param>
void InkParticle::Create(const D3DXVECTOR3& position,const D3DXVECTOR3& nDirection, const D3DXVECTOR4& color,const int index)
{
	assert(!isValidity && "このインクは既に有効状態です");
	SetPosition(position);
	this->direction = nDirection;
	SetRotation(nDirection);
	//このオブジェクトの色を設定する
	SetColor(color);
	//コンポーネント処理を無効にする
	birthFrame = 0;
	isValidity = true;
}

/// <summary>
/// 更新処理
/// </summary>
void InkParticle::Update()
{
	if (isValidity)
	{
		birthFrame++;
		direction.y -= GRAVITY;
		SetPosition(GetPosition() + direction * SPEED);
		CalcQuaternion();
	}
}

/// <summary>
/// 進行方向ベクトルに合わせてクォータニオンで回転させる
/// </summary>
void InkParticle::CalcQuaternion()
{
	//クォータニオン初期化
	D3DXQUATERNION q(0, 0, 0, 1);
	//初期進行方向ベクトル
	D3DXVECTOR3 Zminus(0, 0, -1.0f);
	//初期ベクトルと現在のベクトルのコサイン値を内積で求める
	float cosine = D3DXVec3Dot(&Zminus, &direction);
	//度数からラジアン値に変換
	float delta = acosf(cosine);

	//初期ベクトルから現在のベクトルへ回す際の回転軸を外積で求める
	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &Zminus, &direction);
	D3DXVec3Normalize(&axis, &axis);

	//指定した回転軸周りに指定ラジアンだけ回すクォータニオンを得る
	D3DXQuaternionRotationAxis(&q, &axis, delta);
	SetQuaternion(q);
}


////////////////////////////////////////////////////////////////////




/// <summary>
/// コンストラクタ
/// </summary>
InkParticleManager::InkParticleManager()
{
	intervalFrameCnt = 0;
	isShoot = true;
}

/// <summary>
/// デストラクタ
/// </summary>
InkParticleManager::~InkParticleManager()
{
	std::vector<InkSegment*>().swap(segments);
	//delete[] inkParticle;
}

void InkParticleManager::Initialize()
{
	//全てのインクを予め生成、子供として登録
	for (int i = 0; i<MAX_PARTICLE; i++)
	{
		inkParticle[i] = make_shared<InkParticle>();
		AddChild(inkParticle[i]);
		inkParticle[i]->Destroy();
	}

	//インクオブジェクトのインスタンス生成、初期化
	renderer = make_unique<InkObj2>();
	renderer->Initialize();
	renderer->LoadOBJFile("Resources/OBJ/InkObj.obj");
}


/// <summary>
/// 更新処理
/// </summary>
void InkParticleManager::Update()
{

	InkParticleUpdate();
	SegmentsUpdate();
	InkDataUpdate();
	IntervalUpdate();
}

/// <summary>
/// 描画
/// </summary>
void InkParticleManager::Render()
{
	renderer->Render(inkParticledata);

}

/// <summary>
/// インクを発射する
/// </summary>
/// <param name="emitPosition">発射座標</param>
/// <param name="nDirection">エイㇺのベクトル</param>
/// <param name="color">インクの色</param>
void InkParticleManager::Shoot(const D3DXVECTOR3 & emitPosition, D3DXVECTOR3 & nDirection, const D3DXVECTOR4 & color)
{
	if (isShoot)
	{
		return;
	}

	ShiftDirection(nDirection);
	//D3DXVec3Normalize(&nDirection,&nDirection);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (!inkParticle[i]->IsValidity())
		{
			inkParticle[i]->Create(emitPosition, nDirection, color,i);
			return;
		}
	}
	isShoot = true;
	intervalFrameCnt = 0;
}


/// <summary>
/// インクの削除
/// </summary>
/// <param name="inkSegmentIndex">削除するインクのインデックス(Segmentデータから取得してください。)</param>
void InkParticleManager::Destroy(int inkSegmentIndex)
{
	inkParticle[inkSegmentIndex]->Destroy();
}

/// <summary>
/// インクパーティクルの更新処理
/// </summary>
void InkParticleManager::InkParticleUpdate()
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		inkParticle[i]->Update();
	}
}
	
/// <summary>
/// 線の更新処理
/// </summary>
void InkParticleManager::SegmentsUpdate()
{
	segments.clear();

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (inkParticle[i]->IsValidity())
		{
			segments.emplace_back(inkParticle[i]->GetSegment());
		}
	}

}

/// <summary>
/// 座標の更新処理
/// </summary>
void InkParticleManager::InkDataUpdate()
{
	inkParticledata.clear();
	InkObj2::InkData inkdata;
	for (int i = 0; i < MAX_PARTICLE; i++)	
	{
		if (inkParticle[i]->IsValidity())
		{
		//	inkdata.color = inkParticle[i]->GetSegment()->Color;
			inkdata.wvp = GetChild(i)->GetWVP();
			inkParticledata.emplace_back(inkdata);
		}
	}

}

/// <summary>
/// インターバル更新処理
/// </summary>
void InkParticleManager::IntervalUpdate()
{
	if (INTERVAL_FRAME >= intervalFrameCnt)
	{
		isShoot = false;
	}
	intervalFrameCnt++;
}



/// <summary>
/// 移動ベクトルをずらす
/// </summary>
/// <param name="direction"></param>
/// <returns></returns>
D3DXVECTOR3 InkParticleManager::ShiftDirection(D3DXVECTOR3& direction)
{
	direction.x += RandShiftDirection();
	direction.y += RandShiftDirection();
	return direction;
}

/// <summary>
/// ずらす値を乱数で算出し渡す
/// </summary>
/// <returns>ずらす値</returns>
float InkParticleManager::RandShiftDirection()
{
	float shift =  static_cast<float>(rand() % MAX_SHIFT_DIRECTION) / 1000;
	shift /= 2;
	if (rand() % 2)
		shift *= -1;
	return shift;
}



