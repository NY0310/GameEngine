#include "InkParticle.h"

using namespace std;
using namespace DirectX::Colors;

//速度	
const float InkParticle::SPEED = 0.1f;
//落下速度
const float InkParticle::GRAVITY = 0.01f;
//ローカルの大きさ
const float InkParticle::ROCAL_SIZE = 12.0f;




/// <summary>
/// コンストラクタ
/// </summary>
InkParticle::InkParticle()
{
	colisitionSegment = make_unique<InkSegment>();
	SetScale(0.2f);
	birthFrame = 0;
	isValidity = false;
	SetLocalSize(ROCAL_SIZE);
	AddComponent<InkSegmentCollider>();
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
	SetComponentActive(true);

	Update();
	Calc();
}

/// <summary>
/// 更新処理
/// </summary>
void InkParticle::Update()
{
	if (isValidity)
	{
		birthFrame++;
		LifeCheck();
		Move();
		CalcQuaternion();
	}
}

void InkParticle::Move()
{
	direction.y -= GRAVITY;
	SetPosition(GetPosition() + direction * SPEED);
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

void InkParticle::LifeCheck()
{
	if (birthFrame >= LIFE_FRAME)
	{
		Destroy();
	}
}

void InkParticle::OnCollisiton(Collider * collider)
{
	Destroy();
}


////////////////////////////////////////////////////////////////////

const int InkParticleManager::INTERVAL_FRAME = 10;
const int InkParticleManager::MAX_SHIFT_DIRECTION = 80;


/// <summary>
/// コンストラクタ
/// </summary>
InkParticleManager::InkParticleManager()
{
	intervalFrameCnt = 0;
	isShoot = false;
}

/// <summary>
/// デストラクタ
/// </summary>
InkParticleManager::~InkParticleManager()
{
	for (auto child : inkParticle)
	{
		RemoveChild(child);
		child.reset();
	}
	
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
	renderer = make_unique<InkObj>();
	renderer->Initialize();
	renderer->LoadOBJFile("Resources/OBJ/InkObj.obj");
}


/// <summary>
/// 更新処理
/// </summary>
void InkParticleManager::Update()
{
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
	if (isShoot == true)
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
			isShoot = true;

			return;
		}
	}
}





/// <summary>
/// 座標の更新処理
/// </summary>
void InkParticleManager::InkDataUpdate()
{
	inkParticledata.clear();
	InkObj::InkData inkdata;
	for (int i = 0; i < MAX_PARTICLE; i++)	
	{
		if (inkParticle[i]->IsValidity())
		{
			//D3DXMATRIX matrix;
			//D3DXMATRIX trans;
			//D3DXMATRIX rotMat;
			//D3DXVECTOR3 localposition = GetChild(i)->GetPosition();
			////D3DXMatrixScaling(&mat, objectData->GetScaleX(), objectData->GetScaleY(), objectData->GetScaleZ());
			//D3DXMatrixTranslation(&trans, 0, 0, - 2);
			//if (GetChild(i)->GetIsUseQuternion())
			//{
			//	D3DXMatrixRotationQuaternion(&rotMat, &GetChild(i)->GetQuaternion());
			//}
			//else
			//{
			//	D3DXMatrixRotationYawPitchRoll(&rotMat, GetChild(i)->GetRotation().x, GetChild(i)->GetRotation().y, GetChild(i)->GetRotation().z);
			//}

			//

			//matrix = trans *  GetChild(i)->GetRotationMatrix() * GetChild(i)->GetTransferMatrix();





			//auto camera = FollowCamera::GetInstance();

			//inkdata.wvp = matrix *  camera->GetView() * camera->GetProjection();

			//inkdata.color = GetChild(i)->GetColor();
			//inkParticledata.emplace_back(inkdata);





			//localposition = GetChild(i)->GetPosition();
			////D3DXMatrixScaling(&mat, objectData->GetScaleX(), objectData->GetScaleY(), objectData->GetScaleZ());
			//D3DXMatrixTranslation(&trans, 0, 0, + 2);
			//if (GetChild(i)->GetIsUseQuternion())
			//{
			//	D3DXMatrixRotationQuaternion(&rotMat, &GetChild(i)->GetQuaternion());
			//}
			//else
			//{
			//	D3DXMatrixRotationYawPitchRoll(&rotMat, GetChild(i)->GetRotation().x, GetChild(i)->GetRotation().y, GetChild(i)->GetRotation().z);
			//}
			//matrix = trans *  GetChild(i)->GetRotationMatrix() * GetChild(i)->GetTransferMatrix();
			//inkdata.wvp = matrix *  camera->GetView() * camera->GetProjection();
			//inkdata.color = GetChild(i)->GetColor();
			//inkParticledata.emplace_back(inkdata);

			inkdata.wvp = GetChild(i)->GetWVP();
			inkdata.color = GetChild(i)->GetColor();
			inkParticledata.emplace_back(inkdata);


		}
	}

}

/// <summary>
/// インターバル更新処理
/// </summary>
void InkParticleManager::IntervalUpdate()
{
	intervalFrameCnt++;
	if (INTERVAL_FRAME <= intervalFrameCnt)
	{
		isShoot = false;
		intervalFrameCnt = 0;
	}
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



