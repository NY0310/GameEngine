#pragma once
/// <summary>
/// ゲームオブジェクトインターフェイス 
/// このクラスを継承して自作オブジェクトクラスを作成する
/// 2017/10/10
/// Yusuke Nakata
/// </summary>

namespace gameobject
{
	class GameObjectInterface
	{
	public:
		//コンストラクタ
		GameObjectInterface() = default;
		//コピーコンストラクタ禁止
		GameObjectInterface(const GameObjectInterface&) = delete;
		//代入禁止
		GameObjectInterface& operator=(const GameObjectInterface&) = delete;

		//デストラクタ
		virtual ~GameObjectInterface() = default;

		//初期化
		virtual void Initialize() = 0;

		//更新処理
		virtual void Update() = 0;

		//描画
		virtual void Render() = 0;

		//終了
		virtual void Finalize() = 0;
	private:

	};

}


