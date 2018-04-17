#pragma once
#include "../Character.h"
#include "../../../../NYLibrary/SkinMeshAnimation/SkinMesh.h"
#include "../../../../PaintAsset/PaintGun/PaintGun.h"

class PlayerState;
class Player : public Character , public NYLibrary::SkinMesh
{
public:
	//�R���X�g���N�^
	Player();
	//�f�X�g���N�^
	~Player();
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
	//�G�C���̍s��Ǘ�
	std::shared_ptr<MatrixObject> aimMatrix;
private:
	//�y�C���g�e
	std::shared_ptr<PaintGun> paintGun;
	//�v���C���̏��
	PlayerState* playerState;
};

#include "State/PlayerState.h"
