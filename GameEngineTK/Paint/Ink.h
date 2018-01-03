#include <vector>
#include "../ShaderManager.h"
#include "../FollowCamera.h"

class Campus
{
public:
private:
	struct InkData
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 5.0f;
	//	ID3D11Buffer* vertexBuffer;
	};


	struct CampusVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
		D3DXVECTOR2 Tex; //�e�N�X�`���[���W
	};

public:
	Campus() = default;
	~Campus() = default;
	Campus& operator= (const Campus&) = delete;
	Campus(const Campus&) = delete;

	void CreateInk(D3DXVECTOR4& Color, D3DXVECTOR2& uv, float sclae);
	void CreateVertexBuffer();
	ID3D11ShaderResourceView*& GetInkTexSRV(){ return inkTexSRV; }
	HRESULT InitD3D();
	ID3D11ShaderResourceView*& GetInkNormalMap() { return inkNormalMap; }//�n�C�g�}�b�v	
	void Render();
private:
	ID3D11Buffer* CreateVertexBuffer(InkData& inkdata);
	void InkRender();
	void DripRender();
	void InkRender(InkData& inkdata);
	void SetViewPort(ID3D11RenderTargetView * rtv);
	void ClearViewPort(ID3D11RenderTargetView * rtv);
	const D3DXVECTOR3& ChangeRegularDevice(const D3DXVECTOR3& position) { return D3DXVECTOR3(position.x * 2 - 1, (position.y * 2 - 1) * -1, 0); }
	const D3DXVECTOR2& ChangeRegularDevice(const D3DXVECTOR2& position) { return D3DXVECTOR2(position.x * 2 - 1, (position.y * 2 - 1) * -1); }

	/// <summary>
	/// �S�ẴC���N�������_�����O����L�����p�X
	/// </summary>
	ID3D11InputLayout* inkVertexLayout;//�C���N�e�N�X�`���p���_�C���v�b�g���C�A�E�g
	ID3D11Buffer* inkConstantBuffer;//�C���N�e�N�X�`���p�R���X�^���g�o�b�t�@
	ID3D11Texture2D* inkTex;				//�C���N��h��e�N�X�`��
//	ID3D11Texture2D* inkTexDS;				//�C���N��h��e�N�X�`��
	ID3D11RenderTargetView* inkTexRTV;//�C���N��h��e�N�X�`��TRV
	ID3D11DepthStencilView* inkDSTexDSV;//�[�x�}�b�v�e�N�X�`���[�pDS��DSV	
	ID3D11VertexShader* inkVertexShader;//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
	ID3D11PixelShader* inkPixelShader;//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[
	std::vector<InkData> inkData;//�C���N�f�[�^�z��
	ShaderManager shadermanager;//�V�F�[�_�[�֘A
	FollowCamera* camera = FollowCamera::GetInstance();
	ID3D11Device* device = Devices::Get().Device().Get();//�f�o�C�X
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//�f�o�C�X�R���e�L�X�g
	ID3D11ShaderResourceView* inkTexture;//�C���N�e�N�X�`��
	ID3D11ShaderResourceView* inkTexSRV;   //�C���N��h��e�N�X�`����SRV
	ID3D11ShaderResourceView* oldinkTexSRV;
	ID3D11SamplerState* sampleLimear;//�e�N�X�`���T���v��
	ID3D11ShaderResourceView* inkNormalMap;//�n�C�g�}�b�v	

	ID3D11VertexShader* DripVertexShader;//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
	ID3D11PixelShader* DripPixelShader;//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[


	ID3D11Buffer* oldVertexBuffer;
	ID3D11Texture2D* inkTex2;				//�C���N��h��e�N�X�`��
	ID3D11RenderTargetView* inkTexRTV2;//�C���N��h��e�N�X�`��TRV
	ID3D11DepthStencilView* inkDSTexDSV2;//�[�x�}�b�v�e�N�X�`���[�pDS��DSV	
	ID3D11ShaderResourceView* inkTexSRV2;   //�C���N��h��e�N�X�`����SRV

};