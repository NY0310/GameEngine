#define D3DXSHADER_DEBUG

Texture2D g_texColor: register(t0);//�e�N�X�`���[�� ���W�X�^�[t(n)
SamplerState g_samLinear : register(s0);//�T���v���[�̓��W�X�^�[s(n)

//�O���[�o��
cbuffer global
{
	//matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	//float4 g_vLightDir;//���C�g�����x�N�g��
	//float4 g_Diffuse;//�g�U����(�F)
	//float4 g_Specular;//���ʔ��ˌ�
	//float4 g_Eye;//�J�����ʒu
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	//float4 Color : COLOR0;
	float4 Normal:NORMAL;
	float2 Tex : TEXCOORD;
	//float4 PosLocal : POSITION;
};
//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION , float4 Normal:NORMAL,float2 Tex: TEXCOORD)
{
	//VS_OUTPUT output = (VS_OUTPUT)0;

	//output.Pos = mul(Pos, g_mWVP);
	//float3 _Normal = mul(Normal, (float3x3)g_mW);
	//_Normal = normalize(_Normal);
	//float3 LightDir = normalize(g_vLightDir);
	//float3 PosWorld = mul(Pos, g_mW);
	//float3 EyeVector = g_Eye - PosWorld;
	//float3 ViewDir = normalize(EyeVector);
	//float4 NL = saturate(dot(_Normal, LightDir));
	//float3 Reflect = normalize(2 * NL * _Normal - LightDir);
	//float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 2);
	//output.Color = g_Diffuse * NL + Specular*g_Specular;


	//return output;
	//Normal.w = 0;//w=0�ňړ������𔽉f�����Ȃ�
	//Normal = mul(Normal, g_mW);

	//output.Color = 1.0f*g_Diffuse*dot(Normal, g_vLightDir);//���̎��̓����o�[�g�̗]����
	//output.Color.w = 1.0f;]
	/*output.Tex = Tex;

	return output;*/
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	//output.Pos = Pos;

	//output.PosLocal = output.Pos;
	//output.Normal = Normal;
	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	//float4 Normal = input.Normal;

	////return g_Diffuse;
	////input.Color.w = 1.0f;
	////return input.Color;
	//Normal.w = 0;//w=0�ňړ������𔽉f�����Ȃ�
	//Normal = mul(Normal, g_mW);
	//Normal = normalize(Normal);

	//float4 Color = 1.0f*g_Diffuse*dot(Normal, g_vLightDir);//���̎��̓����o�[�g�̗]����
	//return input.Color;

	return float4(0.5f, 0.5f, 0.5f, 1.0f);
	//return g_texColor.Sample(g_samLinear, input.Tex); 

	//float4 PosWorld = mul(input.PosLocal,g_mW);
	//float3 Normal = normalize(mul(input.Normal,(float3x3)g_mW));
	//float3 LightDir = normalize(g_vLightDir);
	//float3 EyeVector = g_Eye - PosWorld;
	//float3 ViewDir = normalize(EyeVector);
	//float4 NL = saturate(dot(Normal, LightDir));
	//float3 Reflect = normalize(2 * NL * Normal - LightDir);
	//float4 Specular = pow(saturate(dot(Reflect, ViewDir)),2);

	//return (g_Diffuse * NL + Specular*g_Specular) / 2 + g_texColor.Sample(g_samLinear, input.Tex) / 2;

	
}

//�e�N�j�b�N�͕s�v�B�@�ǂ̃V�F�[�_�[���g�����A�v�����Ŏw�肷��̂ŁB