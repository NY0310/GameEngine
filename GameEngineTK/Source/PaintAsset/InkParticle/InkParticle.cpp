#include "InkParticle.h"

using namespace std;
using namespace DirectX::Colors;
using namespace DirectX::SimpleMath;

//重力
const float InkParticle::gravity = 0.03f;
//速度	
const float InkParticle::speed = 0.1f;


/// <summary>
/// コンストラクタ
/// </summary>
InkParticle::InkParticle()
{
	colisitionSegment = make_unique<InkSegment>();
	matrixObject = make_unique<MatrixObject>();
	matrixObject->SetScale(D3DXVECTOR3(1, 1, 1));
	birthFrame = 0;
	isValidity = false;
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
	matrixObject->SetPosition(position);
	this->direction = nDirection;
	matrixObject->SetRotation(nDirection);
	colisitionSegment->Color = color;
	colisitionSegment->index = index;
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
		auto position = matrixObject->GetPosition() ;
		matrixObject->SetPosition(position + direction * speed);
		matrixObject->Calc();
	}
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
	for (int i = 0; i<MAX_PARTICLE; i++)
	{
		inkParticle[i] = make_shared<InkParticle>();
		inkParticle[i]->Destroy();
		AddChild(inkParticle[i]);
	}
	renderer = make_unique<InkObj2>();
	renderer->Initialize();
	renderer->LoadOBJFile("Resources/OBJ/InkObj2.obj");
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
	D3DXVec3Normalize(&nDirection,&nDirection);

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
			inkdata.color = inkParticle[i]->GetSegment()->Color;
			auto mat = inkParticle[i]->GetWVP();
			inkdata.wvp = mat;
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
D3DXVECTOR3 InkParticleManager::ShiftDirection(D3DXVECTOR3 direction)
{
	direction.x += RandShiftDirection();
	direction.y += RandShiftDirection();
	direction.z += RandShiftDirection();
	return direction;
}

/// <summary>
/// ずらす値を乱数で算出し渡す
/// </summary>
/// <returns>ずらす値</returns>
float InkParticleManager::RandShiftDirection()
{
	return rand() % MAX_SHIFT_DIRECTION / 100;
}



