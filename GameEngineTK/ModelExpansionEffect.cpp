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
	const wchar_t*filename,	// �t�@�C����
	int frame,	// �t���[����
	Vector3 position,	// ���W
	Vector3 velocity,	// ���x
	Vector3 accel,	// �����x
	Vector3 start_rotation, Vector3 end_rotation, // ��]�E�J�n�p�x�ƏI���p�x
	Vector3 start_scale, Vector3 end_scale // �g�k�E�J�n�{���ƏI���{��
	, int amount//�g�U����G�t�F�N�g��
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
