#pragma once
#include <list>
#include <string.h>
#include "Component.h"

namespace NYLibrary {
	class Element
	{
	protected:
		std::string tag;
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
				//component->Update(this);
			}
		}

		/// <summary>
		/// �R���|�[�l���g��ǉ�
		/// </summary>
		template <class C> void AddComponent()
		{ 
			componentList.emplace_back(new C);

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