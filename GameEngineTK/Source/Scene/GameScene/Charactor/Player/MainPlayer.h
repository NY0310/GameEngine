#pragma once
#include "../Character.h"
#include "../../../../NYLibrary/SkinMeshAnimation/SkinMesh.h"
#include "../../../../PaintAsset/PaintGun/PaintGun.h"

class PlayerState;
class Player : public Character , public NYLibrary::SkinMesh
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
	//�q����ǉ����e�q�֌W���\�z����
	void CreateAddChild();
	//����������
	void Initialize();
	//�X�V����
	void Update();
	//�I������
	void Finalize();
	//��ԕω�
	void ChangeState(PlayerState* state);
	void Render();
	//�ړ����x��ݒ肷��
	void SetMoveSpeed(float speed) { moveSpeed = speed; }
private:
	std::shared_ptr<PaintGun> paintGun;
	std::unique_ptr<PlayerState>playerState;
	float moveSpeed;
};

#include "State/PlayerState.h"
