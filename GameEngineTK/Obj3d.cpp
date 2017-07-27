#include "Obj3d.h"
#include "VertexTypes.h"
#include <CommonStates.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// 定数
// リソースディレクトリパス
const wstring Obj3d::RESOURCE_DIRECTORY = L"Resources/";
//// ファイル拡張子
//const wstring Obj3d::RESOURCE_EXT = L"";
// 静的メンバ変数の実体
Obj3d::Common Obj3d::s_Common;

void Obj3d::InitializeCommon(CommonDef def)
{
	s_Common.device = def.pDevice;
	s_Common.deviceContext = def.pDeviceContext;
	s_Common.camera = def.pCamera;
	

//	m_order = WorldMatrixOrderFactory.Set(WorldMatrixOrder::ORDER::ROTOMAT_SCALEM_TRANSMAT);

	// エフェクトファクトリ生成
	s_Common.effectFactory = std::make_unique<EffectFactory>(def.pDevice);
	// テクスチャ読み込みパス指定
	s_Common.effectFactory->SetDirectory(RESOURCE_DIRECTORY.c_str());

	// 汎用ステート生成
	s_Common.states = std::make_unique<CommonStates>(def.pDevice);

	// 減算描画用のブレンドステートを作成
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT ret = def.pDevice->CreateBlendState(&desc, &s_Common.blendStateSubtract);
}

void Obj3d::UnloadAll()
{
	s_Common.modelarray.clear();
}

void Obj3d::SetSubtractive()
{
	// 減算描画を設定
	s_Common.deviceContext->OMSetBlendState(s_Common.blendStateSubtract, nullptr, 0xffffff);
}

/// <summary>
/// コンストラクタ
/// </summary>
Obj3d::Obj3d()
	: m_pParent(nullptr)
	, m_pModel(nullptr)
	, m_UseQuternion(false)
{
	// スケールは1倍がデフォルト
	m_Scale = Vector3(1, 1, 1);
	//ワールド行列の掛け算順序ファクトリーの初期化
	m_order = WorldMatrixOrderFactory.Set(WorldMatrixOrder::ORDER::ROTOMAT_SCALEM_TRANSMAT);

}

/// <summary>
/// ファイルからモデルを読み込む
/// </summary>
/// <param name="filename">CMOファイル名</param>
void Obj3d::LoadModel(const wchar_t*filename)
{
	assert(s_Common.effectFactory);

	// 指定ファイルを読み込み済みでないか？
	if (s_Common.modelarray.count(filename) == 0)
	{
		// フルパスに補完
		wstring fullpath_bin = RESOURCE_DIRECTORY + filename;
		// モデルを読み込み、コンテナに登録（キーはファイル名）
		s_Common.modelarray[filename] = Model::CreateFromCMO(s_Common.device, fullpath_bin.c_str(), *s_Common.effectFactory);
	}
	m_pModel = s_Common.modelarray[filename].get();


}

/**
*	@brief オブジェクトのライティングを無効にする
*/
void Obj3d::DisableLighting()
{
	if (m_pModel)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

void Obj3d::EnableAlpha()
{
	if (m_pModel)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect>& ieff = meshpart->effect;
				meshpart->ModifyEffect(s_Common.device, ieff, true);
			}
		}
	}
}


/// <summary>
/// ワールド行列の掛け算を入れ替える
/// </summary>
/// <param name="order"></param>
void Obj3d::ChangeOrder(WorldMatrixOrder::ORDER order)
{
	m_order.reset();
	m_order = this->WorldMatrixOrderFactory.Set(order);
}

void Obj3d::Update()
{
	Matrix scalem;
	Matrix rotm;
	Matrix transm;

	scalem = Matrix::CreateScale(m_Scale);

	if (m_UseQuternion)
	{
		rotm = Matrix::CreateFromQuaternion(m_RotQ);
	}
	else
	{
		rotm = Matrix::CreateFromYawPitchRoll(m_Rot.y, m_Rot.x, m_Rot.z);
	}

	transm = Matrix::CreateTranslation(m_Trans);

	// ワールド行列をSRTの順に合成
	m_World = Matrix::Identity;

	m_order->Calculation(m_World, scalem, rotm,transm);

	// 親行列があれば
	if (m_pParent)
	{
		// 親行列を掛ける
		m_World = m_World * m_pParent->GetLocalWorld();
	}
}

void Obj3d::Draw()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProjection();

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, m_World, view, projection);
	}
}

void Obj3d::DrawSubtractive()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
	//	const Matrix& projection = s_Common.camera->GetProj();

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// 減算描画用の設定関数を渡して描画
	//	m_pModel->Draw(s_Common.deviceContext, *s_Common.states, m_World, view, projection, false, Obj3d::SetSubtractive);
	}
}

/// <summary>
///  ビルボード描画
/// </summary>
void Obj3d::DrawBillboard()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProjection();

		// ビルボード行列をワールド行列に合成
		Matrix world = s_Common.camera->GetBillboard() * m_World;

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// 減算描画用の設定関数を渡して描画
		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, world, view, projection);
	}
}

/// <summary>
///  ビルボード描画
/// </summary>
void Obj3d::DrawBillboardConstrainY()
{
	if (m_pModel)
	{
		assert(s_Common.camera);
		const Matrix& view = s_Common.camera->GetView();
		const Matrix& projection = s_Common.camera->GetProjection();

		// ビルボード行列をワールド行列に合成
		Matrix world = s_Common.camera->GetBillboardConstrainY() * m_World;

		assert(s_Common.deviceContext);
		assert(s_Common.states);

		// 減算描画用の設定関数を渡して描画
		m_pModel->Draw(s_Common.deviceContext, *s_Common.states, world, view, projection);
	}
}
