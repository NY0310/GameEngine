#pragma once

namespace NYLibrary
{
	class Element;

	class Component
	{
	public:
		//初期化
		virtual void Initialize() = 0;
		//更新
		virtual void Update() = 0;
		//終了
		virtual void Finalize() = 0;
	};
};