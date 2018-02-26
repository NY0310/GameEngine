#pragma once
#include "../Charactor.h"
#include "../../SkinMesh.h"

class Player : public Charactor
{
public:
	//�R���X�g���N�^
	Player(int maxHp);
	//�f�X�g���N�^
	~Player() = default;
	// �R�s�[�R���X�g���N�^�֎~
	Player(const Player&) = delete;
	// ����֎~
	Player& operator=(const Player&) = delete;
	//������
	//void Initialize(DirectX::SimpleMath::Vector3 position);
	//����������
	void Initialize()final;
	//�X�V����
	void Update()final;
	//�`�揈���@
	void Render()final;
	//�I������
	void Finalize()final;
private:
	std::unique_ptr<SkinMesh> animetionSkin;

};

