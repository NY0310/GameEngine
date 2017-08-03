/**
* @file Enemy.h
*
* @brief 自機
*
* @date 2015/06/06
*
* @author 中田　湧介
*/
#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include "Obj3d.h"
#include <WICTextureLoader.h>

#include <vector>
#include "CollisionNode.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <SpriteBatch.h>
#include "Bullet.h"

// 自機
class Enemy
{
public:

	//回転角を取得
	const  DirectX::SimpleMath::Vector3& GetRot() {
		// タンクパーツの回転を返す
		return  m_Obj[0].GetRot();
	};
	void SetRot(const DirectX::SimpleMath::Vector3& rot)
		{
			// タンクパーツの座標を設定
			m_Obj[0].SetRot(rot);
		}
	//	自機パーツ
	enum PARTS
	{
		PARTS_BREAST,	//　胸
		PARTS_BODY,		//	体
		PARTS_FING_L,	//  左翼
		PARTS_FING_R,	//  右翼
		PARTS_HEAD1,		//	頭1
		PARTS_HEAD2,		//	頭2
		PARTS_HEAD3,		//	頭3
		PARTS_HEAD4,		//	頭4
		PARTS_HEAD5,		//	頭5
		PARTS_HEAD6,		//	頭6
		PARTS_LEG_L,		//	左足
		PARTS_LEG_R,		//	右足

		PARTS_NUM
	};
	// コンストラクタ
	Enemy(DirectX::Keyboard* keyboard);
	// デストラクタ
	~Enemy();
	// 初期化
	void Initialize();
	void Load();
	// 毎フレーム更新
	void Update();
	// 行列の計算
	void Calc();
	// 描画
	void Draw();
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	// 座標を設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// ワールド行列を取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	void FireBullet(int parts);

	// 全身用の当たり判定を取得
	SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }


	//画像取得
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() {
		return m_texture;
	}



	bool GetInScreen() { return m_InScreen; }
	DirectX::SimpleMath::Vector2 GetOrigin() { return m_origin; }


	//hp取得
	int GetHp() { return m_hp; }
	//hp設定
	void SetHp(int hp) { m_hp = hp; }


	DirectX::SimpleMath::Vector3 GetBulletVel() { return m_BulletVel; }

	bool SetMoveFlag(bool  flag) { return MoveFlag = flag; }



	const Obj3d* GetObj3d() { return &m_Obj[0]; }


	std::vector<SphereNode*>& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }


protected:
	// キーボード
	DirectX::Keyboard* m_pKeyboard;
	// メンバ変数
	// ロボットの構成パーツ
	std::vector<Obj3d> m_Obj;
	// 速度
	DirectX::SimpleMath::Vector3 m_Velocity;
	// ジャンプ（落下）中か？
	bool m_isJump;


	bool head_reset;


	float FingRotX;

	bool flag;


	bool jumpflag;


	int m_time;

	//頭の数
	const int MAX_HEAD = 6;

	//頭の角度
	float head_rotate;

	float Start_head_rotate[6];


	float Lerp(float startposition, float gorlpositon, float time);



	int FiringCnt;

	bool HeadMoveFlag[6];

	bool Flag[6];
	DirectX::SimpleMath::Vector3 MoveVec[6];


	//キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker KeybordTracker;

	//	自機のワールド行列1
	DirectX::SimpleMath::Matrix head_world;

	//大麻
	int m_timer;
	//目標角度
	float m_DistAngle;

	//回転
	float m_cycle;

	// 全身用の当たり判定
	SphereNode m_CollisionNodeBody;



	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//スクリーンに収まっているか
	bool m_InScreen;
	//スクリーン座標
	DirectX::SimpleMath::Vector2 m_screenPos;


	//画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_texture;

	DirectX::SimpleMath::Vector2 m_origin;

	//体力上限
	const int MAX_HP = 30;
	//体力
	int m_hp;

	//移動するか
	bool MoveFlag;

	//弾丸
	std::vector<Bullet*> m_bullet;
	//弾丸用の当たり判定
	std::vector<SphereNode*> m_CollisionNodeBullet;
};
