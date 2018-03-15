#pragma once
#include <memory>
#include <map>

class Collider;

class CollisitionManager
{
public:
	//インスタンスを取得する
	CollisitionManager* GetInstance();
	//更新処理
	void Update();
private:
	//自クラスのインスタンス
	std::unique_ptr<CollisitionManager> collisitionManager;
	//コライダーリスト(タグ、コライダー)
	std::map<std::string, Collider*> colliderList;
};