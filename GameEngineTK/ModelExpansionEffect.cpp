#include "ModelExpansionEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
ModelExpansionEffect::ModelExpansionEffect()
{
	m_Effect = ModelEffectManager::getInstance();
}

ModelExpansionEffect::~ModelExpansionEffect()
{
}

ModelExpansionEffect* ModelExpansionEffect::getInstance()
{
	static ModelExpansionEffect instance;
	return &instance;

}

void ModelExpansionEffect::Entry(
	const wchar_t*filename,	// ファイル名
	int frame,	// フレーム数
	Vector3 position,	// 座標
	Vector3 velocity,	// 速度
	Vector3 accel,	// 加速度
	Vector3 start_rotation, Vector3 end_rotation, // 回転・開始角度と終了角度
	Vector3 start_scale, Vector3 end_scale // 拡縮・開始倍率と終了倍率
	, int amount//拡散するエフェクト数
) /*: m_amount(amount)*/
{
	m_amount = amount;
	for (int i = 0; i < m_amount; i++)
	{
		m_Effect->ChengMatrix();

		float rot = rand() % 20 / 10;

		Vector3 Rotate = Vector3(360 / m_amount * i);
		Rotate = Vector3(0, rot, 0);
		m_Effect->Entry(filename, frame, position + Vector3(rot), velocity, accel, start_rotation, end_rotation + Rotate, start_scale, end_scale);
	}
}

void ModelExpansionEffect::Update()
{
	m_Effect->Update();
}

void ModelExpansionEffect::Draw()
{
	m_Effect->Draw();
}
