//��`
#define MAX_BONE_MATRICES 255

//�O���[�o��
Texture2D texColor: register(t0);//�e�N�X�`���[�� ���W�X�^�[t(n)
SamplerState samLinear : register(s0);//�T���v���[�̓��W�X�^�[s(n)

cbuffer global_0:register(b0)
{
	float4 vLight;  //���C�g�̕����x�N�g��
	float4 vEye;//�J�����ʒu
};

cbuffer global_1:register(b1)
{
	matrix mW;//���[���h�s��
	matrix mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 Ambient=float4(0,0,0,0);//�A���r�G���g��
	float4 Diffuse=float4(1,0,0,0); //�g�U����(�F�j
	float4 Specular=float4(1,1,1,1);//���ʔ���
};

cbuffer bones:register(b2)//�{�[���̃|�[�Y�s�񂪓���
{
	matrix mConstBoneWorld[MAX_BONE_MATRICES];
};

//�X�L�j���O��̒��_�E�@��������
struct Skin
{
	float4 Pos;
	float3 Norm;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSSkinIn
{
	float3 Pos	: POSITION;//�ʒu   
	float3 Norm : NORMAL;//���_�@��
	float2 Tex	: TEXCOORD;//�e�N�X�`���[���W
	uint4  Bones : BONE_INDEX;//�{�[���̃C���f�b�N�X
	float4 Weights : BONE_WEIGHT;//�{�[���̏d��
};
//�s�N�Z���V�F�[�_�[�̓��́i�o�[�e�b�N�X�o�b�t�@�[�̏o�́j�@
struct PSSkinIn
{
	float4 Pos	: SV_Position;//�ʒu
	float3 Norm : NORMAL;//���_�@��
	float2 Tex	: TEXCOORD;//�e�N�X�`���[���W
	float4 Color : COLOR0;//�ŏI�J���[�i���_�V�F�[�_�[�ɂ����Ắj
};

//
//matrix FetchBoneMatrix( uint iBone )
//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ�
matrix FetchBoneMatrix( uint iBone )
{
	return mConstBoneWorld[iBone];
}



//���_���X�L�j���O�i�{�[���ɂ��ړ��j����B

Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos = float4(Input.Pos,1);
	float3 Norm = Input.Norm;
	//�{�[��x
	uint iBone=Input.Bones.x;
	float fWeight=Input.Weights.x;
	matrix m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight * mul(Pos,m);
	Output.Norm+=fWeight * mul(Norm,(float3x3)m);
	//�{�[��y
	iBone=Input.Bones.y;
	fWeight=Input.Weights.y;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//�{�[��z
	iBone=Input.Bones.z;
	fWeight=Input.Weights.z;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//�{�[��w
	iBone=Input.Bones.w;
	fWeight=Input.Weights.w;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);

	return Output;
}

/// <summary>
/// �o�[�e�b�N�X�V�F�[�_�[
/// </summary>
PSSkinIn VSSkin(VSSkinIn input )
{
	PSSkinIn output;
	Skin vSkinned = SkinVert(input);

	output.Pos = mul( vSkinned.Pos, mWVP );
	output.Norm = normalize( mul( vSkinned.Norm, (float3x3)mW ) );
	output.Tex = input.Tex;
	float3 LightDir = normalize(vLight);
	float3 PosWorld = mul(vSkinned.Pos,mW);
	float3 ViewDir = normalize(vEye-PosWorld);
	float3 Normal = normalize(output.Norm);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	output.Color= Diffuse * NL + specular*Specular;

	return output;
}

/// <summary>
/// �s�N�Z���V�F�[�_
/// </summary>
float4 PSSkin(PSSkinIn input) : SV_Target
{	
	float4 TexDiffuse = texColor.Sample( samLinear, input.Tex ); 

	return  input.Color/2+TexDiffuse/2;
}