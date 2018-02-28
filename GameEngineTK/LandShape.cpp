////--------------------------------------------------------------------------------------
//// ファイル名: LandShape
//// 作成者:
//// 作成日:
//// 説明:地形当たり
////--------------------------------------------------------------------------------------
//
//#include <fstream>
//#include <algorithm>
//#include <DirectXColors.h>
//#include "LandShape.h"
//#include "CollisionNode.h"
//
//using namespace std;
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//
//
//// 静的メンバ変数の実体
//std::unique_ptr<LandShapeCommon> LandShape::s_pCommon;
//std::map<std::wstring, std::unique_ptr<LandShapeData>> LandShape::s_dataarray;
//
//LandShapeCommon::LandShapeCommon(LandShapeCommonDef def)
//{
//	// カメラ
//	m_pCamera = def.pCamera;
//	// 描画ステート
//	m_pStates.reset(new CommonStates(def.pDevice));
//	//// エフェクトファクトリ
//	//m_pEffectFactory.reset(new EffectFactory(def.pDevice));
//	//// プリミティブバッチ
//	//m_pPrimitiveBatch.reset(new PrimitiveBatch<VertexPositionNormal>(def.pDeviceContext, BatchSize * 3, BatchSize));
//	//// エフェクト
//	//m_pEffect.reset(new BasicEffect(def.pDevice));
//	//// ライティング有効
//	//m_pEffect->SetLightingEnabled(true);
//	//// マテリアルカラー設定
//	//m_pEffect->SetAmbientLightColor(Vector3(0, 0.0f, 0));
//	//m_pEffect->SetDiffuseColor(Vector3(1.0f, 1.0f, 1.0f));
//	//// ライト0（グリーン）
//	//m_pEffect->SetLightEnabled(0, true);
//	//m_pEffect->SetLightDiffuseColor(0, Vector3(0.1f, 0.6f, 0.1f));
//	//m_pEffect->SetLightDirection(0, Vector3(0, -1.0f, 0));
//	//// ライト1（ピンク）
//	//m_pEffect->SetLightEnabled(1, true);
//	//m_pEffect->SetLightDiffuseColor(1, Vector3(0.5f, 0.2f, 0.3f));
//	//m_pEffect->SetLightDirection(1, Vector3(-1, 0, -2));
//	//// ライト2（水色）
//	//m_pEffect->SetLightEnabled(2, true);
//	//m_pEffect->SetLightDiffuseColor(2, Vector3(0.3f, 0.3f, 0.6f));
//	//m_pEffect->SetLightDirection(2, Vector3(1, 0, 2));
//	//// フォグ（灰色） ※遠近感を強調するために使う
//	//m_pEffect->SetFogEnabled(true);
//	//m_pEffect->SetFogColor(Colors::White);
//	//m_pEffect->SetFogStart(2.f);
//	//m_pEffect->SetFogEnd(10.f);
//
//
//	//void const* shaderByteCode;
//	//size_t byteCodeLength;
//
//	//// シェーダーの取得
//	//m_pEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
//
//	//// 入力レイアウトの作成
//	//def.pDevice->CreateInputLayout(VertexPositionNormal::InputElements,
//	//	VertexPositionNormal::InputElementCount,
//	//	shaderByteCode, byteCodeLength,
//	//	&m_pInputLayout);
//	//// デバイスコンテキスト
//	//m_pDeviceContext = def.pDeviceContext;
//}
//
//LandShapeCommon::~LandShapeCommon()
//{
//	// 入力レイアウトの解放
//	if (m_pInputLayout)
//	{
//		m_pInputLayout->Release();
//		m_pInputLayout = nullptr;
//	}
//}
//
///**
//*	@brief エフェクトファクトリー生成
//*/
//void LandShape::InitializeCommon(LandShapeCommonDef def)
//{
//	// 既に初期化済み
//	if (s_pCommon)	return;
//
//	// 共通データ生成
//	s_pCommon.reset(new LandShapeCommon(def));
//}
//
////--------------------------------------------------------------------------------------
//// コンストラクタ
////--------------------------------------------------------------------------------------
//LandShape::LandShape()
//	: m_pData(nullptr)
//{
//
//}
//
////--------------------------------------------------------------------------------------
//// 初期化
////--------------------------------------------------------------------------------------
//void LandShape::Initialize(const wstring& filename_bin, const wstring& filename_cmo)
//{
//	// ファイル名が空白でなければ
//	if (filename_bin.size() > 0)
//	{
//		// フルパスに補完
//		wstring fullpath_bin = L"LandShape/" + filename_bin + L".landshape";
//
//		std::map<std::wstring, std::unique_ptr<LandShapeData>>::iterator it;
//		it = s_dataarray.find(fullpath_bin);
//		if (s_dataarray.count(fullpath_bin) == 0)
//		{
//			// モデルの読み込み
//			s_dataarray[fullpath_bin] = LandShapeData::CreateFromFile(fullpath_bin.c_str());
//		}
//		// 地形データをセット
//		m_pData = s_dataarray[fullpath_bin].get();
//	}
//
//	// ファイル名が空白でなければ
//	if (filename_cmo.size() > 0)
//	{
//		// フルパスに補完
//		wstring fullpath_cmo =/* L"Resources/" +*/ filename_cmo + L".cmo";
//		// オブジェクト初期化
//		m_Obj.LoadModel(fullpath_cmo.c_str());
//	}
//}
//
////--------------------------------------------------------------------------------------
//// ワールド行列の計算
////--------------------------------------------------------------------------------------
//void LandShape::Update()
//{
//	m_Obj.Update();
//	// 逆行列を計算
//	const Matrix& localworld = m_Obj.GetLocalWorld();
//	m_WorldLocal = localworld.Invert();
//}
//
////--------------------------------------------------------------------------------------
//// 地形データの描画
////--------------------------------------------------------------------------------------
//void LandShape::Draw()
//{
//	////デバック表示がオフなら
//	//if (CollisionNode::GetDebugVisible() == false)
//	//{
//	//	// モデル描画
//	//	m_Obj.Draw();
//	//}
//	//else if (m_pData)
//	//{
//	//	// デバッグ描画
//	//	const Matrix& view = s_pCommon->m_pCamera->GetView();
//	//	const Matrix& projection = s_pCommon->m_pCamera->GetProjection();
//
//	//	// 作成した行列をエフェクトにセット
//	//	s_pCommon->m_pEffect->SetWorld(m_Obj.GetLocalWorld());
//	//	s_pCommon->m_pEffect->SetView(view);
//	//	s_pCommon->m_pEffect->SetProjection(projection);
//
//	//	// エフェクトの設定（各行列やテクスチャなどを設定している）
//	//	s_pCommon->m_pEffect->Apply(s_pCommon->m_pDeviceContext);
//
//	//	// 深度ステンシル ステートを設定する
//	//	s_pCommon->m_pDeviceContext->OMSetDepthStencilState(s_pCommon->m_pStates->DepthDefault(), 0);
//
//	//	// ブレンディング ステートを設定する
//	//	s_pCommon->m_pDeviceContext->OMSetBlendState(s_pCommon->m_pStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);
//
//	//	// ラスタライザ ステートを設定する 時計回りを非表示
//	//	s_pCommon->m_pDeviceContext->RSSetState(s_pCommon->m_pStates->CullNone());
//
//	//	// サンプラーステートを設定する
//	//	auto samplerState = s_pCommon->m_pStates->PointWrap();
//	//	s_pCommon->m_pDeviceContext->PSSetSamplers(0, 1, &samplerState);
//
//	//	// 入力レイアウトを設定する
//	//	s_pCommon->m_pDeviceContext->IASetInputLayout(s_pCommon->m_pInputLayout);
//
//	//	// 描画開始
//	//	s_pCommon->m_pPrimitiveBatch->Begin();
//
//	//	const uint16_t* pIndex = &m_pData->m_Indices[0];
//	//	int numIndex = m_pData->m_Indices.size();
//
//	//	const VertexPositionNormal* pVertex = (VertexPositionNormal*)&m_pData->m_Vertices[0];
//	//	int numVertex = m_pData->m_Vertices.size();
//
//	//	// 三角形プリミティブの描画
//	//	s_pCommon->m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pIndex, numIndex, pVertex, numVertex);
//
//	//	// 描画終了
//	//	s_pCommon->m_pPrimitiveBatch->End();
//	//}
//}
//
//void LandShape::DisableLighting()
//{
//	//	m_Obj.DisableLighting();
//}
//
////--------------------------------------------------------------------------------------
//// 地形と球の交差判定
//// sphere : 判定球
//// reject : 押し出すベクトル
////--------------------------------------------------------------------------------------
//bool LandShape::IntersectSphere(const Sphere& sphere, Vector3* reject)
//{
//
//	if (m_pData == nullptr) return false;
//
//	// ヒットフラグを初期化
//	bool hit = false;
//	// 大きい数字で初期化
//	float over_length = 1.0e5;
//	Vector3 l_inter;
//	Vector3 l_normal;
//	//	Vector3 l_down;
//	// スケールを取得
//	float scale = GetScale();
//
//	// 球をコピー
//	Sphere localsphere = sphere;
//
//	// スケール0の場合、判定しない
//	if (scale <= 1.0e-10) return false;
//
//	// 球の中心点をワールド座標からモデル座標系に引き込む
//	localsphere.Center = Vector3::Transform(sphere.Center, m_WorldLocal);
//	// 半径をワールドをワールド座標系からモデル座標系に変換
//	localsphere.Radius = sphere.Radius / scale;
//
//	// 三角形の数
//	int nTri = m_pData->m_Triangles.size();
//	// 全ての三角形について
//	for (int i = 0; i < nTri; i++)
//	{
//		float temp_over_length;
//		Vector3 temp_inter;
//
//		const Triangle& tri = m_pData->m_Triangles[i];
//
//		// 三角形と球の当たり判定
//		if (CheckSphere2Triangle(localsphere, tri, &temp_inter))
//		{// ヒットした
//			hit = true;
//			// 衝突点から球の中心へのベクトル
//			Vector3 sub = localsphere.Center - temp_inter;
//			// 球の中心が三角形にめりこんでいる距離を計算
//			temp_over_length = sub.Dot(-tri.Normal);
//
//			// めりこみ具合がここまでで最小なら
//			if (temp_over_length < over_length)
//			{
//				// ヒット座標、法線、めりこみ距離を記録
//				l_inter = temp_inter;
//				l_normal = tri.Normal;
//				over_length = temp_over_length;
//			}
//		}
//	}
//
//	if (hit)
//	{
//		// 距離をモデル座標系からワールド座標系での長さに変換
//		over_length *= scale;
//
//		// ワールド行列を取得
//		const Matrix& localworld = m_Obj.GetLocalWorld();
//
//		// 排斥ベクトルの計算
//		if (reject != nullptr)
//		{
//			// 地形の法線方向をモデル座標系からワールド座標系に変換
//			*reject = Vector3::TransformNormal(l_normal, localworld);
//			reject->Normalize();
//			// めり込み分だけ押し出すベクトルを計算
//			const float extra = 0.05f;
//			float reject_distance = sphere.Radius + over_length + extra;
//			*reject = (*reject) * reject_distance;
//		}
//	}
//
//	return hit;
//}
//
////--------------------------------------------------------------------------------------
//// 地形と線分の交差判定
//// segment : 線分
//// （出力）inter : 交点（ポリゴンの平面上で、点との再接近点の座標を返す）
////--------------------------------------------------------------------------------------
//bool LandShape::IntersectSegment(const Segment& segment, Vector3* inter)
//{
//
//
//	if (m_pData == nullptr) return false;
//
//	// ヒットフラグを初期化
//	bool hit = false;
//	// 大きい数字で初期化
//	float distance = 1.0e5;
//	// 角度判定用に地面とみなす角度の限界値<度>
//	const float limit_angle = XMConvertToRadians(30.0f);
//	Vector3 l_inter;
//
//	// コピー
//	Segment localSegment = segment;
//	// 線分をワールド座標からモデル座標系に引き込む
//	localSegment.Start = Vector3::Transform(localSegment.Start, m_WorldLocal);
//	localSegment.End = Vector3::Transform(localSegment.End, m_WorldLocal);
//	// 線分の方向ベクトルを取得
//	Vector3 segmentNormal = localSegment.End - localSegment.Start;
//	segmentNormal.Normalize();
//
//	// 三角形の数
//	int nTri = m_pData->m_Triangles.size();
//	// 全ての三角形について
//	for (int i = 0; i < nTri; i++)
//	{
//		float temp_distance;
//		Vector3 temp_inter;
//
//		// 上方向ベクトルと法線の内積
//		// 長さが１のベクトル２同士の内積は、コサイン（ベクトルの内積の定義より）
//		float cosine = -segmentNormal.Dot(m_pData->m_Triangles[i].Normal);
//		//// コサイン値から、上方向との角度差を計算
//		//float angle = acosf(cosine);
//		//// 上方向との角度が限界角より大きければ、面の傾きが大きいので、地面とみなさずスキップ
//		//if ( angle > limit_angle ) continue;
//
//		//--高速版--
//		const float limit_cosine = cosf(limit_angle);
//		// コサインが１のときにベクトル間の角度は0度であり、ベクトルの角度差が大きいほど、コサインは小さいので、
//		// コサイン値のまま比較すると、角度の比較の場合と大小関係が逆である
//		// つまり、コサイン値が一定値より小さければ、面の傾きが大きいので、地面とみなさずスキップ
//		if (cosine < limit_cosine) continue;
//		//--高速版ここまで--
//
//		// 線分と三角形（ポリゴン）の交差判定
//		if (CheckSegment2Triangle(localSegment, m_pData->m_Triangles[i], &temp_inter))
//		{
//			hit = true;
//			// 線分の始点と衝突点の距離を計算（めりこみ距離）
//			temp_distance = Vector3::Distance(localSegment.Start, temp_inter);
//			// めりこみ具合がここまでで最小なら
//			if (temp_distance < distance)
//			{
//				// 衝突点の座標、めりこみ距離を記録
//				l_inter = temp_inter;
//				distance = temp_distance;
//			}
//		}
//	}
//
//	if (hit && inter != nullptr)
//	{
//		// 衝突点の座標をモデル座標系からワールド座標系に変換
//		const Matrix& localworld = m_Obj.GetLocalWorld();
//		*inter = Vector3::Transform(l_inter, localworld);
//	}
//
//	return hit;
//}