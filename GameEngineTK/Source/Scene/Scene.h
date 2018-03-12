/// <summary>
/// �V�[���f�[�^
/// Yusuke Nakata
/// 2018/01/10
/// </summary>
#pragma once
#include "../NYLibrary/SceneGraph/Node/GameObjectNode.h"
//#include "SceneManager.h"

namespace Scene
{
	//�V�[���̎��
	enum SceneKind
	{
		Title,
		Game,
		Result
	};

	/// <summary>
	/// �V�[���N���X
	/// </summary>
	class SceneAbstract : public NYLibrary::GameObjectNode
	{
	public:
		//�R���X�g���N�^
		SceneAbstract() = default;
		//�f�X�g���N�^
		virtual ~SceneAbstract() = default;
		//������
		virtual void Initialize() = 0;
		//�X�V
		virtual void Update() = 0;
		//�`��
		virtual void Render() = 0;
		//�I��
		virtual void Finalize() = 0;
		//�������󂯎��
		void addListener(std::function<void(SceneKind)> listener) {
			this->listener = listener;
		}
		//�V�[����ύX����
		void RaiseReplaceScene(SceneKind scene) { listener(scene); }
		//�R�s�[�R���X�g���N�^�֎~
		SceneAbstract(const SceneAbstract&) = delete;
		//����֎~
		SceneAbstract& operator=(const SceneAbstract&) = delete;
	protected:
		SceneKind nextSceneKind;
	private:
		//�����_�����󂯎��
		std::function<void(SceneKind)> listener = [&](SceneKind SceneKind) {};
	};




	/// <summary>
	/// Null�V�[��
	/// </summary>
	class NullScene : public SceneAbstract {
	public:
		//�R���X�g���N�^
		NullScene() = default;
		//������
		void Initialize()final{};
		//�X�V
		void Update()final {};
		//�`��
		void Render()final {};
		//�I��
		void Finalize()final {};
		//�R�s�[�R���X�g���N�^�֎~
		NullScene(const NullScene&) = delete;
		//����֎~
		NullScene& operator=(const NullScene&) = delete;
	};
};