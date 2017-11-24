//--------------------------------------------------------------------------------------
// ファイル名: Player.cpp
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

;


Player::Player(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;
	Initialize();


}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Player::Initialize()
{
	auto& devices = Devices::Get();


	//	自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);

	m_Obj[PARTS_BODY].LoadModel(L"Resources\\Body2.cmo");
	m_Obj[PARTS_BREAST].LoadModel(L"Resources\\Body.cmo");

	m_Obj[PARTS_FING_L].LoadModel(L"Resources\\Fing.cmo");
	m_Obj[PARTS_FING_R].LoadModel(L"Resources\\Fing.cmo");

	//for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	//{
	//	m_Obj[i].LoadModel(L"Resources\\head.cmo");
	//}


	m_Obj[PARTS_LEG_L].LoadModel(L"Resources\\Leg.cmo");
	m_Obj[PARTS_LEG_R].LoadModel(L"Resources\\Leg.cmo");

	//	親子関係の構築(子パーツに親を設定)
	m_Obj[PARTS_FING_L].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_FING_R].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_BREAST]);
	m_Obj[PARTS_LEG_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_LEG_R].SetObjParent(&m_Obj[PARTS_BODY]);


	//for (int i = PARTS_HEAD1; i <= PARTS_HEAD6; i++)
	//{
	//	m_Obj[i].SetObjParent(&m_Obj[PARTS_BREAST]);
	//}


	m_Obj[PARTS_BREAST].SetTrans(Vector3(0.0f, 0.0f, 0.0f));

	//	子パーツの親からのオフセット(座標のずれ)をセット
	m_Obj[PARTS_BODY].SetTrans(Vector3(0, 0.5f, 0));
	m_Obj[PARTS_LEG_L].SetTrans(Vector3(-0.5f, -1.7f, 0));
	m_Obj[PARTS_LEG_R].SetTrans(Vector3(0.5f, -1.7f, 0.0));
	m_Obj[PARTS_FING_L].SetTrans(Vector3(-0.5f, 1.0f, 0));
	m_Obj[PARTS_FING_R].SetTrans(Vector3(0.5f, 1.0f, 0.0));

	m_Obj[PARTS_LEG_L].SetRot(Vector3(-90.0f, 0, 0));
	m_Obj[PARTS_LEG_R].SetRot(Vector3(-90.0f, 0, 0));

	m_Obj[PARTS_FING_L].SetRot(Vector3(0, -90.0f, 0));
	m_Obj[PARTS_FING_R].SetRot(Vector3(0, 90.0f, 0));


	//for (int i = 0; i < MAX_HEAD; i++)
	//{
	//	Start_head_rotate[i] = 60 * i + 1;

	//	m_Obj[PARTS_HEAD1 + i].ChangeOrder(WorldMatrixOrder::ORDER::SCALEM_TRANSMAT_ROTOMAT);

	//	m_Obj[PARTS_HEAD1 + i].SetTrans(Vector3(2.0f, 2.0f, 0));

	//	m_Obj[PARTS_HEAD1 + i].SetRot(Vector3(0, XMConvertToRadians(Start_head_rotate[i]), 0));
	//}


	FingRotX = 0.0f;
	head_rotate = 0;
	FiringCnt = 0;
	flag = true;
	//for (int i = 0; i < MAX_HEAD; i++)
	//{
	//	Flag[i] = false;
	//}

	m_time = 0;

	head_reset = false;

	head_rotate = 1.0;

	hitcnt = 0;


	Microsoft::WRL::ComPtr<ID3D11Resource> resTexture;
	// テクスチャのロード
	CreateWICTextureFromFile(devices.Device().Get(), L"HP.png", resTexture.GetAddressOf(),
		m_texture.ReleaseAndGetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	resTexture.As(&texture);

	// テクスチャの中心を割り出す
	CD3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	m_origin.x = float(textureDesc.Width / 2);
	m_origin.y = float(textureDesc.Height / 2);



	Microsoft::WRL::ComPtr<ID3D11Resource> hitresTexture;
	// テクスチャのロード
	CreateWICTextureFromFile(devices.Device().Get(), L"homing.png", hitresTexture.GetAddressOf(),
		m_hittexture.ReleaseAndGetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> hittexture;
	hitresTexture.As(&hittexture);





	{// 全体用の当たり判定ノードの設定
		m_CollisionNodeBody.Initialize();
		// 親パーツを設定
		m_CollisionNodeBody.SetParant(&m_Obj[0]);
		m_CollisionNodeBody.SetTrans(Vector3(0, 2, 0));
		m_CollisionNodeBody.SetLocalRadious(2.0f);
	}


	m_Shadow.LoadModel(L"shadow.cmo");
	m_Shadow.SetTrans(Vector3(0, SHADOW_OFFSET, 0));

	m_isJump = false;


	m_hp = MAX_HP;

}
//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();



	//m_r++;
	//float rot = XMConvertToRadians(m_r);

	//m_Scale = Matrix::CreateScale(sinf(rot) + 1.0f);
	//
	if (KeybordTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		//ジャンプ開始
		// StartJump();
	}
	Vector3 inter;

	//落下中に
	if (m_isJump)
	{
		//重力による加速
		m_Velocity.y -= GRAVITY_ACC;
		//重力速度の限界処理
		if (m_Velocity.y < -JUMP_SPEED_MAX)
		{
			m_Velocity.y = -JUMP_SPEED_MAX;
		}

		// 影の位置更新
		Vector3 shadow_pos = m_Obj[0].GetTrans();
		shadow_pos.y = inter.y + SHADOW_OFFSET;
		m_Shadow.SetTrans(shadow_pos);
	}


	// 影の更新
	{
		Vector3 pos = m_Obj[0].GetTrans();
		pos.y = SHADOW_OFFSET;
		m_Shadow.SetTrans(pos);
	}

	// 垂直方向地形あたり判定
	{
		const Vector3& vel = GetVelocity();

		bool hit = false;
		Segment player_segment;
		Vector3 player_pos = GetTrans();
		player_segment.Start = player_pos + Vector3(0, SEGMENT_LENGTH, 0);
		// 影の位置を検出するため、足元から下方向に余裕をもって判定を取る
		player_segment.End = player_pos + Vector3(0, -SHADOW_DISTANCE, 0);

		// 大きい数字で初期化
		float distance = 1.0e5;



		float temp_distance;
		Vector3 temp_inter;


		Stage stage;
		// 床面との当たりを判定
		if ((&stage.GetLandShape())->IntersectSegment(player_segment, &temp_inter))
		{
			hit = true;
			temp_distance = Vector3::Distance(player_segment.Start, temp_inter);
			if (temp_distance < distance)
			{
				inter = temp_inter;
				distance = temp_distance;
			}
		}

	}



	{//速度による移動
		Vector3 trans = GetTrans();
		trans += m_Velocity;
		SetTrans(trans);

	}


	if (KeybordTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		BulletDate data;
		data.Trans = m_Obj[0].GetTrans() + Vector3(0, 1, 0);

		Matrix worldm = m_Obj[0].GetLocalWorld();

		// 抽出した情報をしまっておく変数
		Vector3 scale;			// ワールドスケーリング
		Quaternion rotation;	// ワールド回転
		Vector3 translation;	// ワールド座標

								// ワールド行列から各要素を抽出
		worldm.Decompose(scale, rotation, translation);


		//弾丸パーツに速度を設定
		data.Velocity = Vector3(0, 0, -0.5f);
		//パーツの向きに合わせて速度ベクトルを回転
		data.Velocity = Vector3::Transform(data.Velocity, rotation);


		Bullet* bullet = new Bullet(data);


		m_bullet.push_back(bullet);

		{//弾丸用の当たり判定ノードの設定
			SphereNode* spherenode = new SphereNode();
			spherenode->Initialize();
			//
			spherenode->SetParant(&bullet->GetObj3d());
			//
			spherenode->SetTrans(Vector3(0.0f, 0.0f, 0));
			spherenode->SetLocalRadious(0.2f);
			m_CollisionNodeBullet.push_back(spherenode);
		}

	}



	for (vector<Bullet*>::iterator it = m_bullet.begin(); it != m_bullet.end(); it++)
	{
		(*it)->Update();

	}


	//	左旋回処理
	if (keystate.D)
	{
		//	自機の角度を回転
		//head_rot += -0.03f;
		float angle = m_Obj[0].GetRot().y;
		m_Obj[0].SetRot(Vector3(0, angle - 0.03f, 0));
	}

	//	左旋回処理
	if (keystate.A)
	{
		//	自機の角度を回転
		//head_rot += 0.03f;
		float angle = m_Obj[0].GetRot().y;
		m_Obj[0].SetRot(Vector3(0, angle + 0.03f, 0));
	}


	//	前進処理
	if (keystate.W)
	{
		//	移動ベクトル
		Vector3 moveV(0.0f, 0.0f, -0.2f);
		//	移動ベクトルを自機の角度分回転させる
		moveV = Vector3::TransformNormal(moveV, head_world);

		float angle = m_Obj[0].GetRot().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		//	自機の座標を移動
		Vector3 pos = m_Obj[0].GetTrans();
		//head_pos += moveV;
		m_Obj[0].SetTrans(pos + moveV);

		//m_Obj[0].SetRot(Vector3(-0.5, m_Obj[PARTS_BREAST].GetRot().y, m_Obj[PARTS_BREAST].GetRot().z));

	}
	/*else
	{
	m_Obj[PARTS_BREAST].SetRot(
	Vector3(0,
	m_Obj[PARTS_BREAST].GetRot().y,
	m_Obj[PARTS_BREAST].GetRot().z)
	);
	}*/


	//キーボードの様態を取得
	Keyboard::State kb = m_pKeyboard->GetState();
	//キーボードトラッカーの更新
	KeybordTracker.Update(kb);






	//	後退処理
	if (keystate.S)
	{
		//	移動ベクトル
		Vector3 moveV(0.0f, 0.0f, 0.2f);
		//	移動ベクトルを自機の角度分回転させる
		moveV = Vector3::TransformNormal(moveV, head_world);

		float angle = m_Obj[0].GetRot().y;

		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		//	自機の座標を移動
		Vector3 pos = m_Obj[0].GetTrans();
		//head_pos += moveV;
		m_Obj[0].SetTrans(pos + moveV);


	}







	//for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	//{
	//	it->Update();
	//}

	Vector3 a = GetTrans();

	head_rotate = 1.0;

	Calc();
}
//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Player::Calc()
{
	// 全パーツ分行列更新
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Update();
	}

	for (std::vector<Bullet*>::iterator it = m_bullet.begin();
		it != m_bullet.end();
		it++) {
		(*it)->Update();
	}


	for (std::vector<SphereNode*>::iterator Sphereit = m_CollisionNodeBullet.begin(); Sphereit != m_CollisionNodeBullet.end(); Sphereit++)

	{
		(*Sphereit)->Update();
		(*Sphereit)->Draw();
	}

	// 影の更新
	m_Shadow.Update();


}
//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Player::Draw()
{
	auto& devices = Devices::Get();


	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Draw();
	}


	// 影を減算描画
	m_Shadow.DrawSubtractive();


	for (vector<Bullet*>::iterator it = m_bullet.begin(); it != m_bullet.end(); it++)
	{
		(*it)->Draw();
	}


	float  a = static_cast<float>(m_hp) / static_cast<float>(MAX_HP);
	const RECT rect = { 0, 0, 170 * a, 30 };
	devices.SpriteBatch()->Draw(m_texture.Get(), Vector2(410, 560), &rect, Colors::White,
		0.0f, m_origin, Vector2(1.0f, 1.0f), SpriteEffects_None, 0.0f);


	float  b = static_cast<float>(hitcnt) / static_cast<float>(MAX_HOMING);
	const RECT homingrect = { 0, 0, 170 * b, 30 };
	Color color;
	if (hitcnt == MAX_HOMING)
	{
		color = Colors::Red;
	}
	else
	{
		color = Colors::White;
	}
	devices.SpriteBatch()->Draw(m_hittexture.Get(), Vector2(100, 440), &homingrect, color,
		-1.6f, m_origin, Vector2(1.0f, 1.0f), SpriteEffects_None, 0.0f);



	m_CollisionNodeBody.Update();
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetTrans();
}
void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetTrans(trans);
}
const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetLocalWorld();
}

void Player::FireBullet(int parts)
{
	//親子関係を加味したワールド座標を取得する
	Matrix worldm = m_Obj[parts].GetLocalWorld();
	Vector3 scale;
	Quaternion rotq;
	Vector3 pos;
	////行列の1行ずつを、Vector3として扱う
	//Vector3* m0 = (Vector3*)&worldm.m[0];
	//Vector3* m1 = (Vector3*)&worldm.m[1];
	//Vector3* m2 = (Vector3*)&worldm.m[2];
	//Vector3* m3 = (Vector3*)&worldm.m[3];

	//ワールド座行列から各要素を抽出
	worldm.Decompose(scale, rotq, pos);

	//親パーツから分離、独立させる
	m_Obj[parts].SetObjParent(nullptr);
	m_Obj[parts].SetScale(scale);

	m_Obj[parts].SetRotQ(rotq);

	m_Obj[parts].SetTrans(pos);


	//弾丸パーツに速度を設定
	m_BulletVel[parts] = Vector3(0, 0, -0.1f);
	//パーツの向きに合わせて速度ベクトルを回転
	m_BulletVel[parts] = Vector3::Transform(m_BulletVel[parts], rotq);
}



/// <summary>
/// ジャンプを開始
/// </summary>
void Player::StartJump()
{
	if (!m_isJump)
	{
		// 上方向の初速を設定
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

//落下開始
void Player::StartFall()
{
	if (!m_isJump)
	{
		//上方向の初速を設定
		m_Velocity.y = 0;
		m_isJump = true;
	}
}

void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}

//落下を終了


//ラープ4
float Player::Lerp(float startposition, float gorlpositon, float time)
{
	return startposition * (1 - time) + gorlpositon * time;
}