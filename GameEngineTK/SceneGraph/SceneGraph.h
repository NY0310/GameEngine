/// <summary>
/// namespace SceneGraph ���g�p����Ύ����I�ɕK�v�Ȃ��̂�
/// using����
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

	