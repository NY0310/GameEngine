#pragma once

namespace NYLibrary
{
	class Element;

	class Component
	{
	public:
		//‰Šú‰»
		virtual void Initialize() = 0;
		//XV
		virtual void Update() = 0;
		//I—¹
		virtual void Finalize() = 0;
	};
};