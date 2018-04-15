#pragma once

namespace NYLibrary
{
	class Element;

	class Component
	{
	public:
		//������
		virtual void Initialize() = 0;
		//�X�V
		virtual void Update() = 0;
		//�I��
		virtual void Finalize() = 0;
	};
};