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
//		//���_�\���̂��擾
//		virtual const Vertex SetVertex(const void*& sysmem) = 0;
//		////�o�[�e�b�N�X�o�b�t�@�[�N���A
//		//virtual void ClearVertexBuffer() = 0;
//		//���W�N���A
//		virtual void ClearPosition() = 0;
//		//�@���N���A
//		virtual void ClearNormal() = 0;
//		//�e�N�X�`�����W�N���A
//		virtual void ClearUv() = 0;
//
//		virtual void SetPosition(const D3DXVECTOR3& position) = 0;
//		virtual void SetNormal(const D3DXVECTOR3& normal) = 0;
//		virtual void SetUv(const D3DXVECTOR2& uv) = 0;
//		////�o�[�e�b�N�X�o�b�t�@�[�擾
//		const std::unique_ptr<ID3D11Buffer*>& GetVertexBuffer() { return vertexBuffer; }
//		//�o�[�e�b�N�X�o�b�t�@�[�̃T�C�Y���擾
//		virtual UINT GetByteWidth() = 0;
//	private:
//		//�o�[�e�b�N�X�o�b�t�@�[
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
//		//�o�[�e�b�N�X�o�b�t�@���擾
//		//���_�\���̂��擾
//		const Vertex SetVertex(const void*& sysmem)override { sysmem = vertex; }
//
//		//void SetPosition(const D3DXVECTOR3& position)override { vertex.position.emplace_back(position); }
//		//�o�[�e�b�N�X�o�b�t�@�[���W�N���A
//		//void ClearPosition()override { vertex.position.clear(); }
//		//�o�[�e�b�N�X�o�b�t�@�[�̃T�C�Y���擾
//		UINT GetByteWidth()override{ return sizeof(Vertex); };
//
//	private:
//		void SetNormal(const D3DXVECTOR3& normal)override {};
//		void SetUv(const D3DXVECTOR2& uv)override {};
//		//�o�[�e�b�N�X�o�b�t�@�[�@���N���A
//		void ClearNormal()override {};
//		//�o�[�e�b�N�X�o�b�t�@�[�e�N�X�`�����W�N���A
//		void ClearUv()override {};
//	//	Vertex vertex[];
//	};
//
//	class VertexBuffer_Pos_NormalManager : public  VertexBufferManager
//	{
//	public:
//		VertexBuffer_Pos_NormalManager() = default;
//		~VertexBuffer_Pos_NormalManager() = default;
//		//�o�[�e�b�N�X�o�b�t�@���擾
//		//template<class T>
//		//const T GetVertex()override { return vertex; }
//		//void SetPosition(const D3DXVECTOR3& position)override { vertex.position.emplace_back(position); }
//		//void SetNormal(const D3DXVECTOR3& normal)override { vertex.normal.emplace_back(normal); }
//		//�o�[�e�b�N�X�o�b�t�@�[���W�N���A
//		//void ClearPosition()override { vertex.position.clear(); }
//		//�o�[�e�b�N�X�o�b�t�@�[�@���N���A
//		//void ClearNormal()override { vertex.normal.clear(); };
//		//�o�[�e�b�N�X�o�b�t�@�[�̃T�C�Y���擾
//		UINT GetByteWidth()override { return sizeof(Vertex_Pos_Normal); };
//
//	private:
//		void SetUv(const D3DXVECTOR2& uv) override {};
//		//�o�[�e�b�N�X�o�b�t�@�[�e�N�X�`�����W�N���A
//		void ClearUv()override {};
//	//	Vertex_Pos_Normal vertex[4];
//	};
//
//	class VertexBuffer_Pos_TexManager : public  VertexBufferManagerAbstract
//	{
//	public:
//		VertexBuffer_Pos_TexManager() = default;
//		~VertexBuffer_Pos_TexManager() = default;
//		//�o�[�e�b�N�X�o�b�t�@���擾
//		//template<class T>
//		//const T GetVertex()override { return Vertex; }
//		//void SetPosition(const D3DXVECTOR3& position)override { Vertex.position.emplace_back(position); }
//		//void SetUv(const D3DXVECTOR2& uv) override { Vertex.tex.emplace_back(uv); };
//		//�o�[�e�b�N�X�o�b�t�@�[���W�N���A
//		//void ClearPosition()override { Vertex.position.clear(); }
//		//�o�[�e�b�N�X�o�b�t�@�[�e�N�X�`�����W�N���A
//		//void ClearUv()override { Vertex.tex.clear(); };
//		//�o�[�e�b�N�X�o�b�t�@�[�̃T�C�Y���擾
//		UINT GetByteWidth()override { return sizeof(Vertex_Pos_Tex); };
//
//
//	private:
//		void SetNormal(const D3DXVECTOR3& normal)override {}
//		//�o�[�e�b�N�X�o�b�t�@�[�@���N���A
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
//		//�o�[�e�b�N�X�o�b�t�@���擾
////		const T GetVertex()override { return vertex; }
//		//void SetPosition(const D3DXVECTOR3& position)override { vertex.position.emplace_back(position); }
//		//void SetNormal(const D3DXVECTOR3& normal)override { vertex.normal.emplace_back(normal); }
//		//void SetUv(const D3DXVECTOR2& uv) override { vertex.tex.emplace_back(uv); };
//		//�o�[�e�b�N�X�o�b�t�@�[���W�N���A
//		//void ClearPosition()override { vertex.position.clear(); }
//		//�o�[�e�b�N�X�o�b�t�@�[�@���N���A
//		//void ClearNormal()override { vertex.normal.clear(); };
//		//�o�[�e�b�N�X�o�b�t�@�[�e�N�X�`�����W�N���A
//		//void ClearUv()override { vertex.tex.clear(); }
//		//�o�[�e�b�N�X�o�b�t�@�[�̃T�C�Y���擾
//		UINT GetByteWidth()override { return sizeof(Vertex_Pos_Normal_Tex); };
//	private:
//	//	Vertex_Pos_Normal_Tex vertex[4];
//	};
//
//
//
//	class Geometry
//	{
//	public:	//���_�v�f
//		enum VertexElement
//		{
//			Position,
//			Uv,
//			Normal,
//
//			Number
//		};
//		//�W�I���g�����
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
//		//�o�[�e�b�N�X�o�b�t�@�[�擾
//	//	ID3D11Buffer* GetVertexBuffer() { return *vertexBuffer.get(); }
//
//	private:
//		//���_�f�[�^��ǉ�����
//		void EntryElement(const VertexElement& element);
//		//���_�f�[�^��ǉ�����
//		void EntryElement(const VertexElement& element, const VertexElement& element1);
//		//���_�v�f���L�������ׂ�
//		bool ElementCheck(const VertexElement element) {
//			//assert(element == VertexElement::Number && "�s���Ȑ��l");
//			if (element != VertexElement::Number)return true; return false;
//		}
//		bool ElementCheck(const VertexElement& element, const VertexElement& element1) {
//			if (!ElementCheck(element))return false;
//			return ElementCheck(element1);
//		}
//		//�S�v�f���܂܂�Ă��邩
//		bool IsAllElement() { for (int i = 0; i < VertexElement::Number; i++) { if (!isEntry[i]) return false; } return true; }
//		//�o�[�e�b�N�X�o�b�t�@�}�l�[�W���쐬
//		VertexBufferManagerAbstract* VertexBufferCreate();
//		//�v�f������
//		void InitializeIsEntry() { for (int i = 0; i < VertexElement::Number; i++) isEntry[i] = false; }
//		//�o�[�e�b�N�X�o�b�t�@�[�쐬
//	//	void CreateVertexBuffer();
//
//	protected:
//
//		Geometry();
//		Geometry(const VertexElement& element);
//		Geometry(const VertexElement& element, const VertexElement& element1);
//		virtual ~Geometry() = default;
//		bool isEntry[Number];
//		//���_��
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
