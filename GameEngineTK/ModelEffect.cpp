#include "ModelEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ModelEffectManager * ModelEffectManager::getInstance()
{
	static ModelEffectManager instance;

	return &instance;
}

void ModelEffectManager::Dispose()
{
	modelEffects.clear();
}

void ModelEffectManager::Entry(
	const wchar_t*filename,	// ファイル名
	int frame,	// フレーム数
	Vector3 position,	// 座標
	Vector3 velocity,	// 速度
	Vector3 accel,	// 加速度
	Vector3 start_rotation, Vector3 end_rotation, // 回転・開始角度と終了角度
	Vector3 start_scale, Vector3 end_scale // 拡縮・開始倍率と終了倍率
)
{
	ModelEffect effect;

	effect.obj.LoadModel(filename);

	effect.position = position;
	effect.velocity = velocity;
	effect.accel = accel;
	effect.s_rotation = start_rotation;
	effect.e_rotation = end_rotation;
	effect.s_scale = start_scale;
	effect.e_scale = end_scale;
	effect.frame = 0;
	effect.num_frame = frame;

	modelEffects.push_back(std::move(effect));

}

void ModelEffectManager::Update()
{
	// モデルエフェクトを全て更新
	std::list<ModelEffect>::iterator it;
	for( it = modelEffects.begin(); it != modelEffects.end(); )
	{
		// 更新
		if (it->Update(gravity))
		{
			it++;
		}
		else
		{
			// 寿命に達したモデルエフェクトを解放する
			it = modelEffects.erase(it);
		}
	}
}

void ModelEffectManager::Draw()
{
	// モデルエフェクトを全て描画
	std::list<ModelEffect>::iterator it;
	for (it = modelEffects.begin(); it != modelEffects.end(); it++)
	{
		it->obj.Draw();
	}
}

void ModelEffectManager::ChengMatrix()
{
	// モデルエフェクトを全て描画
	std::list<ModelEffect>::iterator it;
	for (it = modelEffects.begin(); it != modelEffects.end(); it++)
	{
		//it->obj.transmat_rotomat_change_flag = false;
	}
}

ModelEffectManager::ModelEffectManager()
{
}

bool ModelEffectManager::ModelEffect::Update(const Vector3& gravity)
{
	// 経過フレーム数をカウント
	frame++;
	if (frame > num_frame) return false;
	// 進行度を0～1の範囲に換算
	float f = (float)num_frame / frame;

	// 速度に加速度を加算
	velocity += accel;

	// 重力加速度による加速
	velocity += gravity;

	// 速度による移動
	position += velocity;

	// 角度の線形補完
	Vector3 rotation = (e_rotation - s_rotation) / f;
	rotation += s_rotation;
	rotation.x = XMConvertToRadians(rotation.x);
	rotation.y = XMConvertToRadians(rotation.y);
	rotation.z = XMConvertToRadians(rotation.z);

	// スケールの線形補間
	Vector3 scale = (e_scale - s_scale) / f;
	scale += s_scale;

	obj.SetTrans(position);
	obj.SetRot(rotation);
	obj.SetScale(scale);
	obj.Update();

	return true;
}
