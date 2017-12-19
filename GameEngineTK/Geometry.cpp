//#include "Geometry.h"
//
//using namespace MyLibrary;
//using namespace std;
//
//Geometry::Geometry()
//{
//	InitializeIsEntry();
//	isEntry[VertexElement::Position] = true;
//	vertexBufferManager = new VertexBufferManager();
//}
//
//Geometry::Geometry(const VertexElement& element)
//{
//	InitializeIsEntry();
//	isEntry[VertexElement::Position] = true;
//	EntryElement(element);
//	vertexBufferManager = VertexBufferCreate();
//}
//
//Geometry::Geometry(const VertexElement& element, const VertexElement& element1)
//{
//	InitializeIsEntry();
//	isEntry[VertexElement::Position] = true;
//	EntryElement(element,element1);
//	vertexBufferManager = VertexBufferCreate();
//}
//
//
//
///// <summary>
///// ���_�f�[�^�̗v�f��ǉ�����
///// </summary>
///// <param name="element">�g�p���钸�_�f�[�^</param>
//void Geometry::EntryElement(const VertexElement& element)
//{
//	if (ElementCheck(element))
//		isEntry[element] = true;
//}
//
//void Geometry::EntryElement(const VertexElement& element, const VertexElement& element1)
//{
//	if (ElementCheck(element, element1))
//	{
//		isEntry[element] = true;
//		isEntry[element1] = true;
//	}
//}
//
///// <summary>
///// �o�[�e�b�N�X�o�b�t�@�쐬
///// </summary>
///// <returns>�쐬�����o�[�e�b�N�X�o�b�t�@</returns>
//VertexBufferManagerAbstract* Geometry::VertexBufferCreate()
//{
//	if (IsAllElement())
//		return new VertexBuffer_Pos_Normal_TexManager();
//	if (isEntry[VertexElement::Position] && isEntry[VertexElement::Normal])
//		return new VertexBuffer_Pos_NormalManager(); 
//	if (isEntry[VertexElement::Position] && isEntry[VertexElement::Uv])
//		return new VertexBuffer_Pos_TexManager();
//	return nullptr;
//}
//
//
//Square::Square()
//{
//	vertexNumber = Kind::Square;
//	position.reserve(Kind::Square);
//	normal.reserve(Kind::Square);
//	tex.reserve(Kind::Square);
//}
//
//Square::Square(const VertexElement && element):Geometry(element)
//{
//	vertexNumber = Kind::Square;
//	position.reserve(Kind::Square);
//	normal.reserve(Kind::Square);
//	tex.reserve(Kind::Square);
//
//	for (auto& data : tex)
//	{
//		static int a = 0;
//		a++;
//	}
//}
//
//Square::Square(const VertexElement && element, const VertexElement && element1) : Geometry(element,element1)
//{
//	vertexNumber = Kind::Square;
//}
//
//void Geometry::SetPosition(D3DXVECTOR3 position[])
//{
//	int cnt = 0;
//	vertexBufferManager->ClearPosition();
//	for (auto& pos : this->position)
//	{
//		pos = position[cnt];
//
//	}
//	vertexBufferManager->CreateVertexBuffer();
//
//}
//
//void Geometry::SetNormal(const D3DXVECTOR3 normal[])
//{
//	vertexBufferManager->ClearNormal();
//	for (int i = 0; i < vertexNumber; i++)
//		vertexBufferManager->SetNormal(normal[i]);
//	CreateVertexBuffer();
//}
//
//void Geometry::SetUv(const D3DXVECTOR2 uv[])
//{
//	vertexBufferManager->ClearUv();
//	for (int i = 0; i < vertexNumber; i++)
//		vertexBufferManager->SetUv(uv[i]);
//	CreateVertexBuffer();
//}
//
//
///// <summary>
///// �o�[�e�b�N�X�o�b�t�@�[�쐬
///// </summary>
///// <param name="vertexnumber">���_��</param>
//void VertexBufferManagerAbstract::CreateVertexBuffer()
//{
//	//�f�o�C�X�擾
//	ID3D11Device* device = Devices::Get().Device().Get();
//
//	//�o�[�e�b�N�X�o�b�t�@�[�쐬
//	D3D11_BUFFER_DESC bd;
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = GetByteWidth() * 4;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.MiscFlags = 0;
//
//
//	vertexBuffer = make_unique<ID3D11Buffer*>();
//	D3D11_SUBRESOURCE_DATA InitData;
//	SetVertex(InitData.pSysMem);
//	(device->CreateBuffer(&bd, &InitData, vertexBuffer.get()));
//}
