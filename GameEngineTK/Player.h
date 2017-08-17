/**
* @file Player.h
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
#include <vector>
#include "CollisionNode.h"
#include "LandShape.h"
#include "Stage.h"
#include "Bullet.h"
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
// 自機
class Player
{
public:
	//重力加速度
	const float GRAVITY_ACC = 0.03f;
	//ジャンプ初速
	const float JUMP_SPEED_FIRST = 0.5f;
	//
	const float JUMP_SPEED_MAX = 0.3f;
	//	自機パーツ
	enum PARTS
	{
		PARTS_BREAST,	//　胸
		PARTS_BODY,		//	体
		PARTS_FING_L,	//  左翼
		PARTS_FING_R,	//  右翼
		//PARTS_HEAD1,		//	頭1
		//PARTS_HEAD2,		//	頭2
		//PARTS_HEAD3,		//	頭3
		//PARTS_HEAD4,		//	頭4
		//PARTS_HEAD5,		//	頭5
		//PARTS_HEAD6,		//	頭6
		PARTS_LEG_L,		//	左足
		PARTS_LEG_R,		//	右足

		PARTS_NUM
	};
	// コンストラクタ
	Player(DirectX::Keyboard* keyboard);
	// デストラクタ
	~Player();
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 行列の計算
	void Calc();
	// 描画
	void Draw();
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	//回転角を取得
	const  DirectX::SimpleMath::Vector3& GetRot() {
		// タンクパーツの回転を返す
		return  m_Obj[0].GetRot();
	};
	// 座標を設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// ワールド行列を取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();

	void FireBullet(int parts);
	void StartJump();
	void StartFall();
	////弾丸用の当たり判定を取得する
	std::vector<SphereNode*>& GetCollisionNodeBullet() {

		return m_CollisionNodeBullet;
	}
	// 全身用の当たり判定を取得
	SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

	//ジャンプ
	//void StartJump();
	void StopJump();

	DirectX::SimpleMath::Vector3 GetVelocity() {
		return m_Velocity;
	}

	//hp取得
	int GetHp() { return m_hp; }
	//hp設定
	void SetHp(int hp) { m_hp = hp; }

	//ヒットカウント取得
	int Gethitcnt() { return hitcnt; }
	//ヒットカウント設定
	void Sethitcnt(int cnt) { hitcnt = cnt; }

	void ResetCnt() { hitcnt = 0; }


	int GetMAX_HOMING() { return MAX_HOMING;}


protected:
	// キーボード
	DirectX::Keyboard* m_pKeyboard;

	////キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker KeybordTracker;

	// メンバ変数
	// ロボットの構成パーツ
	std::vector<Obj3d> m_Obj;
	// 速度
	DirectX::SimpleMath::Vector3 m_Velocity;
	// ジャンプ（落下）中か？
	bool m_isJump;

	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel[6];


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



	//	自機のワールド行列1
	DirectX::SimpleMath::Matrix head_world;

	//弾丸用の当たり判定
	std::vector<SphereNode*> m_CollisionNodeBullet;
	// 全身用の当たり判定
	SphereNode m_CollisionNodeBody;

	//影
	Obj3d m_Shadow;


	// 影のオフセット
	const float SHADOW_OFFSET = 0.08f;
	// 足元から影までの最大距離
	const float SHADOW_DISTANCE = 30.0f;
	// 足元から何メートル上まで垂直方向の当たりをとるか
	const float SEGMENT_LENGTH = 1.0f;
	// 足元から何メートル下まで吸着を行うか
	const float ADSORP_LENGTH = 0.5f;



	//弾丸
	std::vector<Bullet*> m_bullet;

	//体力上限
	const int MAX_HP = 100;
	//体力
	int m_hp;

	//hp画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_hittexture;

	DirectX::SimpleMath::Vector2 m_origin;



	//ヒットカウント
	int hitcnt;
	int MAX_HOMING = 10;
};
