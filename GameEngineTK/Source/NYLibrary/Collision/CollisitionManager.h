#pragma once
#include <memory>
#include <map>

class Collider;

class CollisitionManager
{
public:
	//�C���X�^���X���擾����
	CollisitionManager* GetInstance();
	//�X�V����
	void Update();
private:
	//���N���X�̃C���X�^���X
	std::unique_ptr<CollisitionManager> collisitionManager;
	//�R���C�_�[���X�g(�^�O�A�R���C�_�[)
	std::map<std::string, Collider*> colliderList;
};