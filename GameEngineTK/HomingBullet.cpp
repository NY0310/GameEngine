//#include "HomingBullet.h"
//#include "Enemy.h"
//#include "ModelEffect.h"
//
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//
//// 飛行速度<m/frame>
//const float HomingBullet::SPEED = 0.3f;
//// 1フレームでの角度変化制限<度→ラジアン>
//const float HomingBullet::ANGLE_DELTA_MAX = DirectX::XMConvertToRadians(30.0f);
//// 寿命
//const int HomingBullet::LIFE_TIME = 120;
//
//HomingBullet::HomingBullet()
//{
//	m_Target = nullptr;
//	m_LifeTime = 0;
//}
//
//void HomingBullet::Initialize()
//{
//	m_Velocity = Vector3(0, 0, -SPEED);
//	m_Obj.LoadModel(L"Resources\\head.cmo");
//}
//
///// <summary>
///// 毎フレーム更新
///// </summary>
//bool HomingBullet::Update()
//{
//	bool death = false;
//
//	if (m_LifeTime > 0)
//	{
//		if (--m_LifeTime <= 0)
//		{
//			death = true;
//		}
//	}
//
//	// 誘導対象がいれば
//	if (!death && m_Target)
//	{
//		// 現在の進行方向ベクトルを求める（単に速度ベクトルを正規化したもの）
//		Vector3 moveDir;
//		m_Velocity.Normalize(moveDir);
//
//		// 座標の差分ベクトルを求める
//		Vector3 targetDir = m_Target->GetTrans() + Vector3(0,2,0) - m_Obj.GetTrans();
//		float distance = targetDir.Length();
//		if (distance >= 0.2f)
//		{
//			targetDir.Normalize();
//
//			// 内積の公式から、2ベクトルの内積が角度差のコサイン値
//			// A: 現在の進行方向ベクトル
//			// B: 誘導対象の方向ベクトル
//			float cosine = moveDir.Dot(targetDir);
//			// 2ベクトル角度差<ラジアン>を求める
//			float delta = acosf(cosine);
//			// このフレームでの回転角<ラジアン>を決定
//			float angle;
//			if (distance <= 2.0f)
//			{// 距離が近いと誘導性能があがる
//				angle = min(delta, ANGLE_DELTA_MAX);
//			}
//			else
//			{
//				// 寿命が短いと誘導瀬能があがる
//				float rate = 1.0f - (float)m_LifeTime / LIFE_TIME;
//				angle = min(delta, rate * ANGLE_DELTA_MAX);
//			}
//
//			// AからBへ回す際の回転軸を外積で求める
//			Vector3 axis = moveDir.Cross(targetDir);
//			axis.Normalize();
//			// 指定した回転軸周りに指定ラジアンだけ回すクォータニオンを得る
//			Quaternion qDelta = Quaternion::CreateFromAxisAngle(axis, angle);
//			// 今回の回転分を反映
//			Quaternion q = m_Obj.GetRotQ();
//			q = q * qDelta;
//			m_Obj.SetRotQ(q);
//			// 速度ベクトルを回転
//			m_Velocity = Vector3::Transform(Vector3(0, 0, -SPEED), q);
//		}
//		else
//		{
//		m_Target->SetHp(m_Target->GetHp() - DAMAGE);
//			// 追跡終了（本来なら衝突して弾が消える）
//			m_Target = nullptr;
//
//			death = true;
//		}
//	}
//
//	if (death)
//	{
//		// 追跡終了（本来なら衝突して弾が消える）
//		ModelEffectManager::getInstance()->Entry(
//			L"HitEffect.cmo",
//			10,
//			m_Obj.GetTrans(),	// 座標
//			Vector3(0, 0, 0),	// 速度
//			Vector3(0, 0, 0),	// 加速度
//			Vector3(0, 0, 0),	// 回転角（初期）
//			Vector3(0, 0, 0),	// 回転角（最終）
//			Vector3(0, 0, 0),	// スケール（初期）
//			Vector3(6, 6, 6)	// スケール（最終）
//		);
//
//	
//		return true;
//	}
//
//	// 前進
//	{
//		// 速度分移動する
//		m_Obj.SetTrans(m_Obj.GetTrans() + m_Velocity);
//	}
//
//	// 行列更新
//	m_Obj.Update();
//
//	return false;
//}
//
///// <summary>
///// 描画
///// </summary>
//void HomingBullet::Draw()
//{
//	// 描画
//	m_Obj.Draw();
//}
//
///// <summary>
///// 発射
///// </summary>
///// <param name="direction">進行方向ベクトル</param>
//void HomingBullet::Fire(const DirectX::SimpleMath::Vector3 & pos, const DirectX::SimpleMath::Vector3 & direction)
//{
//	// 寿命
//	m_LifeTime = LIFE_TIME;
//	// 速度
//	m_Velocity = direction;
//	m_Velocity.Normalize();
//	m_Velocity *= SPEED;
//
//	m_Obj.SetTrans(pos);
//
//	// 初期の回転情報を進行方向から算出する
//	{
//		Vector3 Zminus = -Vector3::UnitZ;
//		// 内積の公式から、2ベクトルの内積が角度差のコサイン値
//		// A: デフォルトの進行方向ベクトル(Zマイナス方向)
//		// B: 誘導対象の方向ベクトル
//		float cosine = Zminus.Dot(direction);
//		// 2ベクトル角度差<ラジアン>を求める
//		float delta = acosf(cosine);
//
//		// AからBへ回す際の回転軸を外積で求める
//		Vector3 axis = Zminus.Cross(direction);
//		axis.Normalize();
//		// 指定した回転軸周りに指定ラジアンだけ回すクォータニオンを得る
//		Quaternion q = Quaternion::CreateFromAxisAngle(axis, delta);
//		// 初期回転をセット
//		m_Obj.SetRotQ(q);
//	}
//}
