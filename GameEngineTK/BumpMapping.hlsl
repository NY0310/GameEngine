//�f�B�t���[�Y�F���e�N�X�`���[�F�ɒu�������@

//�O���[�o��
Texture2D g_texNormal: register(t0);
SamplerState g_samLinear : register(s0);

//�O���[�o��
cbuffer global
{
	matrix g_mW;//���[���h�s��
	matrix g_mWIT;//���[���h�s��̋t�s��̓]�u�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLight;//���C�g�̕����x�N�g��
	float4 g_Ambient;//����	
	float4 g_Diffuse; //�g�U���ˌ�
	float4 g_Specular;//���ʔ��ˌ�
	float4 g_vEye;//�J�����i���_�j
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 HalfVector : TEXCOORD2;
	float3 EyeVector : TEXCOORD3;
	float2 Tex : TEXCOORD4;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD,
	float4 Tangent : TANGENT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 worldPos = mul(Pos, g_mW).xyz;
	float3 LightVector = normalize(g_vLight).xyz;
	float3 EyeVector = normalize(g_vEye.xyz - worldPos);
	float3 HalfVector = normalize(LightVector + EyeVector);

	float3 T = mul(Tangent, (float3x3)g_mWIT);
	float3 N = mul(Normal, (float3x3)g_mWIT);
	float3 B = cross(N, T) * Tangent.w;

	float3x3 TangentToObject = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z);
	output.Normal = mul(Normal, g_mW);
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;
	output.HalfVector = mul(HalfVector, TangentToObject);
	output.Light = mul(LightVector, TangentToObject);

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 HalfVector = normalize(input.HalfVector);
	float3 Light = normalize(input.Light);
	float3 Normal = normalize(g_texNormal.Sample(g_samLinear, input.Tex).rgb * 2.0f - 1.0f);
	float NH = saturate(dot(Normal, HalfVector));
	float NL = saturate(dot(Normal, Light));
	float Power = (NL == 0.0f) ? 0.0f : pow(NH,2);

	float4 FinalColor = (g_Diffuse * NL) + (g_Specular * Power);

	return FinalColor;
}