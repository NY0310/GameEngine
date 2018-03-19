#pragma once
#include <list>
#include <string.h>
#include <functional>
#include "Collider/Collider.h"
#include "../Object/LocalObject/LocalObject.h"
#include "../Object/MatrixObject/MatrixObject.h"
#include "../Object/ColorObject/ColorObject.h"

namespace NYLibrary {
	class Element : public MatrixObject,public LocalObject,public ColorObject
	{
	private:
		std::string tag;//�^�O
		std::list<Component*> componentList;// �R���|�[�l���g���X�g
		bool isActive;// �A�N�e�B�u���
	public:
		//�R���X�g���N�^
		Element()
			: isActive(true)
		{
		}

		// �f�X�g���N�^
		virtual ~Element()
		{
			Clear();
		}

		//�X�V����
		virtual void Update()
		{
			// �q�����Ă���R���|�[�l���g�̏���
			for (auto& component : componentList)
			{
				component->Update();
			}
		}

		/// <summary>
		/// �R���|�[�l���g��ǉ�
		/// </summary>
		template <class C> void AddComponent()
		{ 
			//C* type = dynamic_cast<Collider>(C*);
			////�R���C�_�[�Ȃ�MatrixObject�̏���n��
			//if (type)
			//{
			//	componentList.emplace_back(new C(this));
			//}
			//else
			//{
			//	componentList.emplace_back(new C());
			//}

			// �d���h�~
			componentList.unique();
		}

		/// <summary>
		/// �R���|�[�l���g���폜
		/// </summary>
		template <class C> void RemoveComponent()
		{
			for (auto& component : componentList)
			{
				C* type = dynamic_cast<C*>(component);

				if (type)
				{
					componentList.remove(component);
					break;
				}
			}
		}

		/// <summary>
		/// �R���|�[�l���g���擾����
		/// </summary>
		/// <returns>�R���|�[�l���g</returns>
		template <class C> C* GetComponent()
		{
			for (auto& component : componentList_)
			{
				C* type = dynamic_cast<C*>(component);

				if (type)
				{
					return type;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// �R���|�[�l���g���X�g���N���A����
		/// </summary>
		void Clear()
		{
			for (auto& component : componentList)
			{
				delete component;
			}

			componentList.clear();
		}

		//�R���C�_�[�����̃R���C�_�[�ɓ�����ƌĂяo�����
		virtual void OnCollisiton(Collider* collider) {}

		//�V�[���I�u�W�F�N�g�ɂɃV�[���ύX�̃I�u�W�F�N�g�֐���o�^����
		void SetReplaceSceneObject(Collider* collider) {
			std::function<void(Collider*)> thisFunction = std::bind(&Element::OnCollisiton, this, std::placeholders::_1);
			collider->addListener(thisFunction);
		}

		// �A�N�e�B�u��Ԃ�ݒ肷��
		virtual void SetActive(bool active) { isActive = active; }
		// �A�N�e�B�u���擾����
		inline virtual bool GetActive()	const { return isActive; }

		//�^�O��ݒ肷��
		void SetTag(const std::string& tagName) { tag = tagName; }
		//�^�O���擾����
		const std::string& GetTag() { return tag; }
	};
};