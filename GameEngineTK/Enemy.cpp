//--------------------------------------------------------------------------------------
// ファイル名: Enemy.cpp
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------
#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

;


Enemy::Enemy(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Enemy::Initialize()
{
	//	自機パーツの読み込み
m_Obj.resize(PARTS_NUM);
	Load();
	m_Obj[PARTS_BODY].LoadModel(L"Resources\\Body2.cmo");
	m_Obj[PARTS_BREAST].LoadModel(L"Resources\\Body.cmo");

	m_Obj[PARTS_FING_L].LoadModel(L"Resources\\Fing.cmo");
	m_Obj[PARTS_FING_R].LoadModel(L"Resources\\Fing.cmo");

	for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	{
		m_Obj[i].LoadModel(L"Resources\\head.cmo");
	}


	m_Obj[PARTS_LEG_L].LoadModel(L"Resources\\Leg.cmo");
	m_Obj[PARTS_LEG_R].LoadModel(L"Resources\\Leg.cmo");




	//	親子関係の構築(子パーツに親を設定)
	m_Obj[PARTS_FING_L].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_FING_R].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_LEG_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_LEG_R].SetObjParent(&m_Obj[PARTS_BODY]);


	for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	{
		m_Obj[i].SetObjParent(&m_Obj[PARTS_BREAST]);
	}





	m_Obj[PARTS_BREAST].SetTrans(Vector3(10.0f, 3.0f, 0.0f));

	//	子パーツの親からのオフセット(座標のずれ)をセット
	m_Obj[PARTS_BODY].SetTrans(Vector3(0, 0.5f, 0));
	m_Obj[PARTS_LEG_L].SetTrans(Vector3(-0.5f, -1.7f, 0));
	m_Obj[PARTS_LEG_R].SetTrans(Vector3(0.5f, -1.7f, 0.0));
	m_Obj[PARTS_FING_L].SetTrans(Vector3(-0.5f, 1.0f, 0));
	m_Obj[PARTS_FING_R].SetTrans(Vector3(0.5f, 1.0f, 0.0));

	m_Obj[PARTS_LEG_L].SetRot(Vector3(-0.5f, 0, 0));
	m_Obj[PARTS_LEG_R].SetRot(Vector3(-0.5f, 0, 0));

	m_Obj[PARTS_FING_L].SetRot(Vector3(0, -0.5f, 0));
	m_Obj[PARTS_FING_R].SetRot(Vector3(0, 0.5f, 0));


	for (int i = 0; i < MAX_HEAD; i++)
	{
		Start_head_rotate[i] = 60 * i + 1;

		m_Obj[PARTS_HEAD1 + i].ChangeOrder(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT);
		m_Obj[PARTS_HEAD1 + i].SetTrans(Vector3(2.0f, 2.0f, 0));

		m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i]), 0));
	}


	FingRotX = 0.0f;
	head_rotate = 0;
	FiringCnt = 0;
	flag = true;
	for (int i = 0; i < MAX_HEAD; i++)
	{
		Flag[i] = false;
	}

	m_time = 0;

	head_reset = false;

	head_rotate = 1.0;

	//初期位置の設定
	Vector3 pos;
	/*pos.x = rand() % 10;
	pos.z = rand() % 10;*/
	//pos.x = 1;
	//pos.z = 1;
	//pos.y = 4;

	//SetTrans(pos);

	m_timer = 0;
	m_DistAngle = 0;

	m_cycle = 0;





	{// 全体用の当たり判定ノードの設定
		m_CollisionNodeBody.Initialize();
		// 親パーツを設定
		m_CollisionNodeBody.SetParant(&m_Obj[0]);
		m_CollisionNodeBody.SetTrans(Vector3(0, 0, 0));
		m_CollisionNodeBody.SetLocalRadious(2.0f);
	}


}


void Enemy::Load()
{
	wifstream ifs("Player.csv");

	wstring line;

	// 1行目をスキップ
	getline(ifs, line);

	vector<wstring> part_names;
	vector<wstring> parent_names;

	// 名簿からデータを解析
	while (getline(ifs, line)) {

		wstring filepath;
		wstring filename;

		wchar_t del = L',';

		// カンマを空白に置き換え
		std::replace(line.begin(), line.end(), L',', L' ');

		// 1行分を文字列ストリームに変換
		wistringstream stream(line);

		// ファイル名の文字列を読み込み
		stream >> filename;
		// プロジェクトの基点からの相対パスでファイル名を補う
		filepath = L"Resources/" + filename + L".cmo";

		Obj3d obj;
		// 読み込む
		obj.LoadModel(filepath.c_str());

		// スケーリングの読み取り
		Vector3 scale;
		stream >> scale.x;
		stream >> scale.y;
		stream >> scale.z;
		obj.SetScale(scale);

		// 角度の読み取り
		Vector3 rotation;
		stream >> rotation.z;
		stream >> rotation.x;
		stream >> rotation.y;
		obj.SetRot(rotation);

		// 座標の読み取り
		Vector3 position;
		stream >> position.x;
		stream >> position.y;
		stream >> position.z;
		obj.SetTrans(position);

		//m_Obj.push_back(obj);

		wstring parent_name;

		stream >> parent_name;

		part_names.push_back(filename);

		parent_names.push_back(parent_name);
	}

	//// 親子関係の組み立て
	for (int i = 0; i < m_Obj.size(); i++)
	{
		// 親の指定あり
		if (parent_names[i].length() > 0)
		{
			for (int j = 0; j < m_Obj.size(); j++)
			{
				if (j == i) continue;

				// 指定の親発見
				if (part_names[j] == parent_names[i])
				{
					m_Obj[i].SetObjParent(&m_Obj[j]);
				}
			}
		}
	}


}


//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	// キーボードの状態取得


	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;

		// 目標角をランダムに抽選
		// -0.5～+0.5の乱数
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90～+90の乱数
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);

		m_DistAngle += rnd;
	}

	{
		// 自機の角度を回転
		Vector3 rot = GetRot();

		// 今の角度と目標角度の差
		float angle = m_DistAngle - rot.y;
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		// 補間
		rot.y += angle * 0.01f;

		SetRot(rot);
	}



	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 移動ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRot().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTrans();
		m_Obj[0].SetTrans(pos + moveV);
	}


	Vector3 a = GetTrans();
	

	//キーボードの様態を取得
	Keyboard::State kb = m_pKeyboard->GetState();
	//キーボードトラッカーの更新
	KeybordTracker.Update(kb);


	for (int i = 0; i < MAX_HEAD; i++)
	{

		if (HeadMoveFlag[i] == true)
		{
			if (Flag[i] == false)
			{


				Flag[i] = true;
				//親から切り離す
				FireBullet(PARTS_HEAD1 + i);

				m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(m_Obj[0].GetRot()));
			}
			//m_Obj[PARTS_HEAD1 + i].transmat_rotomat_change_flag = true;




			m_Obj[PARTS_HEAD1 + i].SetTrans(
				Vector3(m_Obj[PARTS_HEAD1 + i].GetTrans().x, m_Obj[PARTS_HEAD1 + i].GetTrans().y, m_Obj[PARTS_HEAD1 + i].GetTrans().z - 1 * MoveVec[i].z));


			MoveVec[i].z += 0.5;

		}
		else
		{
			//m_Obj[PARTS_HEAD1 + i].transmat_rotomat_change_flag = false;
			//m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i] + head_rotate), 0));

			Start_head_rotate[i] += head_rotate;
		}
	}

		// //敵のワールド座標に対応するスクリーン座標を得る
	//Vector2 screenPos;
	//if (Obj3d::GetCamera()->(m_Obj[0].GetTrans(), &screenPos))
	//{
	//	// ビューポートの取得
	//	D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();

	//	// 画面の範囲に収まっているか？
	//	if (viewport.TopLeftX <= screenPos.x && screenPos.x <= viewport.TopLeftX + viewport.Width &&
	//		viewport.TopLeftY <= screenPos.y && screenPos.y <= viewport.TopLeftY + viewport.Height)
	//	{
	//		// スクリーン座標を記録
	//		m_screenPos = screenPos;
	//		m_InScreen = true;
	//	}
	//}


	Calc();
}
//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	m_CollisionNodeBody.Update();
}
//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Draw();
	}

	m_CollisionNodeBody.Draw();

}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetTrans();
}
void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetTrans(trans);
}
const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetLocalWorld();
}

//ラープ
float Enemy::Lerp(float startposition, float gorlpositon, float time)
{
	return startposition * (1 - time) + gorlpositon * time;
}





void Enemy::FireBullet(int parts)
{
	//親子関係を加味したワールド座標を取得する
	Matrix worldm = m_Obj[parts].GetLocalWorld();
	Vector3 scale;
	Quaternion rotq;
	Vector3 pos;
	//行列の1行ずつを、Vector3として扱う
	Vector3* m0 = (Vector3*)&worldm.m[0];
	Vector3* m1 = (Vector3*)&worldm.m[1];
	Vector3* m2 = (Vector3*)&worldm.m[2];
	Vector3* m3 = (Vector3*)&worldm.m[3];

	//ワールド座行列から各要素を抽出
	worldm.Decompose(scale, rotq, pos);

	//親パーツから分離、独立させる
	m_Obj[parts].SetObjParent(nullptr);
	m_Obj[parts].SetScale(scale);

	m_Obj[parts].SetRotQ(rotq);

	m_Obj[parts].SetTrans(pos);


	//弾丸パーツに速度を設定
	m_BulletVel = Vector3(0, 0, -0.1f);
	//パーツの向きに合わせて速度ベクトルを回転
	m_BulletVel = Vector3::Transform(m_BulletVel, rotq);
}
