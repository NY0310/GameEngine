#pragma once

#include <vector>
#include <memory>
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "Collision.h"


// 地形当たりデータ
class LandShapeData
{
public:
	friend class LandShape;

	// 頂点情報の構造体（ファイルから読み込むデータ）
	struct VERTEX_LANDSHAPE_FILE
	{
		DirectX::SimpleMath::Vector3	Pos;		// 座標
	};

	// 頂点情報の構造体
	struct VERTEX_LANDSHAPE
	{
		DirectX::SimpleMath::Vector3	Pos;		// 座標
		DirectX::SimpleMath::Vector3	Normal;		// 法線
	};

	// ノード情報
	struct NodeInfo
	{
		// 種類 (0:bone 1:mesh)
		int kind;

		// 名前
		char name[32];

		// 兄弟のノード番号
		int sibling;

		// 子供のノード番号
		int child;

		// 基本姿勢
		DirectX::SimpleMath::Vector3 translation;
		DirectX::SimpleMath::Quaternion rotation;
		DirectX::SimpleMath::Vector3 scale;
	};

	// 名前
	std::wstring name;

	// 頂点データ配列
	std::vector<VERTEX_LANDSHAPE> m_Vertices;
	// インデックスデータ配列
	std::vector<USHORT> m_Indices;
	// 三角形データ配列
	std::vector<Triangle> m_Triangles;

	// モデルを読み込み
	static std::unique_ptr<LandShapeData> CreateFromData(const char* meshData);
	static std::unique_ptr<LandShapeData> CreateFromFile(const wchar_t* szFileName);
	// 座標変換
	static void UpdateNodeMatrices(int index, const NodeInfo* nodeInfo_array, DirectX::SimpleMath::Matrix* pParentMatrix, std::vector<std::vector<VERTEX_LANDSHAPE>>& vertexArray);
};