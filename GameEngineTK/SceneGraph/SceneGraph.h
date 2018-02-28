/// <summary>
/// namespace SceneGraph を使用すれば自動的に必要なものを
/// usingする
/// Yusuke Nakata
/// 2017/10/13
/// </summary>
#pragma once


#include "../Node/GameObjectNode.h"

namespace SceneGraph
{
	using namespace scenegraph;
	using GameObjNode = std::shared_ptr<GameObjectNode>;
}

	