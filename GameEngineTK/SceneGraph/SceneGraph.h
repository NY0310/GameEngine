/// <summary>
/// namespace SceneGraph を使用すれば自動的に必要なものを
/// usingする
/// Yusuke Nakata
/// 2017/10/13
/// </summary>
#pragma once


#include "../Test/TestCharactor.h"
#include "../Node/GameObjectNode.h"

namespace SceneGraph
{
	using namespace scenegraph;
	using namespace gameobject;
	using GameObj = std::shared_ptr<GameObjectInterface>;
	using GameObjNode = std::shared_ptr<GameObjectNode>;
}

	