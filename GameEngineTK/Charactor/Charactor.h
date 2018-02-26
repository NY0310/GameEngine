#pragma once
#include <memory>
#include "../MatrixObject.h"
#include "../Node/GameObjectNode.h"

class Charactor : public scenegraph::GameObjectNode
{
public:
	//�R���X�g���N�^
	Charactor(int maxHp);
	//�f�X�g���N�^
	virtual ~Charactor();
	// �R�s�[�R���X�g���N�^�֎~
	Charactor(const Charactor&) = delete;
	// ����֎~
	Charactor& operator=(const Charactor&) = delete;
	//������
	virtual void Initialize();
	//�X�V����
	virtual void Update() = 0;
	//�`�揈���@
	virtual void Render() = 0;
	//�I������
	virtual void Finalize();

protected:
	//�̗͏��
	const int MAX_HP;
	//�c��̗�
	int hp;

	//�s��Ǘ�
	std::unique_ptr<MatrixObject> matrixObject;
};