///// <summary>
///// 誘導性能付きの弾
///// </summary>
//#pragma once
//
//#include "Obj3D.h"
//
//class Enemy;
//
//class HomingBullet
//{
//public:
//	// 飛行速度<m/frame>
//	static const float SPEED;
//	// 1フレームでの角度変化制限<度→ラジアン>
//	static const float ANGLE_DELTA_MAX;
//	// 寿命
//	static const int LIFE_TIME;
//
//	HomingBullet();
//
//	void Initialize();
//
//	bool Update();
//
//	void Draw();
//
//	void Fire(const DirectX::SimpleMath::Vector3 & pos, const DirectX::SimpleMath::Vector3 & direction);
//
//	//速度取得
//	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_Velocity = velocity; }
//	//座標取得
//	void SetTarget(Enemy* target) { m_Target = target; }
//	//対象取得
//	const Enemy* GetEnemy() {return m_Target;}
//protected:
//	// 3Dオブジェクト
//	Obj3d m_Obj;
//	// 速度
//	DirectX::SimpleMath::Vector3 m_Velocity;
//	// 対象
//	Enemy* m_Target;
//	// 寿命
//	int m_LifeTime;
//	//ダメージ
//	const int DAMAGE = 10;
//};
//
