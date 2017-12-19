//#pragma once
//#include <d3d11_1.h>
//#include <d3d11.h>
//#include <d3dx10.h>
//#include <d3dx11.h>
//#include <memory>
//#include <vector> 
//#include <cassert>
//#include <functional>
//#include  "Device.h"
//
//namespace MyLibrary
//{
//	struct Vertex
//	{
//		D3DXVECTOR3 position;
//	};
//
//	struct Vertex_Pos_Normal : virtual public Vertex
//	{
//		D3DXVECTOR3 normal;
//	};
//
//
//	struct Vertex_Pos_Tex : virtual public  Vertex
//	{
//		D3DXVECTOR2 tex;
//	};
//
//
//	struct Vertex_Pos_Normal_Tex : public Vertex_Pos_Normal, Vertex_Pos_Tex
//	{
//	};
//
//
//	class VertexBufferManagerAbstract
//	{
//	public:
//		VertexBufferManagerAbstract() = default;
//		virtual ~VertexBufferManagerAbstract() = default;
//
//		//頂点構造体を取得
//		virtual const Vertex SetVertex(const void*& sysmem) = 0;
//		////バーテックスバッファークリア
//		//virtual void ClearVertexBuffer() = 0;
//		//座標クリア
//		virtual void ClearPosition() = 0;
//		//法線クリア
//		virtual void ClearNormal() = 0;
//		//テクスチャ座標クリア
//		virtual void ClearUv() = 0;
//
//		virtual void SetPosition(const D3DXVECTOR3& position) = 0;
//		virtual void SetNormal(const D3DXVECTOR3& normal) = 0;
//		virtual void SetUv(const D3DXVECTOR2& uv) = 0;
//		////バーテックスバッファー取得
//		const std::unique_ptr<ID3D11Buffer*>& GetVertexBuffer() { return vertexBuffer; }
//		//バーテックスバッファーのサイズを取得
//		virtual UINT GetByteWidth() = 0;
//	private:
//		//バーテックスバッファー
//		std::unique_ptr<ID3D11Buffer*> vertexBuffer;
//
//		void CreateVertexBuffer();
//
//	};
//
//
//	class VertexBufferManager : public VertexBufferManagerAbstract
//	{
//	public:
//		VertexBufferManager() = default;
//		virtual ~VertexBufferManager() = default;
//		//バーテックスバッファを取得
//		//頂点構造体を取得
//		const Vertex SetVertex(const void*& sysmem)override { sysmem = vertex; }
//
//		//void SetPosition(const D3DXVECTOR3& position)override { vertex.position.emplace_back(position); }
//		//バーテックスバッファー座標クリア
//		//void ClearPosition()override { vertex.position.clear(); }
//		//バーテックスバッファーのサイズを取得
//		UINT GetByteWidth()override{ return sizeof(Vertex); };
//
//	private:
//		void SetNormal(const D3DXVECTOR3& normal)override {};
//		void SetUv(const D3DXVECTOR2& uv)override {};
//		//バーテックスバッファー法線クリア
//		void ClearNormal()override {};
//		//バーテックスバッファーテクスチャ座標クリア
//		void ClearUv()override {};
//	//	Vertex vertex[];
//	};
//
//	class VertexBuffer_Pos_NormalManager : public  VertexBufferManager
//	{
//	public:
//		VertexBuffer_Pos_NormalManager() = default;
//		~VertexBuffer_Pos_NormalManager() = default;
//		//バーテックスバッファを取得
//		//template<class T>
//		//const T GetVertex()override { return vertex; }
//		//void SetPosition(const D3DXVECTOR3& position)override { vertex.position.emplace_back(position); }
//		//void SetNormal(const D3DXVECTOR3& normal)override { vertex.normal.emplace_back(normal); }
//		//バーテックスバッファー座標クリア
//		//void ClearPosition()override { vertex.position.clear(); }
//		//バーテックスバッファー法線クリア
//		//void ClearNormal()override { vertex.normal.clear(); };
//		//バーテックスバッファーのサイズを取得
//		UINT GetByteWidth()override { return sizeof(Vertex_Pos_Normal); };
//
//	private:
//		void SetUv(const D3DXVECTOR2& uv) override {};
//		//バーテックスバッファーテクスチャ座標クリア
//		void ClearUv()override {};
//	//	Vertex_Pos_Normal vertex[4];
//	};
//
//	class VertexBuffer_Pos_TexManager : public  VertexBufferManagerAbstract
//	{
//	public:
//		VertexBuffer_Pos_TexManager() = default;
//		~VertexBuffer_Pos_TexManager() = default;
//		//バーテックスバッファを取得
//		//template<class T>
//		//const T GetVertex()override { return Vertex; }
//		//void SetPosition(const D3DXVECTOR3& position)override { Vertex.position.emplace_back(position); }
//		//void SetUv(const D3DXVECTOR2& uv) override { Vertex.tex.emplace_back(uv); };
//		//バーテックスバッファー座標クリア
//		//void ClearPosition()override { Vertex.position.clear(); }
//		//バーテックスバッファーテクスチャ座標クリア
//		//void ClearUv()override { Vertex.tex.clear(); };
//		//バーテックスバッファーのサイズを取得
//		UINT GetByteWidth()override { return sizeof(Vertex_Pos_Tex); };
//
//
//	private:
//		void SetNormal(const D3DXVECTOR3& normal)override {}
//		//バーテックスバッファー法線クリア
//		void ClearNormal()override {};
//		Vertex_Pos_Tex Vertex[4];
//	};
//
//
//
//	class VertexBuffer_Pos_Normal_TexManager : public VertexBufferManagerAbstract
//	{
//	public:
//		VertexBuffer_Pos_Normal_TexManager() = default;
//		~VertexBuffer_Pos_Normal_TexManager() = default;
//		//バーテックスバッファを取得
////		const T GetVertex()override { return vertex; }
//		//void SetPosition(const D3DXVECTOR3& position)override { vertex.position.emplace_back(position); }
//		//void SetNormal(const D3DXVECTOR3& normal)override { vertex.normal.emplace_back(normal); }
//		//void SetUv(const D3DXVECTOR2& uv) override { vertex.tex.emplace_back(uv); };
//		//バーテックスバッファー座標クリア
//		//void ClearPosition()override { vertex.position.clear(); }
//		//バーテックスバッファー法線クリア
//		//void ClearNormal()override { vertex.normal.clear(); };
//		//バーテックスバッファーテクスチャ座標クリア
//		//void ClearUv()override { vertex.tex.clear(); }
//		//バーテックスバッファーのサイズを取得
//		UINT GetByteWidth()override { return sizeof(Vertex_Pos_Normal_Tex); };
//	private:
//	//	Vertex_Pos_Normal_Tex vertex[4];
//	};
//
//
//
//	class Geometry
//	{
//	public:	//頂点要素
//		enum VertexElement
//		{
//			Position,
//			Uv,
//			Normal,
//
//			Number
//		};
//		//ジオメトリ種類
//		enum Kind
//		{
//			Point = 1,
//			Sircle = 1,
//			Line,
//			Triangle,
//			Square
//		};
//
//	public:
//
//
//		void SetPosition(D3DXVECTOR3 position[]);
//		void SetNormal(const D3DXVECTOR3 position[]);
//		void SetUv(const D3DXVECTOR2 uv[]);
//
//		//バーテックスバッファー取得
//	//	ID3D11Buffer* GetVertexBuffer() { return *vertexBuffer.get(); }
//
//	private:
//		//頂点データを追加する
//		void EntryElement(const VertexElement& element);
//		//頂点データを追加する
//		void EntryElement(const VertexElement& element, const VertexElement& element1);
//		//頂点要素が有効か調べる
//		bool ElementCheck(const VertexElement element) {
//			//assert(element == VertexElement::Number && "不正な数値");
//			if (element != VertexElement::Number)return true; return false;
//		}
//		bool ElementCheck(const VertexElement& element, const VertexElement& element1) {
//			if (!ElementCheck(element))return false;
//			return ElementCheck(element1);
//		}
//		//全要素が含まれているか
//		bool IsAllElement() { for (int i = 0; i < VertexElement::Number; i++) { if (!isEntry[i]) return false; } return true; }
//		//バーテックスバッファマネージャ作成
//		VertexBufferManagerAbstract* VertexBufferCreate();
//		//要素初期化
//		void InitializeIsEntry() { for (int i = 0; i < VertexElement::Number; i++) isEntry[i] = false; }
//		//バーテックスバッファー作成
//	//	void CreateVertexBuffer();
//
//	protected:
//
//		Geometry();
//		Geometry(const VertexElement& element);
//		Geometry(const VertexElement& element, const VertexElement& element1);
//		virtual ~Geometry() = default;
//		bool isEntry[Number];
//		//頂点数
//		int vertexNumber;
//		VertexBufferManagerAbstract* vertexBufferManager;
//		std::vector<D3DXVECTOR3> position;
//		std::vector<D3DXVECTOR3> normal;
//		std::vector<D3DXVECTOR2> tex;
//
//	};
//
//
//
//	class Square : public Geometry
//	{
//	public:
//		Square();
//		Square(const VertexElement&& element);
//		Square(const VertexElement&& element, const VertexElement&& element1);
//
//		~Square() = default;
//	private:
//	};
//
//}
